// PIC18F4520 Configuration Bit Settings
#pragma config OSC = INTIO67
#pragma config PWRT = OFF
#pragma config BOREN = ON
#pragma config WDT = OFF
#pragma config PBADEN = OFF
#pragma config LVP = OFF
#pragma config CPD = OFF

#include <xc.h>
#include <pic18f4520.h>

#define _XTAL_FREQ 125000   // ?????? 125kHz ? ?? __delay_ms()
#define TOSC_US     8
#define TMR2_PRES   4
#define PULSE_CENTER_US 1450U
#define PULSE_HALFSPAN_US 950U

// ----------- ???? -----------
void set_servo_angle(int angle);
int clamp(int val, int min, int max);

// ----------- ???? -----------
volatile int state = 0;

// ----------- ?????? -----------
void __interrupt() ISR(void) {
    if (INT0IF) {         // ????? INT0 ??
        INT0IF = 0;       // ??????
        
        // ?????
        __delay_ms(50);
        if (PORTBbits.RB0 == 1) return; // ??????

        if (state == 0) {
            CCPR1L = 0x12;
            CCP1CONbits.DC1B = 0b11;
            state = 1;
        } else if (state == 1) {
            CCPR1L = 0x0B;
            CCP1CONbits.DC1B = 0b01;
            state = 2;
        } else if (state == 2) {
            CCPR1L = 0x04;
            CCP1CONbits.DC1B = 0b00;
            state = 3;
        } else {
            CCPR1L = 0x0B;
            CCP1CONbits.DC1B = 0b01;
            state = 0;
        }
    }
}

void main(void){
    // ====== ???? ======
    OSCCONbits.IRCF = 0b001;    // ????125kHz
    TRISBbits.TRISB0 = 1;       // RB0 -> ???? (INT0)
    TRISCbits.TRISC2 = 0;       // RC2 -> CCP1 ??
    LATCbits.LATC2 = 0;

    // ====== Timer2 + PWM ======
    T2CONbits.T2CKPS = 0b01;    // prescaler = 4
    PR2 = 0x9B;                 // PWM period ?20ms
    CCP1CONbits.CCP1M = 0b1100; // PWM??
    T2CONbits.TMR2ON = 1;       // ??Timer2

    CCPR1L = 0x0B;
    CCP1CONbits.DC1B = 0b01;         // ?????????

    // ====== ?????? ======
    INT0IF = 0;                 // ?????
    INT0IE = 1;                 // ??INT0??
    INTEDG0 = 0;                // ???????? (??????)
    GIE = 1;                    // ??????
    PEIE = 1;                   // ??????????????

    while(1) {
        // ?????????????
    }
}


int clamp(int val, int min, int max) {
    if (val < min) return min;
    if (val > max) return max;
    return val;
}
