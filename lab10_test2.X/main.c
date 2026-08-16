#include "setting_hardaware/setting.h"
#include <stdlib.h>
#include "stdio.h"
#include "string.h"
#define _XTAL_FREQ 4000000
// using namespace std;

char str[20];
int count = 0;
int f = 1000;

void Mode1(){   // Todo : Mode1 
    return ;
}
void Mode2(){   // Todo : Mode2 
    return ;
}
void main(void) 
{
    
    SYSTEM_Initialize() ;
    UART_Write('0');
    
    while(1) {
        strcpy(str, GetString()); // TODO : GetString() in uart.c
        if(str[0]=='m' && str[1]=='1'){ // Mode1
            Mode1();
            ClearBuffer();
        }
        else if(str[0]=='m' && str[1]=='2'){ // Mode2
            Mode2();
            ClearBuffer();  
        }
    }
    return;
}

void __interrupt(high_priority) Hi_ISR(void)
{    
    if (INTCONbits.INT0IF)
    {
        __delay_ms(20);
        count ++;
        if(count > 999) count = 0;
        Update(count);        
        
        int ledShow = count % 16;
        LATD = (ledShow & 0x0F) << 4;

        INTCONbits.INT0IF = 0; 
    }
    
    if(PIR1bits.RCIF)
    {
        PIR1bits.RCIF = 0;
        if(RCSTAbits.OERR)
        {
            CREN = 0;
            Nop();
            CREN = 1;
        }
        
        MyusartRead();
    }
}