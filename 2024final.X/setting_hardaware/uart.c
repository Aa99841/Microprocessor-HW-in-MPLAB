#include <xc.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <ctype.h>
#include "setting.h"
    //setting TX/RX

char mystring[20];
int lenStr = 0;

extern int angel;
extern int state;


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

void UART_PrintHex(unsigned char c)
{
    const char hex[] = "0123456789ABCDEF";
    UART_Write(hex[(c >> 4) & 0x0F]);  // high nibble
    UART_Write(hex[c & 0x0F]);         // low nibble
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

void UART_DeleteLine(int n){
    UART_Write('\r');
    for(int i = 0; i < n; i++)
    { 
        UART_Write(' ');  
    }
    UART_Write('\r');
}

void UART_Write_Text(char* text) { // Output on Terminal, limit:10 chars
    for(int i=0;text[i]!='\0';i++)
        UART_Write(text[i]);
}

void Update(int num){
    UART_DeleteLine(3);
    
    if(num < 0){
        UART_Write('-');
        num = num * -1;
    }
    
    char digit;
    if(num > 10000){
        digit = '0' + num / 10000;
        UART_Write(digit);
    }
    if(num > 1000){
        digit = '0' + (num % 10000) / 1000;
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
    char c = RCREG;
    UART_Write(c);

    if (c == '\r') {  // Enter
        UART_Write('\n');
        
        int len = strlen(mystring);
        // deal with the first char which is not what we need
//        if(!isalpha(mystring[0]) && state == 0){
//            memcpy(mystring, mystring + 1, len - 1);
//            mystring[len - 1] = '\0';
//        }
        if(state == 0){
            if(strcmp(mystring,"m1") == 0) state = 4;
            else if(strcmp(mystring,"m2") == 0) state = 5;
        }
        else if(state == 4){
            state = 0;
        }
//        if(state == 4 || state == 5){
//            if(mystring[0] == 'e' && len < 3) state = 0;
//        }
        else if(state == 5){
            int num = atof(mystring);
            if(num > 90 || num < -90){
                UART_Write_Text("error");
                state = 0;
                set_servo_angle(0);
            }
            angel = num;
            set_servo_angle(angel);
            state = 0;
        }
        ClearBuffer();
        LATD = state << 4;
    }
    else if(c == 0x7F){
        lenStr --;
        mystring[lenStr] = '\0';
    }
    // input buffer
    else if (lenStr < 18) {
        mystring[lenStr++] = c;
        mystring[lenStr] = '\0';
//        UART_Write_Text(mystring);
    }
    return ;
}

char *GetString(){
    return mystring;
}