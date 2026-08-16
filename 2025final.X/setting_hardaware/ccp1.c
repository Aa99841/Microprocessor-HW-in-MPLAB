#include <xc.h>
#include <stdint.h>

//void CCP1_Initialize() {
//    TRISCbits.TRISC2 = 0;   // RC2 = CCP1 output
//    CCP1CON = 0b00001100;   // PWM mode
//    // CCP1CON=9;		// Compare mode, initialize CCP1 pin high, clear output on compare match
//    PIR1bits.CCP1IF=0;
//    IPR1bits.CCP1IP = 1;
//    T3CONbits.T3CCP1 = 1;
//}

void CCP1_Initialize() {
    TRISCbits.TRISC2 = 0;   // RC2 ????
    
    // ???????? 90 ? (D=375)
    // 375 (10-bit value) -> CCPR1L = 0x5D (93), DC1B = 0b11 (3)
    CCPR1L = 0x5D; 
    
    // CCP1CON = DC1B<1:0> (11) + CCP1M<3:0> (1100) -> 0b00111100 (0x3C)
    CCP1CON = 0b00111100;   // 0x3C
    
    PIR1bits.CCP1IF=0;
    IPR1bits.CCP1IP = 1;
    
    // ?? Timer2 
    T2CONbits.TMR2ON = 1;
}

void CCP2_Initialize() {
    TRISCbits.TRISC1=0;	// RC2 pin is output.
    CCP2CON=0b00001100;		// Compare mode, initialize CCP1 pin high, clear output on compare match
    PIR2bits.CCP2IF=0;
    IPR2bits.CCP2IP = 1;
}

int clamp(int val, int min, int max) {
    if (val < min) return min;
    if (val > max) return max;
    return val;
}

// SetCCP1DutyCycle ???????????? 10-bit PWM ????
void SetCCP1DutyCycle(uint16_t duty_value)
{
    if (duty_value > 1023) {
        duty_value = 1023;
    }

    // 1. ? 8 ???? CCPR1L
    CCPR1L = (uint8_t)(duty_value >> 2); 

    // 2. ? 2 ???? CCP1CON ? DC1B<1:0> (Bits 5:4)
    CCP1CON &= 0xCF; // ?? DC1B<1:0> 
    CCP1CON |= (uint8_t)((duty_value & 0x0003) << 4); // ?? D[1:0]
}

/**
 * @brief ??? (0-180) ??? 10 ?? PWM ???? (D)
 * * ?? Fosc=4MHz, TMR2 Prescaler=1:16, PR2=255 -> T_PWM = 4.096 ms
 * D ??: 250 (1ms) ? 500 (2ms)
 * @param angle ?????? (0 ? 180)
 * @return uint16_t 10-bit Duty Cycle value (250-500)
 */
uint16_t AngleToDutyCycle(uint8_t angle)
{
    if (angle > 180) {
        angle = 180;
    }
    
    // ??????? D_min = 125 (0.5ms)
    // ??????? D_max = 625 (2.5ms)
    // ?????????: 625 - 125 = 500
    // ????: D = 125 + angle * (500 / 180)
    // ????: 500 / 180 = 25 / 9
    
    // ?? 32 ????????????????
    // D = 125 + angle * (25 / 9)
    uint32_t D_value = 125UL + ( (uint32_t)angle * 25UL / 9UL ); 

    // ?? D_value ? 125 ? 625 ??
    const uint16_t D_MIN = 125;
    const uint16_t D_MAX = 625;

    if (D_value > D_MAX) D_value = D_MAX; 
    if (D_value < D_MIN) D_value = D_MIN;
    
    return (uint16_t)D_value;
}

void set_servo_angle(int angle) {
    
    // 1. ??????? -90 ? 90 
    if (angle < -90) angle = -90;
    if (angle > 90) angle = 90;
    
    // 2. ??? 0 ? 180 ????
    int angle_0_180 = angle + 90;

    // 3. ??? 10 ?????? (250-500)
    uint16_t duty_val = AngleToDutyCycle((uint8_t)angle_0_180);
    
    // 4. ?? CCP1 ???
    SetCCP1DutyCycle(duty_val);
}

//void set_servo_angle(int angle) {
//    
//    angle = clamp(angle, -90, 90);
//    
//    // 125k
//    static const uint8_t angle_table[37][2] = {
//        // CCPR1L, DC1B
//        {0x04, 0b00},  // -90°
//        {0x04, 0b01},  // -85°
//        {0x04, 0b11},  // -80°
//        {0x05, 0b01},  // -75°
//        {0x05, 0b10},  // -70°
//        {0x06, 0b00},  // -65°
//        {0x06, 0b10},  // -60°
//        {0x06, 0b11},  // -55°
//        {0x07, 0b01},  // -50°
//        {0x07, 0b10},  // -45°
//        {0x08, 0b00},  // -40°
//        {0x08, 0b10},  // -35°
//        {0x08, 0b11},  // -30°
//        {0x09, 0b01},  // -25°
//        {0x09, 0b11},  // -20°
//        {0x0A, 0b00},  // -15°
//        {0x0A, 0b10},  // -10°
//        {0x0B, 0b00},  // -5°
//        {0x0B, 0b01},  // 0°
//        {0x0B, 0b11},  // 5°
//        {0x0C, 0b01},  // 10°
//        {0x0C, 0b10},  // 15°
//        {0x0D, 0b00},  // 20°
//        {0x0D, 0b10},  // 25°
//        {0x0D, 0b11},  // 30°
//        {0x0E, 0b01},  // 35°
//        {0x0E, 0b11},  // 40°
//        {0x0F, 0b00},  // 45°
//        {0x0F, 0b10},  // 50°
//        {0x0F, 0b11},  // 55°
//        {0x10, 0b01},  // 60°
//        {0x10, 0b11},  // 65°
//        {0x11, 0b00},  // 70°
//        {0x11, 0b10},  // 75°
//        {0x12, 0b00},  // 80°
//        {0x12, 0b01},  // 85°
//        {0x12, 0b11}   // 90°
//    };
//    
//    int index = (angle + 90) / 5;
//    
//    if (index < 0) index = 0;
//    if (index > 36) index = 36;
//    
//    // PWM
//    CCPR1L = angle_table[index][0];
//    CCP1CONbits.DC1B = angle_table[index][1];
//    
//      // Max Duty = (255 + 1) * 4 = 256 * 4 = 1024
//      // transfer between a and b
//      // led_duty = Min_PWM + (adc_value - Min_ADC) * ((Max_PWM - Min_PWM) / (Max_ADC - Min_ADC))
////    unsigned long led_duty = (index * 900) / 36;
////
////    // ccp2 output
////    CCPR2L = (uint8_t)(led_duty >> 2);
////    CCP2CONbits.DC2B = (uint8_t)(led_duty & 0x03);
//}