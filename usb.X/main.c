#include <xc.h>
#include <stdio.h>
#include <stdlib.h>

#pragma config OSC = INTIO67   // Internal oscillator block
#pragma config FCMEN = OFF     // Fail-Safe Clock Monitor
#pragma config IESO = OFF      // Internal/External Switch Over

#pragma config PWRT = ON       // Power-up Timer
#pragma config BOREN = SBORDIS // Brown-out reset
#pragma config BORV = 3

#pragma config WDT = OFF       // Watchdog Timer OFF

#pragma config CCP2MX = PORTC
#pragma config PBADEN = OFF
#pragma config LPT1OSC = OFF
#pragma config MCLRE = ON

#pragma config LVP = OFF       // MUST disable LVP (RB5 pin)
#pragma config XINST = OFF     // Extended instruction set OFF
#pragma config DEBUG = OFF     // Disable background debug


#define _XTAL_FREQ 4000000

void CH375_ReadKeyboard(void);
void UART_Initialize();
void CH375_InitHost(void);
void CH375_WriteCmd(unsigned char cmd);
void CH375_WriteData(unsigned char data);
unsigned char CH375_ReadData(void);
char HID_KeycodeToASCII(unsigned char code, unsigned char modifier);
void Parse_HID(unsigned char *buf);
void TMR1_Initialize(void);
void UART_Write(unsigned char data);
void UART_Write_Text(char* text);

int num = 0;
int tick = 0;


int main(int argc, char** argv) {

    TRISD = 0x0F; 
    LATD = 0x00;
    UART_Initialize();
    CH375_InitHost();
    TMR1_Initialize();
    UART_Write('0');

    while(1)
    {
        if(tick > 16000){
            UART_Write_Text("0101");
            num ++;
            LATD = (num % 16) << 4;
            tick = 0;
        }
        tick ++;
    }
}

//void __interrupt(low_priority)  Lo_ISR(void)
//{
//    if (PIR1bits.TMR1IF)
//    {
//        PIR1bits.TMR1IF = 0;   // clear the flag    
//        TMR0H = 0xFB;             // preload for 1 sec @ 4MHz
//        TMR0L = 0x1E;   
//        
//    }
//}

void printHex(unsigned char v)
{
    char hex[3];
    const char* h = "0123456789ABCDEF";
    hex[0] = h[v >> 4];
    hex[1] = h[v & 0xF];
    hex[2] = 0;
    UART_Write_Text(hex);
}

void Debug_WriteCmd(const char *name, unsigned char cmd)
{
    UART_Write_Text("CMD ");
    UART_Write_Text(name);
    UART_Write_Text(" = ");
    printHex(cmd);
    UART_Write_Text("\r\n");
    CH375_WriteCmd(cmd);
}

void Debug_WriteData(const char *name, unsigned char data)
{
    UART_Write_Text("CMD ");
    UART_Write_Text(name);
    UART_Write_Text(" = ");
    printHex(data);
    UART_Write_Text("\r\n");
    CH375_WriteData(data);
}


void CH375_ReadKeyboard(void)
{
    Debug_WriteCmd("ISSUE_TOKEN", 0x4A);
    Debug_WriteData("IN 0x81", 0x81);

    Debug_WriteCmd("GET_STATUS", 0x22);
    unsigned char status = CH375_ReadData();

    UART_Write_Text("GET_STATUS returned: 0x");
    printHex(status);
    UART_Write_Text("\r\n");

    if (status == 0x14)
    {
        Debug_WriteCmd("RD_USB_DATA", 0x28);

        unsigned char len = CH375_ReadData();
        UART_Write_Text("DATA LEN = ");
        // UART_Write_Text(len);
        UART_Write_Text(" \r\n");

        unsigned char buf[8];
        for (int i = 0; i < len; i++)
        {
            buf[i] = CH375_ReadData();
            printHex(buf[i]);
        }
        UART_Write_Text("\r\n");

        Parse_HID(buf);
    }
    else
    {
        UART_Write_Text("No key, status=0x");
        printHex(status);
        UART_Write_Text("\r\n");
    }
}

void UART_Write(unsigned char data)  // Output on Terminal
{
    while(!TXSTAbits.TRMT);
    TXREG = data;              //write to TXREG will send data 
}

void UART_Write_Text(char* text) { // Output on Terminal, limit:10 chars
    for(int i=0;text[i]!='\0';i++)
        UART_Write(text[i]);
}

void CH375_WriteCmd(unsigned char cmd)
{
//    // ? 9 bit = 1 ????
//    TXSTAbits.TX9 = 1;
//    TXSTAbits.TX9D = 1;  // ? 9 bit
    TXREG = cmd;
    while(!TXSTAbits.TRMT);
}

void CH375_WriteData(unsigned char data)
{
//    // ? 9 bit = 0 ????
//    TXSTAbits.TX9 = 1;
//    TXSTAbits.TX9D = 0;
    TXREG = data;
    while(!TXSTAbits.TRMT);
}

unsigned char CH375_ReadData(void)
{
    while(!PIR1bits.RCIF); // ???
    return RCREG;
}

char HID_KeycodeToASCII(unsigned char code, unsigned char modifier)
{
    static const char normal[256] = {
        [0x04]='a',[0x05]='b',[0x06]='c',[0x07]='d',
        [0x08]='e',[0x09]='f',[0x0A]='g',[0x0B]='h',
        [0x0C]='i',[0x0D]='j',[0x0E]='k',[0x0F]='l',
        [0x10]='m',[0x11]='n',[0x12]='o',[0x13]='p',
        [0x14]='q',[0x15]='r',[0x16]='s',[0x17]='t',
        [0x18]='u',[0x19]='v',[0x1A]='w',[0x1B]='x',
        [0x1C]='y',[0x1D]='z',
        [0x1E]='1',[0x1F]='2',[0x20]='3',[0x21]='4',
        [0x22]='5',[0x23]='6',[0x24]='7',[0x25]='8',
        [0x26]='9',[0x27]='0',
    };

    char c = normal[code];

    // Shift
    if (modifier & 0x22)  // Left or Right Shift
    {
        if (c >= 'a' && c <= 'z')
            c -= 32;      // to uppercase
    }

    return c;
}

void Parse_HID(unsigned char *buf)
{
    unsigned char modifier = buf[0];
    unsigned char keycode = buf[2];   // ? 3 bytes ? key code

    char c = HID_KeycodeToASCII(keycode, modifier);

    if (c != 0)
        putch(c);     // UART ???
}

void TMR1_Initialize(void)
{
    PIR1bits.TMR1IF = 0;
    PIE1bits.TMR1IE = 1;
    IPR1bits.TMR1IP = 0; 
    
    T1CONbits.RD16 = 1;        // 16-bit R/W
    T1CONbits.TMR1CS = 0;      // Clock = Fosc/4
    T1CONbits.T1CKPS = 0b10;  // Prescaler 1:8
    // 1 tick = 8 µs * ( 4 / Fosc ) @ 4MHz Fosc
    // ?????: 10ms interval (1250 ticks). 65536 - 1250 = 64286 (0xFB1E)

    // *** ??: ???? TMR1H ? TMR1L ??? TMR1 ???? ***
    TMR1H = 0xFB;           
    TMR1L = 0x1E;
    
    T1CONbits.TMR1ON = 1;      // turn on Timer1
}

void UART_Initialize() 
{
    OSCCONbits.IRCF = 0b110; // 4MHz
    OSCCONbits.SCS = 1;      

    TRISCbits.TRISC6 = 0;  // TX output
    TRISCbits.TRISC7 = 1;  // RX input

    // 9600 baud @ 4MHz
    TXSTAbits.SYNC = 0;
    TXSTAbits.BRGH = 0;
    BAUDCONbits.BRG16 = 0;
    SPBRG = 51;    // 9600 for 4MHz (???)

    RCSTAbits.SPEN = 1;
    TXSTAbits.TXEN = 1;
    RCSTAbits.CREN = 1;

    PIR1bits.RCIF = 0;
}

void CH375_InitHost(void)
{
    // printf("Init Host...\r\n");
//
//    Debug_WriteCmd("SET_USB_MODE", 0x15);
//    Debug_WriteData("MODE=05", 0x05);
    LATD = 0xFF;

    while (1) {
        Debug_WriteCmd("TEST_CONNECT", 0x16);
        unsigned char st = CH375_ReadData();
        LATD = 0x01 << 4;

        UART_Write_Text("TEST_CONNECT returned: 0x");
        printHex(st);
        UART_Write_Text("\r\n");

        if (st == 0x15)
        {
            UART_Write_Text("USB device CONNECTED.\r\n");
            break;
        }
        
        __delay_ms(50);
    }

    Debug_WriteCmd("SET_USB_MODE", 0x15);
    Debug_WriteData("MODE=07 reset", 0x07);
    __delay_ms(20);

    Debug_WriteCmd("SET_USB_MODE", 0x15);
    Debug_WriteData("MODE=06 SOF", 0x06);
    __delay_ms(10);

    UART_Write_Text("Init Host done.\r\n");
}
