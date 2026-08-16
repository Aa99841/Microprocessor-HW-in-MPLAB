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

// ----------- ???? -----------
volatile int angle = -90;
volatile int direction = 1;

// ----------- ?????? -----------
void __interrupt() ISR(void) {
    if (INT0IF) {         // ????? INT0 ??
        INT0IF = 0;       // ??????
        
        // ?????
        __delay_ms(50);
        if (PORTBbits.RB0 == 1) return; // ??????
        // -90 -> 45 -> 0 -> -45 -> 90 -> -45 -> 0 -> 45 -> -90
        if(direction == 1){
            angle += 135;
            if(angle > 90){
                LATDbits.LATD7 = 1;
                set_servo_angle(90);   
                __delay_ms(200);
                angle = 90 - (angle - 90);
                direction = -1;
            }
            set_servo_angle(angle);
        }else if(direction == -1){
            angle -= 135;
            if(angle < -90){
                LATDbits.LATD7 = 0;
                set_servo_angle(-90); 
                __delay_ms(200);
                angle = -90 - (angle + 90);
                direction = 1;
            }
            set_servo_angle(angle);
        }
        
    }
}

void main(void){
    // ====== ???? ======
    OSCCONbits.IRCF = 0b001;    // ????125kHz
    TRISBbits.TRISB0 = 1;       // RB0 -> ???? (INT0)
    TRISCbits.TRISC2 = 0;       // RC2 -> CCP1 ??
    LATDbits.LATD7 = 0;
    TRISDbits.TRISD7 = 0;

    // ====== Timer2 + PWM ======
    T2CONbits.T2CKPS = 0b01;    // prescaler = 4
    PR2 = 0x9B;                 // PWM period ?20ms
    CCP1CONbits.CCP1M = 0b1100; // PWM??
    T2CONbits.TMR2ON = 1;       // ??Timer2

    CCPR1L = 0x04;
    CCP1CONbits.DC1B = 0b00;         // ?????????

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

void set_servo_angle(int angle){
    // ??????????????
    switch(angle) {
        case 90:
            CCPR1L = 0x12;
            CCP1CONbits.DC1B = 0b11;
            break;
        case 45:
            CCPR1L = 0x0F;
            CCP1CONbits.DC1B = 0b00;
            break;
        case 0:
            CCPR1L = 0x0B;
            CCP1CONbits.DC1B = 0b01;
            break;
        case -45:
            CCPR1L = 0x7;
            CCP1CONbits.DC1B = 0b10;
            break;
        case -90:
            CCPR1L = 0x04;
            CCP1CONbits.DC1B = 0b00;
            break;
        default:
            CCPR1L = 0x0B;
            CCP1CONbits.DC1B = 0b01;
            break;
    }
}

int clamp(int val, int min, int max) {
    if (val < min) return min;
    if (val > max) return max;
    return val;
}
