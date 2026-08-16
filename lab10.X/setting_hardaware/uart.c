#include <xc.h>
#include "setting.h"
    //setting TX/RX

char mystring[20];
int lenStr = 0;

extern int count;

void UART_Initialize() {
           
//          TODObasic   
//           Serial Setting      
//        1.   Setting Baud rate
//        2.   choose sync/async mode 
//        3.   enable Serial port (configures RX/DT and TX/CK pins as serial port pins)
//        3.5  enable Tx, Rx Interrupt(optional)
//        4.   Enable Tx & RX
    
    TRISCbits.TRISC6 = 1; // TX/RC6 ?????
    TRISCbits.TRISC7 = 1; // RX/RC7 ?????
    
    // 2. Setting baud rate: 9600 bps @ 4MHz Fosc
    TXSTAbits.SYNC = 0;    // Asynchronous mode
    BAUDCONbits.BRG16 = 0; // 8-bit Baud Rate Generator
    TXSTAbits.BRGH = 0;    // High Speed
    SPBRG = 51;            // SPBRG ??

    // 3. Serial enable (???? TX/RX enable)
    RCSTAbits.SPEN = 1;    // Enable Serial Port (?? RC6/RC7 ? UART ??)              
    
    // 4. Enable Tx 
    TXSTAbits.TXEN = 1;    // Enable Transmitter 
    
    // ?? RX ?????
    RCSTAbits.CREN = 0; 
    PIE1bits.RCIE = 0;
    
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
        UART_Write('\b');   // ?????
        UART_Write(' ');    // ???
        UART_Write('\b');   // ????
    }
}

void UART_Write_Text(char* text) { // Output on Terminal, limit:10 chars
    for(int i=0;text[i]!='\0';i++)
        UART_Write(text[i]);
}

void Update(int num){
    UART_DeleteN(3);
    
    char digit;
    
    digit = '0' + num / 100;
    UART_Write(digit);
        
    digit = '0' + (num % 100) / 10;
    UART_Write(digit);

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
    char c = RCREG;   // ??????

    if (c == '\r') {  // ? Enter
        UART_Write('\n');
    }
    
    // ??? buffer
    if (lenStr < 10) {
        mystring[lenStr++] = c;
        mystring[lenStr] = '\0';
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
    if(RCIF)
    {
        if(RCSTAbits.OERR)
        {
            CREN = 0;
            Nop();
            CREN = 1;
        }
        
        MyusartRead();
    }
    
    
   
    return;
}