#include <xc.h>
#include <stdlib.h>
#include <math.h>
#include "setting.h"
    //setting TX/RX

char mystring[20];
int lenStr = 0;
int tick;

extern int count;
extern int f;
extern int adcValue;
int lastV = 0;

void UART_Initialize() {
           
//          TODObasic   
//           Serial Setting      
//        1.   Setting Baud rate
//        2.   choose sync/async mode 
//        3.   enable Serial port (configures RX/DT and TX/CK pins as serial port pins)
//        3.5  enable Tx, Rx Interrupt(optional)
//        4.   Enable Tx & RX
    
    TRISCbits.TRISC6 = 1;  // TX
    TRISCbits.TRISC7 = 1;  // RX

    // === Baud rate 9600, Fosc = 4MHz ===
    TXSTAbits.SYNC = 0;   // Async
    BAUDCONbits.BRG16 = 0;
    TXSTAbits.BRGH = 0;   // Low speed
    SPBRG = 51;           // 9600bps (4MHz)

    // === Enable UART ===
    RCSTAbits.SPEN = 1;   // Enable UART

    TXSTAbits.TXEN = 1;   // Enable transmit
    RCSTAbits.CREN = 1;   // Enable continuous receive

    // === Interrupt for RX ===
    PIE1bits.RCIE = 1;    // Enable RX interrupt
    IPR1bits.RCIP = 1;    // RX low priority

    PIR1bits.RCIF = 0;    // Clear flag
    
}

void UART_Write(unsigned char data)  // Output on Terminal
{
    while(!TXSTAbits.TRMT);
    TXREG = data;              //write to TXREG will send data 
}

void UART_DeleteN(int n)
{
    for(int i = 0; i < n; i++)
    {
        UART_Write('\b');   
        UART_Write(' ');   
        UART_Write('\b');  
    }
}

void UART_Write_Text(char* text) { // Output on Terminal, limit:10 chars
    for(int i=0;text[i]!='\0';i++)
        UART_Write(text[i]);
}

void Update(int num){
    UART_DeleteN(4);
    
    char digit;
    if(num > 1000){
        digit = '0' + num / 1000;
        UART_Write(digit);
    }
    if(num > 100){
        digit = '0' + (num % 1000) / 100;
        UART_Write(digit);
    }
    if(num > 10){
        digit = '0' + (num % 100) / 10;
        UART_Write(digit);
    }     
    
    digit = '0' + (num % 10);
    UART_Write(digit);
}

void ClearBuffer(){
    for(int i = 0; i < 10 ; i++)
        mystring[i] = '\0';
    lenStr = 0;
}

void MyusartRead()
{
    // UART_Write('A');
    char c = RCREG;   // ??????

    if (c == '\r') {  // ? Enter
        UART_Write('\n');
        
        float num = atof(mystring);
        num = num / 0.1;
        f = num * 100;
             
        ClearBuffer();
    }
    
    // ??? buffer
    if (lenStr < 10) {
        mystring[lenStr++] = c;
        mystring[lenStr] = '\0';
        UART_Write_Text(mystring);
        
    }
    return ;
}

char *GetString(){
    return mystring;
}

void putch(unsigned char data)
{
    char buf[4];
    int i = 0;

    // ??????? (??? 255 -> 3 ?)
    UART_DeleteN(3);

    // ??????
    if (data >= 100) {
        buf[i++] = '0' + data / 100;          // ??
    }
    if (data >= 10) {
        buf[i++] = '0' + (data / 10) % 10;    // ??
    }
    buf[i++] = '0' + (data % 10);             // ??
    buf[i] = '\0';

    UART_Write_Text(buf);
}


// void interrupt low_priority Lo_ISR(void)
void __interrupt(low_priority)  Lo_ISR(void)
{    
    // timer 2
//    if (PIR1bits.TMR2IF)
//    {
//        PIR1bits.TMR2IF = 0;   // ??????
//        
//        tick++;
//        
//        if(tick >= f){
//            count ++;
//            if(count > 0x0f) count = 0;
//            
//            LATD = count << 4;
//            
//            tick = 0;
//        }
//    }
    
    // adc interrupt
    if(PIR1bits.ADIF){ 
        int value = ((unsigned int)ADRESH << 8) | ADRESL;
       
        if(abs(lastV - value) > 10){
            value = value >> 6;   // 10-bit ? 4-bit
            value = value & 0x0F;
            
            LATD = value << 4;
            lastV = value;
        }
        
        PIR1bits.ADIF = 0;
        ADCON0bits.GO = 1;
    }
   
    return;
}