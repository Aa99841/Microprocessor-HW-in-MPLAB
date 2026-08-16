
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

int even[8] = {0,2,4,6,8,10,12,14};
int odd[8] = {1,3,5,7,9,11,13,15};
int index_even = 0;
int index_odd = 0;
int index_is_even = 0;
int lastValue = 0;

void displayLed(int index);

void __interrupt(high_priority)H_ISR(){
    
    //step4
    int value = ADRESH;    
    
    //do things   
    int change = value - lastValue;
    if(abs(change) > 10) {  // ??????5
        // ????
        if(change > 0){
            displayLed(even[index_even]);
            index_even++;
            if(index_even >= 8){
                index_even = 0;
            }
            index_odd = 0;
        }else{
            displayLed(odd[index_odd]);
            index_odd++;
            if(index_odd >= 8){
                index_odd = 0;
            }
            index_even = 0;
        }
        
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
    //LATD = (LATD & 0x0F) | ((out & 0x0F) << 4);
    LATD = out << 4;
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
    TRISD = 0X0F;
    LATD = LATD & 0x00;
    
    while(1){
    }
    
    return;
}
