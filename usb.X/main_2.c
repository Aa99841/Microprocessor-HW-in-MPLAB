#include <xc.h>
#include <stdint.h>

#pragma config OSC = INTIO67
#pragma config WDT = OFF
#pragma config LVP = OFF
#pragma config PBADEN = OFF

#define _XTAL_FREQ 4000000

// ------------------------- CH375 Parallel Pins -------------------------
#define CH375_DATA      PORTD      // D0~D7
#define CH375_DATA_DIR  TRISD

#define CH375_A0        PORTBbits.RB0
#define CH375_RD        PORTBbits.RB1
#define CH375_WR        PORTBbits.RB2
#define CH375_CS        PORTBbits.RB3
#define CH375_RST       PORTBbits.RB4
#define CH375_INT       PORTBbits.RB5

#define CH375_A0_DIR    TRISBbits.TRISB0
#define CH375_RD_DIR    TRISBbits.TRISB1
#define CH375_WR_DIR    TRISBbits.TRISB2
#define CH375_CS_DIR    TRISBbits.TRISB3
#define CH375_RST_DIR   TRISBbits.TRISB4
#define CH375_INT_DIR   TRISBbits.TRISB5

// -----------------------------------------------------------------------

void UART_Init(void);
void UART_Write(char c);
void UART_Write_Text(const char *s);

void CH375_WriteCmd(uint8_t cmd);
void CH375_WriteData(uint8_t d);
uint8_t CH375_ReadData(void);

void CH375_Reset(void);
void CH375_SetMode(uint8_t mode);
uint8_t CH375_TestConnect(void);

void USB_Init(void);
void USB_Task(void);

char HID_KeycodeToASCII(uint8_t code, uint8_t modifier);

// ======================================================================
//                          Main Program
// ======================================================================

void main(void)
{
    OSCCON = 0b01100010;   // Internal 4MHz

    UART_Init();

    UART_Write_Text("CH375 Parallel Keyboard Test\r\n");

    USB_Init();

    while(1)
    {
        USB_Task();
    }
}

// ======================================================================
//                           UART Functions
// ======================================================================

void UART_Init(void)
{
    TRISC6 = 0;   // TX output
    TRISC7 = 1;   // RX input

    TXSTA = 0b00100000;
    RCSTA = 0b10010000;
    SPBRG = 25;   // 9600 @ 4MHz
}

void UART_Write(char c)
{
    while(!TXIF);
    TXREG = c;
}

void UART_Write_Text(const char *s)
{
    while(*s)
    {
        UART_Write(*s++);
    }
}

// ======================================================================
//                      CH375 Parallel Interface
// ======================================================================

void CH375_Reset(void)
{
    CH375_RST = 0;
    __delay_ms(5);
    CH375_RST = 1;
    __delay_ms(20);
}

void CH375_WriteCmd(uint8_t cmd)
{
    CH375_DATA_DIR = 0x00; // output
    CH375_A0 = 1;
    CH375_RD = 1;
    CH375_CS = 0;

    CH375_DATA = cmd;

    CH375_WR = 0;
    __delay_us(2);
    CH375_WR = 1;

    CH375_CS = 1;
}

void CH375_WriteData(uint8_t d)
{
    CH375_DATA_DIR = 0x00; // output
    CH375_A0 = 0;
    CH375_RD = 1;
    CH375_CS = 0;

    CH375_DATA = d;

    CH375_WR = 0;
    __delay_us(2);
    CH375_WR = 1;

    CH375_CS = 1;
}

uint8_t CH375_ReadData(void)
{
    uint8_t v;

    CH375_DATA_DIR = 0xFF; // input
    CH375_A0 = 0;
    CH375_WR = 1;
    CH375_CS = 0;

    CH375_RD = 0;
    __delay_us(2);
    v = CH375_DATA;
    CH375_RD = 1;

    CH375_CS = 1;

    return v;
}

void CH375_SetMode(uint8_t mode)
{
    CH375_WriteCmd(0x15);
    CH375_WriteData(mode);
    __delay_ms(10);
}

uint8_t CH375_TestConnect(void)
{
    CH375_WriteCmd(0x16);
    __delay_ms(2);
    return CH375_ReadData();
}

// ======================================================================
//                       USB Keyboard Handling
// ======================================================================

void USB_Init(void)
{
    CH375_Reset();

    UART_Write_Text("Setting mode 05...\r\n");
    CH375_SetMode(0x05);

    while(1)
    {
        uint8_t st = CH375_TestConnect();
        UART_Write_Text("TEST_CONNECT = ");
        UART_Write((st>>4)+'0');
        UART_Write((st&0x0F)+'0');
        UART_Write_Text("\r\n");

        if(st == 0x15)
        {
            UART_Write_Text("USB Device Connected!\r\n");
            break;
        }
        __delay_ms(200);
    }

    UART_Write_Text("Reset bus...\r\n");
    CH375_SetMode(0x07);
    __delay_ms(20);

    UART_Write_Text("Start SOF...\r\n");
    CH375_SetMode(0x06);
    __delay_ms(10);

    UART_Write_Text("USB Host Ready.\r\n");
}

void USB_Task(void)
{
    uint8_t status;

    CH375_WriteCmd(0x4A);     // ISSUE TOKEN (IN)
    CH375_WriteData(0x81);

    CH375_WriteCmd(0x22);     // GET_STATUS
    status = CH375_ReadData();

    if(status == 0x14)
    {
        CH375_WriteCmd(0x28); // RD_USB_DATA
        uint8_t len = CH375_ReadData();

        uint8_t buf[8];
        for(uint8_t i=0;i<len;i++)
            buf[i] = CH375_ReadData();

        char c = HID_KeycodeToASCII(buf[2], buf[0]);
        if(c)
            UART_Write(c);
    }
}

// ======================================================================
//                           HID ASCII Decode
// ======================================================================

char HID_KeycodeToASCII(uint8_t code, uint8_t modifier)
{
    static const char map[256] = {
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

    char c = map[code];

    if(modifier & 0x22) // shift
    {
        if(c>='a' && c<='z')
            c -= 32;
    }

    return c;
}
