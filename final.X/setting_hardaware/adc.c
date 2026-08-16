#include <xc.h>

void ADC_Initialize(void) {
    TRISA = 0xff;		// Set as input port
    ADCON1 = 0x0e;  	// Ref vtg is VDD & Configure pin as analog pin 
    // ADCON2 = 0x92;  	
    ADFM = 1 ;          // Right Justifie
    ADCON2bits.ADCS = 7; // 
    ADRESH=0;  			// Flush ADC output Register
    ADRESL=0;  
    
    ADCON0 = 0b00000001;   // open ADC module, Channel AN0
    ADCON1 = 0b00001110;   // AN0 analog, other Digital(0b00001110)
    ADCON2 = 0b10101010;   // Right justify, 12TAD, Fosc/32

    PIR1bits.ADIF = 0;     // clear interrupt flag
    ADCON0bits.GO = 1;
    
}

int ADC_Read(int channel)
{
    int digital;
    
    ADCON0bits.CHS =  0x00; // Select Channe7
    ADCON0bits.GO = 1;
    ADCON0bits.ADON = 1;
    
    while(ADCON0bits.GO_nDONE==1);

    digital = (ADRESH*256) | (ADRESL);
    return(digital);
}