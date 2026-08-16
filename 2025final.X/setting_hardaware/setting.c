// CONFIG1H
#pragma config OSC = INTIO67      // Oscillator Selection bits (HS oscillator)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enable bit (Fail-Safe Clock Monitor disabled)
#pragma config IESO = ON       // Internal/External Oscillator Switchover bit (Oscillator Switchover mode disabled)

// CONFIG2L
#pragma config PWRT = OFF       // Power-up Timer Enable bit (PWRT disabled)
#pragma config BOREN = SBORDIS  // Brown-out Reset Enable bits (Brown-out Reset enabled in hardware only (SBOREN is disabled))
#pragma config BORV = 3         // Brown Out Reset Voltage bits (Minimum setting)

// CONFIG2H
#pragma config WDT = OFF        // Watchdog Timer Enable bit (WDT disabled (control is placed on the SWDTEN bit))
#pragma config WDTPS = 1        // Watchdog Timer Postscale Select bits (1:1)

// CONFIG3H
#pragma config CCP2MX = PORTC   // CCP2 MUX bit (CCP2 input/output is multiplexed with RC1)
// #pragma config PBADEN = ON      // PORTB A/D Enable bit (PORTB<4:0> pins are configured as analog input channels on Reset)
#pragma config PBADEN = OFF
#pragma config LPT1OSC = OFF    // Low-Power Timer1 Oscillator Enable bit (Timer1 configured for higher power operation)
#pragma config MCLRE = ON       // MCLR Pin Enable bit (MCLR pin enabled; RE3 input pin disabled)

// CONFIG4L
#pragma config STVREN = ON      // Stack Full/Underflow Reset Enable bit (Stack full/underflow will cause Reset)
#pragma config LVP = OFF         // Single-Supply ICSP Enable bit (Single-Supply ICSP enabled)
#pragma config XINST = OFF      // Extended Instruction Set Enable bit (Instruction set extension and Indexed Addressing mode disabled (Legacy mode))

// CONFIG5L
#pragma config CP0 = OFF        // Code Protection bit (Block 0 (000800-001FFFh) not code-protected)
#pragma config CP1 = OFF        // Code Protection bit (Block 1 (002000-003FFFh) not code-protected)
#pragma config CP2 = OFF        // Code Protection bit (Block 2 (004000-005FFFh) not code-protected)
#pragma config CP3 = OFF        // Code Protection bit (Block 3 (006000-007FFFh) not code-protected)

// CONFIG5H
#pragma config CPB = OFF        // Boot Block Code Protection bit (Boot block (000000-0007FFh) not code-protected)
#pragma config CPD = OFF        // Data EEPROM Code Protection bit (Data EEPROM not code-protected)

// CONFIG6L
#pragma config WRT0 = OFF       // Write Protection bit (Block 0 (000800-001FFFh) not write-protected)
#pragma config WRT1 = OFF       // Write Protection bit (Block 1 (002000-003FFFh) not write-protected)
#pragma config WRT2 = OFF       // Write Protection bit (Block 2 (004000-005FFFh) not write-protected)
#pragma config WRT3 = OFF       // Write Protection bit (Block 3 (006000-007FFFh) not write-protected)

// CONFIG6H
#pragma config WRTC = OFF       // Configuration Register Write Protection bit (Configuration registers (300000-3000FFh) not write-protected)
#pragma config WRTB = OFF       // Boot Block Write Protection bit (Boot block (000000-0007FFh) not write-protected)
#pragma config WRTD = OFF       // Data EEPROM Write Protection bit (Data EEPROM not write-protected)

// CONFIG7L
#pragma config EBTR0 = OFF      // Table Read Protection bit (Block 0 (000800-001FFFh) not protected from table reads executed in other blocks)
#pragma config EBTR1 = OFF      // Table Read Protection bit (Block 1 (002000-003FFFh) not protected from table reads executed in other blocks)
#pragma config EBTR2 = OFF      // Table Read Protection bit (Block 2 (004000-005FFFh) not protected from table reads executed in other blocks)
#pragma config EBTR3 = OFF      // Table Read Protection bit (Block 3 (006000-007FFFh) not protected from table reads executed in other blocks)

// CONFIG7H
#pragma config EBTRB = OFF      // Boot Block Table Read Protection bit (Boot block (000000-0007FFh) not protected from table reads executed in other blocks)



#include "setting.h"

void OSCILLATOR_Initialize(void)
{    
    OSCCONbits.IRCF = 0b110; // ???? 4MHz
    OSCCONbits.SCS = 1;      // ???????
    // set Fosc  = 125kHz
    // 0b001 = 125kHz (for PIC18F4520)
//    OSCCONbits.IRCF = 0b001; // Fosc
//    OSCCONbits.SCS = 1;     
}


void PIN_MANAGER_Initialize(void) {
    TRISD = 0x0F; 
    LATD = 0x00;
}

/* timer 0
 T0CONbits.T0PS (???) => ?? (Prescaler Ratio)
 0b000 => 1:2
 0b001 => 1:4
 0b010 => 1:8
 0b011 => 1:16
 0b100 => 1:32
 0b101 => 1:64
 0b110 => 1:128
 0b111 => 1:256
 */
void TMR0_Initialize(void)
{
    T0CONbits.T08BIT = 0;     // 16-bit timer
    T0CONbits.T0CS = 0;       // Clock = Fosc/4
    T0CONbits.PSA = 0;        // Enable prescaler
    T0CONbits.T0PS = 0b111;   // Prescaler 1:256
    // 1 tick = 256 µs * ( 4 / Fosc )

    TMR0H = 0xFC;             // preload for 1 sec @ 4MHz
    TMR0L = 0x2F;
    // 65536 - 0xFC2F = 976 ticks
    // 976 * 256 = 249856 µs ~= 250 ms
    
    T0CONbits.TMR0ON = 1;     // turn on Timer0
}

/* timer1
 T1CONbits.T1CKPS (???) => ?? (Prescaler Ratio)
 0b00 => 1:1
 0b01 => 1:2
 0b10 => 1:4
 0b11 => 1:8
 */
void TMR1_Initialize(void)
{
    T1CONbits.RD16 = 1;        // 16-bit R/W
    T1CONbits.TMR1CS = 0;      // Clock = Fosc/4
    T1CONbits.T1CKPS = 0b10;  // Prescaler 1:8
    // 1 tick = 8 µs * ( 4 / Fosc ) @ 4MHz Fosc
    // ?????: 10ms interval (1250 ticks). 65536 - 1250 = 64286 (0xFB1E)

    // *** ??: ???? TMR1H ? TMR1L ??? TMR1 ???? ***
    TMR1H = 0xFB;           
    TMR1L = 0x1E;
    
    T1CONbits.TMR1ON = 1;      // turn on Timer1
}

void TMR2_Initialize(){
    PR2 = 0xFF; // ????
    T2CON = 0x00;
    // Postscaler 1:1, Prescaler 1:16
    T2CON = 0b00000011; // 0x03
    TMR2 = 0x00;
}

//void TMR2_Initialize(){
//    // OSCCONbits.IRCF = 0b110; // ???? 4MHz
//    //    OSCCONbits.SCS = 1;      // ???????
////    T2CONbits.T2CKPS = 0b11;  // Prescaler = 16
////    T2CONbits.TOUTPS3 = 0;
////    T2CONbits.TOUTPS2 = 0;
////    T2CONbits.TOUTPS1 = 0;
////    T2CONbits.TOUTPS0 = 0;
////
////    // PR2 = 61;             // frequence (1ms)
////    
////    PIR1bits.TMR2IF = 0;
////    PIE1bits.TMR2IE = 1;
////    IPR1bits.TMR2IP = 0;
////    T2CONbits.TMR2ON = 1;
//    
//    
//    OSCCONbits.IRCF = 0b001; 
//    OSCCONbits.SCS = 1;      // ?????
//    T2CONbits.T2CKPS = 0b01;  // Prescaler = 4
//    
//    T2CONbits.TOUTPS3 = 0;
//    T2CONbits.TOUTPS2 = 0;
//    T2CONbits.TOUTPS1 = 0;
//    T2CONbits.TOUTPS0 = 0;
//
//    PR2 = 0x26; // PR2 = 38 (??? 19.456 ms)
//    
//    PIR1bits.TMR2IF = 0;
//    PIE1bits.TMR2IE = 1;
//    IPR1bits.TMR2IP = 0;
//    T2CONbits.TMR2ON = 1;
//}

void SYSTEM_Initialize(void)
{
    ADCON1bits.PCFG = 0x0E;
    PIN_MANAGER_Initialize();
    OSCILLATOR_Initialize(); //default 1Mhz
    TMR2_Initialize();
    TMR1_Initialize();
    TMR0_Initialize();
    INTERRUPT_Initialize();
    UART_Initialize();
    CCP1_Initialize();
    CCP2_Initialize();
    ADC_Initialize();
}