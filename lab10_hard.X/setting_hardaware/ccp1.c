#include <xc.h>

void CCP1_Initialize() {
    TRISCbits.TRISC2=0;	// RC2 pin is output.
    CCP1CON=9;		// Compare mode, initialize CCP1 pin high, clear output on compare match
    PIR1bits.CCP1IF=0;
    IPR1bits.CCP1IP = 1;
}

void CCP2_Initialize() {
    TRISCbits.TRISC2=0;	// RC2 pin is output.
    CCP2CON=9;		// Compare mode, initialize CCP1 pin high, clear output on compare match
    PIR1bits.CCP1IF=0;
    IPR1bits.CCP1IP = 1;
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
    
//    unsigned long led_duty = (index * 900) / 36;
//
//    //unsigned long led_duty = 966;
//    CCPR2L = (uint8_t)(led_duty >> 2);
//    CCP2CONbits.DC2B = (uint8_t)(led_duty & 0x03);
    
    __delay_us(50);
}

int clamp(int val, int min, int max) {
    if (val < min) return min;
    if (val > max) return max;
    return val;
}