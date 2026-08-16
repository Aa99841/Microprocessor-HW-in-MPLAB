#include <xc.h>

void INTERRUPT_Initialize (void)
{
    RCONbits.IPEN = 1;      //enable Interrupt Priority mode
    INTCONbits.GIEH = 1;    //enable high priority interrupt
    INTCONbits.GIEL = 1;     //disable low priority interrupt
    
    // buttom
    INTCONbits.INT0IE = 1;   // high INT0
    INTCON2bits.INTEDG0 = 0; // ???
    INTCONbits.INT0IF = 0;   // ????
    
    // ---- ADC interrupt ----
    PIR1bits.ADIF = 0;      // clear ADC interrupt flag
    PIE1bits.ADIE = 1;      // Enable ADC interrupt
    IPR1bits.ADIP = 0;      // set Low priority (or 1 = High)
    
    // ---- Timer0 interrupt ----
    INTCONbits.TMR0IF = 0;
    INTCONbits.TMR0IE = 1;
    INTCON2bits.TMR0IP = 0;   // low priority

    // ---- Timer1 interrupt ----
    PIR1bits.TMR1IF = 0;
    PIE1bits.TMR1IE = 1;
    IPR1bits.TMR1IP = 0;      // low priority

    // ---- Timer2 interrupt ----
    PIR1bits.TMR2IF = 0;
    PIE1bits.TMR2IE = 1;
    IPR1bits.TMR2IP = 0;      // low priority    
}
