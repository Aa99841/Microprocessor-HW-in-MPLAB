
#include <xc.h>
#include <pic18f4520.h>
#include <stdio.h>

#pragma config OSC = INTIO67 // Oscillator Selection bits
#pragma config WDT = OFF     // Watchdog Timer Enable bit
#pragma config PWRT = OFF    // Power-up Enable bit
#pragma config BOREN = ON    // Brown-out Reset Enable bit
#pragma config PBADEN = OFF  // Watchdog Timer Enable bit
#pragma config LVP = OFF     // Low Voltage (single -supply) In-Circute Serial Pragramming Enable bit
#pragma config CPD = OFF     // Data EEPROM?Memory Code Protection bit (Data EEPROM code protection off)

int light = 0;
int lastValue = 0;

void displayLed(int index);

void __interrupt(high_priority)H_ISR(){
    
    //step4
    int value = ADRESH;    
    
    //do things   
    int change = value - lastValue;
    if(abs(change) > 3) {  // ??????5
        // ????
        if(change > 0){
            light += 10;
            if(light > 1020){
                light = 1020;
            }
        }else{
            light -= 10;
            if(light < 0){
                light = 0;
            }
        }
        
        displayLed(light);
//        index++;
//        if(index >= 8){
//            index = 0;
//        }
        
        // ?????ADC?
        lastValue = value;
    }
    
    //clear flag bit
    PIR1bits.ADIF = 0;
    
    
    //step5 & go back step3
    /*
    delay at least 2tad
    */
    ADCON0bits.GO = 1;
    
    return;
}

void displayLed(int out){    
    CCPR2L = (uint8_t)(out >> 2);
    CCP2CONbits.DC2B = (uint8_t)(out & 0x03);
    return;
}

void main(void) 
{
    //configure OSC and port
    OSCCONbits.IRCF = 0b100; //1MHz
    TRISAbits.RA0 = 1;       //analog input port
    
    //step1
    ADCON1bits.VCFG0 = 0;
    ADCON1bits.VCFG1 = 0;
    ADCON1bits.PCFG = 0b1110; //AN0 ?analog input,???? digital
    ADCON0bits.CHS = 0b0000;  //AN0 ?? analog input
    ADCON2bits.ADCS = 0b000;  //????000(1Mhz < 2.86Mhz)
    ADCON2bits.ACQT = 0b001;  //Tad = 2 us acquisition time?2Tad = 4 > 2.4
    ADCON0bits.ADON = 1;
    ADCON2bits.ADFM = 0;    //left justified 
    
    
    //step2
    PIE1bits.ADIE = 1;
    PIR1bits.ADIF = 0;
    INTCONbits.PEIE = 1;
    INTCONbits.GIE = 1;


    //step3
    ADCON0bits.GO = 1;
    
    // inital output 
    // ?????
    OSCCONbits.IRCF = 0b100; // 1MHz
    TRISAbits.RA0 = 1;       // RA0?????
    
    // ??PWM???PORT (CCP2???RC1)
    TRISCbits.RC1 = 0;       // ??RC1???(PWM??)
    
    // ??Timer2 for PWM
    PR2 = 0xFF;              // PWM?? = (PR2+1) * 4 * Tosc * TMR2???
    T2CONbits.T2CKPS = 0b00; // Timer2??? = 1:1
    T2CONbits.TMR2ON = 1;    // ??Timer2
    
    // ??CCP2?PWM??
    CCP2CONbits.CCP2M = 0b1100; // PWM??
    
    while(1){
    }
    
    return;
}
