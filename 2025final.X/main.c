#include "setting_hardaware/setting.h"
#include <stdlib.h>
#include "stdio.h"
#include "string.h"
#define _XTAL_FREQ 4000000
// using namespace std;

char str[20];
int state = 0;
int direction = -1;
int angel = 90;

int count = 0;
int tick = 1;
int num = 0;
int lastn = 0;
int a = 0;

int lastV;
uint8_t lighttable[6] = {0b01110000, 0b00110000, 0b00010000, 0b00000000, 0b01000000, 0b01100000};
int index = 0;

void Mode1(){   // Todo : Mode1 
    return ;
}
void Mode2(){   // Todo : Mode2 
    return ;
}
void main(void) 
{
    
    SYSTEM_Initialize() ;
    set_servo_angle(90);
    LATD = 0xF0;
    
    
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

void __interrupt(low_priority)  Lo_ISR(void)
{    
    // timer 2
    if (PIR1bits.TMR2IF)
    {
        PIR1bits.TMR2IF = 0;
//        int q = -90;
//        
//        if (state == 1)  q = -90;
//        else if (state == 2) q = 0;
//        
//        angel += direction * 5;
//
//        if (angel >= 90) {
//            angel = 90;
//            direction = -1;
//        }
//
//        if (angel <= q) {
//            angel = q;
//            direction = 1;
//        }
//        
//        set_servo_angle(angel);
    }
    
    // timer1 
    if (PIR1bits.TMR1IF)
    {
        PIR1bits.TMR1IF = 0;   // clear the flag    
        TMR0H = 0xFB;             // preload for 1 sec @ 4MHz
        TMR0L = 0x1E;   
        
        num = (num + 1) % 16;
        LATD = (num << 4);
//        if(count == tick){
//            num = (num + 1) % 16;
//            LATD = (num << 4);
//            count = 0;
//        }
//        count ++;
    }
    
    // timer0
    if (INTCONbits.TMR0IF)
    {
        INTCONbits.TMR0IF = 0;    // clear the flag
        // preload
//        TMR0H = 0xFC;             
//        TMR0L = 0x2F;       
//        
//        if(count == tick){
//            num ++;
//            LATD = (state << 4) & 0xF0;
//            count = 0;
//        }
//        count ++;
    }
    
    // adc interrupt
    if(PIR1bits.ADIF){ 
        PIR1bits.ADIF = 0;
        
        int v = ((unsigned int)ADRESH << 8) | ADRESL;
//        if(lastV - v > 0 && abs(lastV - v) > 5){
//            index = (index + 1) % 6;
//            lastV = v;
//        }
//        else if(lastV - v < 0 && abs(lastV - v) > 5){
//            index = (index - 1 + 6) % 6;
//            lastV = v;
//        }
//        LATD = lighttable[index];
//        UART_Write_Text("\r"); // ??????????????????
        if(state == 4 && abs(lastV - v) > 10){
            unsigned long limited_range_angle = ( (unsigned long)v * 180UL ) / 1023UL; 
            angel = (int)limited_range_angle;
            angel -= 90;
            set_servo_angle(angel);
            lastV = v;
        }
      // Max Duty = (PR2 + 1) * 4 = 256 * 4 = 1024
      // transfer between a and b
      // led_duty = Min_PWM + (adc_value - Min_ADC) * ((Max_PWM - Min_PWM) / (Max_ADC - Min_ADC))
//        unsigned long led_duty = (v * 1023) / 1023;
        if(state != 4){
             // ccp2 output
            CCPR2L = (uint8_t)(v >> 2);
            CCP2CONbits.DC2B = (uint8_t)(v & 0x03);   
        }
        
        ADCON0bits.GO = 1;
    }
   
    return;
}

void __interrupt(high_priority) Hi_ISR(void)
{    
    //buttom
    if (INTCONbits.INT0IF)
    {
        INTCONbits.INT0IF = 0; 
//        if(state == 1) {state = 2; UART_Write('2');}
//        else { state = 1; UART_Write('1'); }
//        LATD = 0xf0;
//        switch(state){
//            case 1:
//                state = 2;
//                angel += 45 * direction;
//                break;
//            case 2:
//                state = 3;
//                angel += 90 * direction;
//                break;
//            case 3:
//                state = 1;
//                angel += 180 * direction;
//                break;        
//            default:
//                break;
//        }
//        
//        if(angel >= 90){
//            set_servo_angle(90);
//            direction = -1;
//            angel  = 180 - angel;
//        }
//        if(angel <= -90){
//            set_servo_angle(-90);
//            direction = 1;
//            angel  = -180 - angel;
//        }
//        __delay_ms(100);
//        set_servo_angle(angel);
        
    }
    
    //uart
    if(PIR1bits.RCIF)
    {
        PIR1bits.RCIF = 0;
        
        if(RCSTAbits.OERR)
        {
            CREN = 0;
            Nop();
            CREN = 1;
        }
        LATD = 1 << 4;
        MyusartRead();
    }
}