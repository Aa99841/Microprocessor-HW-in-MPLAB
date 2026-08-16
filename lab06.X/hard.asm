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
    BSF TRISB, 0
    CLRF LATA
    CLRF TRISA

    MOVLW 0X00
    MOVWF 0X00
    
; ckeck button
check_process:
    BTFSS PORTB, 0
    BRA wait_release
    BRA check_process
    
wait_release:
    ;???
    DELAY d'111', d'35' 
    BTFSS PORTB, 0  ; WAIT FOR PORTB0 == 1
    BRA wait_release
    
    INCF 0x00
    MOVLW 0x03
    CPFSLT 0x00         ; ?? mode < 2???
	CLRF 0x00           ; ?????0
    
    BRA check_mode
    
check_mode:
    MOVF 0x00, W
    
    MOVLW 0x00
    CPFSEQ 0x00
	BRA check_mode1
    BRA lightup0
    
check_mode1:
    MOVLW 0x01
    CPFSEQ 0x00
	BRA check_mode2
    BRA lightup1
    
check_mode2:
    MOVLW 0x02
    CPFSEQ 0x00
	BRA check_process  
    BRA lightup2
    
lightup0:    
    MOVLW B'00000000'   ; ??LED?
    MOVWF LATA
    BRA check_process
    
lightup1:
    BTFSS PORTB, 0
    BRA wait_release
    
    MOVLW B'00000001'   
    MOVWF LATA
    DELAY d'111', d'70' ;delay 0.5s
    
    BTFSS PORTB, 0
    BRA wait_release
    DELAY d'111', d'70'
    
    BTFSS PORTB, 0
    BRA wait_release
    
    MOVLW B'00000010'  
    MOVWF LATA
    DELAY d'111', d'70' ;delay 0.5s
    
    BTFSS PORTB, 0
    BRA wait_release
    DELAY d'111', d'70'
    
    BTFSS PORTB, 0
    BRA wait_release
   
    MOVLW B'00000100'  
    MOVWF LATA
    DELAY d'111', d'70' ;delay 0.5s
    
    BTFSS PORTB, 0
    BRA wait_release
    DELAY d'111', d'70'
    
    BTFSS PORTB, 0
    BRA wait_release
    
    BRA lightup1

lightup2:
    BTFSS PORTB, 0
    BRA wait_release
    
    MOVLW B'00000001'
    MOVWF LATA
    DELAY d'111', d'70' ;delay 1s
    
    BTFSS PORTB, 0
    BRA wait_release
    DELAY d'111', d'70'
    
    BTFSS PORTB, 0
    BRA wait_release
    DELAY d'111', d'70'
    
    BTFSS PORTB, 0
    BRA wait_release
    DELAY d'111', d'70'
    
    BTFSS PORTB, 0
    BRA wait_release
    
    MOVLW B'00000011' 
    MOVWF LATA
    DELAY d'111', d'70' ;delay 1s
    
    BTFSS PORTB, 0
    BRA wait_release
    DELAY d'111', d'70'
    
    BTFSS PORTB, 0
    BRA wait_release
    DELAY d'111', d'70' 
    
    BTFSS PORTB, 0
    BRA wait_release
    DELAY d'111', d'70'
    
    BTFSS PORTB, 0
    BRA wait_release
   
    MOVLW B'00000100' 
    MOVWF LATA
    
    BTFSS PORTB, 0
    BRA wait_release
    
    ;FLASH
    DELAY d'111', d'70' ;delay 0.5s
    BTFSS PORTB, 0
    BRA wait_release
    DELAY d'111', d'70'
    
    MOVLW B'00000000' 
    MOVWF LATA
    
    BTFSS PORTB, 0
    BRA wait_release
    
    DELAY d'111', d'70' ;delay 1s
    
    BTFSS PORTB, 0
    BRA wait_release
    DELAY d'111', d'70'
    
    BTFSS PORTB, 0
    BRA wait_release
    DELAY d'111', d'70'
    
    BTFSS PORTB, 0
    BRA wait_release
    DELAY d'111', d'70'
    
    MOVLW B'00000100'
    MOVWF LATA
    
    BTFSS PORTB, 0
    BRA wait_release
    
    DELAY d'111', d'70' ;delay 0.5s
    BTFSS PORTB, 0
    BRA wait_release
    DELAY d'111', d'70'
    
    MOVLW B'00000000' 
    MOVWF LATA
    
    BTFSS PORTB, 0
    BRA wait_release
    
    DELAY d'111', d'70' ;delay 1s
    
    BTFSS PORTB, 0
    BRA wait_release
    DELAY d'111', d'70'
    
    BTFSS PORTB, 0
    BRA wait_release
    DELAY d'111', d'70'
    
    BTFSS PORTB, 0
    BRA wait_release
    DELAY d'111', d'70'
    
    MOVLW B'00000100' 
    MOVWF LATA
    
    BTFSS PORTB, 0
    BRA wait_release
    
    DELAY d'111', d'70' ;delay 0.5s
    
    BTFSS PORTB, 0
    BRA wait_release
    DELAY d'111', d'70'
    
    BTFSS PORTB, 0
    BRA wait_release
    
    BRA lightup2
    
    
end
    
