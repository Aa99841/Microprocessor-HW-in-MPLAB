#include <xc.h>

#include "test_uart.h"

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

void main(void) {
    char c;
    int count = 0;
    
    test_uart_init();
    while (c = test_uart_read()) {
        if (c == '\r'){
            test_uart_write('\n');
            continue;
        }
        
        if(count >= 3){
            UART_DeleteN(5);
            count = 0;
        }
        
        UART_Write(c);
        count ++;
    }
    
    return;
}
