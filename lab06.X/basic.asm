LIST p=18f4520
#include<p18f4520.inc>

    CONFIG OSC = INTIO67 ; Set internal oscillator to 1 MHz
    CONFIG WDT = OFF     ; Disable Watchdog Timer
    CONFIG LVP = OFF     ; Disable Low Voltage Programming

    L1 EQU 0x14         ; Define L1 memory location
    L2 EQU 0x15         ; Define L2 memory location
    org 0x00            ; Set program start address to 0x00

; instruction frequency = 1 MHz / 4 = 0.25 MHz
; instruction time = 1/0.25 = 4 ?s
; Total_cycles = 2 + (2 + 8 * num1 + 3) * num2 cycles
; num1 = 111, num2 = 70, Total_cycles = 62512 cycles
; Total_delay ~= Total_cycles * instruction time = 0.25 s
DELAY macro num1, num2
    local LOOP1         ; Inner loop
    local LOOP2         ; Outer loop
    
    ; 2 cycles
    MOVLW num2          ; Load num2 into WREG
    MOVWF L2            ; Store WREG value into L2
    
    ; Total_cycles for LOOP2 = 2 cycles
    LOOP2:
    MOVLW num1          
    MOVWF L1  
    
    ; Total_cycles for LOOP1 = 8 cycles
    LOOP1:
    NOP                 ; busy waiting
    NOP
    NOP
    NOP
    NOP
    DECFSZ L1, 1        
    BRA LOOP1           ; BRA instruction spends 2 cycles
    
    ; 3 cycles
    DECFSZ L2, 1        ; Decrement L2, skip if zero
    BRA LOOP2           
endm

start:
int:
; let pin can receive digital signal 
MOVLW 0x0f
MOVWF ADCON1
CLRF PORTB
BSF TRISB, 0      ; RB0 as input
CLRF LATA
BCF TRISA, 0      ; RA0 as output
BCF TRISA, 1      ; RA1 as output  
BCF TRISA, 2      ; RA2 as output
    
MOVLW 0x00
MOVWF 0x00
    
check_process:
   BTFSS PORTB, 0    
   BRA button_pressed 
   BRA check_process
   
button_pressed:
    ;???
    DELAY d'20', d'70'  
    
wait_release:
    BTFSS PORTB, 0  ; WAIT FOR PORTB0 == 1
    BRA wait_release
    
    ; STATUS CHANGE
    INCF 0x00      
    
    MOVLW 0x04
    CPFSLT 0x00
    CLRF 0x00          
    
    
    MOVF 0x00, W
    
    BZ turnoff
    
    SUBLW 0x01
    BZ lightup1
    
    MOVF 0x00, W
    SUBLW 0x02
    BZ lightup2
    
    MOVF 0x00, W
    SUBLW 0x03
    BZ lightup3
    
    BRA check_process
    
lightup1:
    BSF LATA, 0        ; LED0?
    BCF LATA, 1
    BCF LATA, 2
    DELAY d'111', d'70'
    BRA check_process
    
lightup2:
    BCF LATA, 0
    BSF LATA, 1        ; LED1?
    BCF LATA, 2
    DELAY d'111', d'70'
    BRA check_process
    
lightup3:
    BCF LATA, 0
    BCF LATA, 1
    BSF LATA, 2        ; LED2?
    DELAY d'111', d'70'
    BRA check_process
    
turnoff:
    BCF LATA, 0        ; ??LED?
    BCF LATA, 1
    BCF LATA, 2
    DELAY d'111', d'70'
    BRA check_process
    
end