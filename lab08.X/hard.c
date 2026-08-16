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
void smooth_move(int fro, int des, int d);
void set_servo_angle(int angle);
int clamp(int val, int min, int max);

// ----------- ???? -----------
volatile int state = 0;
volatile int angle = -90;
volatile int direction = 1;

// ----------- ?????? -----------
void __interrupt() ISR(void) {
    // ??
    if(INT0IF){
        INT0IF = 0;
        __delay_ms(50); // ???
        if(PORTBbits.RB0 == 1) return;
        state = !state; // ????
    }

    // Timer1
    if(PIR1bits.TMR1IF){
        PIR1bits.TMR1IF = 0; // ???
        
        unsigned int timer1_val = 65536 - 271;
        TMR1H = timer1_val >> 8;
        TMR1L = timer1_val & 0xFF;

        if(state){ // ?????
            angle += direction*5; // ?? 5 ?
            if(angle >= 90){
                angle = 90;
                direction = -1;
            }else if(angle <= -90){
                angle = -90;
                direction = 1;
            }
            set_servo_angle(angle); // ???? & LED PWM
        }
    }
}

void main(void){
    // ====== ???? ======
    OSCCONbits.IRCF = 0b001;    // ????125kHz
    TRISBbits.TRISB0 = 1;       // RB0 -> ???? (INT0)
    TRISCbits.TRISC2 = 0;       // RC2 -> CCP1 ??
    LATDbits.LATD7 = 0;    // ?????

    // ====== Timer1 ???????? ======
    T1CONbits.TMR1CS = 0;    // Fosc/4
    T1CONbits.T1CKPS = 0b11; // 1:8 prescaler

    unsigned int timer1_val = 65536 - 271;  // 271 ???? 69.4ms
    TMR1H = timer1_val >> 8;
    TMR1L = timer1_val & 0xFF;

    PIR1bits.TMR1IF = 0;
    PIE1bits.TMR1IE = 1;
    T1CONbits.TMR1ON = 1;
    
    // ====== Timer2 + PWM ?? ======
    TRISCbits.TRISC2 = 0;  // CCP1 pin ?? (??)
    TRISCbits.TRISC1 = 0;  // CCP2 pin ?? (LED)
    PR2 = 0x9B;             // PWM period ? 20ms

    // CCP1 PWM (??)
    CCP1CONbits.CCP1M = 0b1100;   // PWM??
    CCPR1L = 0x0B;                 // ?? duty
    CCP1CONbits.DC1B = 0b01;

    // CCP2 PWM (LED)
    CCP2CONbits.CCP2M = 0b1100;   // PWM??
    CCPR2L = 0x00;                 // ?? duty
    CCP2CONbits.DC2B = 0b00;

    // Timer2 ??
    T2CONbits.T2CKPS = 0b01;  // prescaler = 4
    T2CONbits.TMR2ON = 1;     // ?? Timer2



    // ====== ?????? ======
    INT0IF = 0;                 // ?????
    INT0IE = 1;                 // ??INT0??
    INTEDG0 = 0;                // ???????? (??????)
    PEIE = 1;                   // ??????
    GIE = 1;                    // ????????

    CCPR1L = 0x04;
    CCP1CONbits.DC1B = 0b00;
    
    while(1) {
        
    }
}


void smooth_move(int fro, int des, int d) {
    if(d == 1){
        for(int i = fro ; i <= des ; i+=5){
            set_servo_angle(i);
            __delay_ms(50);
        }
    }else{
        for(int i = fro ; i >= des ; i-=5){
            set_servo_angle(i);
            __delay_ms(50);
        }
    }
}

void set_servo_angle(int angle) {
    
    angle = clamp(angle, -90, 90);
    
    // ??????? (?5????)
    static const uint8_t angle_table[37][2] = {
        // ??, CCPR1L, DC1B
        {0x04, 0b00},  // -90°
        {0x04, 0b01},  // -85°
        {0x04, 0b11},  // -80°
        {0x05, 0b01},  // -75°
        {0x05, 0b10},  // -70°
        {0x06, 0b00},  // -65°
        {0x06, 0b10},  // -60°
        {0x06, 0b11},  // -55°
        {0x07, 0b01},  // -50°
        {0x07, 0b10},  // -45°
        {0x08, 0b00},  // -40°
        {0x08, 0b10},  // -35°
        {0x08, 0b11},  // -30°
        {0x09, 0b01},  // -25°
        {0x09, 0b11},  // -20°
        {0x0A, 0b00},  // -15°
        {0x0A, 0b10},  // -10°
        {0x0B, 0b00},  // -5°
        {0x0B, 0b01},  // 0°
        {0x0B, 0b11},  // 5°
        {0x0C, 0b01},  // 10°
        {0x0C, 0b10},  // 15°
        {0x0D, 0b00},  // 20°
        {0x0D, 0b10},  // 25°
        {0x0D, 0b11},  // 30°
        {0x0E, 0b01},  // 35°
        {0x0E, 0b11},  // 40°
        {0x0F, 0b00},  // 45°
        {0x0F, 0b10},  // 50°
        {0x0F, 0b11},  // 55°
        {0x10, 0b01},  // 60°
        {0x10, 0b11},  // 65°
        {0x11, 0b00},  // 70°
        {0x11, 0b10},  // 75°
        {0x12, 0b00},  // 80°
        {0x12, 0b01},  // 85°
        {0x12, 0b11}   // 90°
    };
    
    // ????: (-90 -> 0), (-85 -> 1), ..., (90 -> 36)
    int index = (angle + 90) / 5;
    
    // ????????
    if (index < 0) index = 0;
    if (index > 36) index = 36;
    
    // ?? PWM ???
    CCPR1L = angle_table[index][0];
    CCP1CONbits.DC1B = angle_table[index][1];
    
    unsigned long led_duty = (index * 900) / 36;
//    if(index == 35){
//        led_duty = 994;
//    }else if(index == 34){
//        led_duty = 966;
//    }else if(index == 36){
//        led_duty = 1023;
//    }else if(index == 33){
//        led_duty = 937;
//    }
    //unsigned long led_duty = 966;
    CCPR2L = (uint8_t)(led_duty >> 2);
    CCP2CONbits.DC2B = (uint8_t)(led_duty & 0x03);
    
    __delay_us(50);
}

int clamp(int val, int min, int max) {
    if (val < min) return min;
    if (val > max) return max;
    return val;
}
