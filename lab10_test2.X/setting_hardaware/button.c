#include <xc.h>

#define BUTTON_PORT PORTBbits.RB0
#define BUTTON_TRIS TRISBbits.TRISB0

void BUTTON_Initialize(void) {
    BUTTON_TRIS = 1;  // Set RB0 as input for button
    OPTION_REGbits.nRBPU = 0;  // Enable PORTB pull-ups
    WPUBbits.WPUB0 = 1;  // Enable weak pull-up on RB0
}

int IsButtonPressed(void) {
    static int lastState = 1;  // Assuming pull-up, not pressed = 1
    int currentState = BUTTON_PORT;
    int pressed = 0;
    
    if (lastState == 1 && currentState == 0) {  // Detect falling edge
        pressed = 1;
        // Simple debounce delay
        for(int i = 0; i < 1000; i++);
    }
    
    lastState = currentState;
    return pressed;
}