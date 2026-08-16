#include "p18f4520.inc"

; ===== CONFIG ????????????? =====
; CONFIG1H
  CONFIG  OSC = INTIO67         ; Oscillator Selection bits (Internal oscillator block, port function on RA6 and RA7)
  CONFIG  FCMEN = OFF           ; Fail-Safe Clock Monitor Enable bit (Fail-Safe Clock Monitor disabled)
  CONFIG  IESO = OFF            ; Internal/External Oscillator Switchover bit (Oscillator Switchover mode disabled)

; CONFIG2L
  CONFIG  PWRT = OFF            ; Power-up Timer Enable bit (PWRT disabled)
  CONFIG  BOREN = SBORDIS       ; Brown-out Reset Enable bits (Brown-out Reset enabled in hardware only (SBOREN is disabled))
  CONFIG  BORV = 3              ; Brown Out Reset Voltage bits (Minimum setting)

; CONFIG2H
  CONFIG  WDT = OFF             ; Watchdog Timer Enable bit (WDT disabled (control is placed on the SWDTEN bit))
  CONFIG  WDTPS = 32768         ; Watchdog Timer Postscale Select bits (1:32768)

; CONFIG3H
  CONFIG  CCP2MX = PORTC        ; CCP2 MUX bit (CCP2 input/output is multiplexed with RC1)
  CONFIG  PBADEN = ON           ; PORTB A/D Enable bit (PORTB<4:0> pins are configured as analog input channels on Reset)
  CONFIG  LPT1OSC = OFF         ; Low-Power Timer1 Oscillator Enable bit (Timer1 configured for higher power operation)
  CONFIG  MCLRE = ON            ; MCLR Pin Enable bit (MCLR pin enabled; RE3 input pin disabled)

; CONFIG4L
  CONFIG  STVREN = ON           ; Stack Full/Underflow Reset Enable bit (Stack full/underflow will cause Reset)
  CONFIG  LVP = OFF             ; Single-Supply ICSP Enable bit (Single-Supply ICSP disabled)
  CONFIG  XINST = OFF           ; Extended Instruction Set Enable bit (Instruction set extension and Indexed Addressing mode disabled (Legacy mode))

; CONFIG5L
  CONFIG  CP0 = OFF             ; Code Protection bit (Block 0 (000800-001FFFh) not code-protected)
  CONFIG  CP1 = OFF             ; Code Protection bit (Block 1 (002000-003FFFh) not code-protected)
  CONFIG  CP2 = OFF             ; Code Protection bit (Block 2 (004000-005FFFh) not code-protected)
  CONFIG  CP3 = OFF             ; Code Protection bit (Block 3 (006000-007FFFh) not code-protected)

; CONFIG5H
  CONFIG  CPB = OFF             ; Boot Block Code Protection bit (Boot block (000000-0007FFh) not code-protected)
  CONFIG  CPD = OFF             ; Data EEPROM Code Protection bit (Data EEPROM not code-protected)

; CONFIG6L
  CONFIG  WRT0 = OFF            ; Write Protection bit (Block 0 (000800-001FFFh) not write-protected)
  CONFIG  WRT1 = OFF            ; Write Protection bit (Block 1 (002000-003FFFh) not write-protected)
  CONFIG  WRT2 = OFF            ; Write Protection bit (Block 2 (004000-005FFFh) not write-protected)
  CONFIG  WRT3 = OFF            ; Write Protection bit (Block 3 (006000-007FFFh) not write-protected)

; CONFIG6H
  CONFIG  WRTC = OFF            ; Configuration Register Write Protection bit (Configuration registers (300000-3000FFh) not write-protected)
  CONFIG  WRTB = OFF            ; Boot Block Write Protection bit (Boot block (000000-0007FFh) not write-protected)
  CONFIG  WRTD = OFF            ; Data EEPROM Write Protection bit (Data EEPROM not write-protected)

; CONFIG7L
  CONFIG  EBTR0 = OFF           ; Table Read Protection bit (Block 0 (000800-001FFFh) not protected from table reads executed in other blocks)
  CONFIG  EBTR1 = OFF           ; Table Read Protection bit (Block 1 (002000-003FFFh) not protected from table reads executed in other blocks)
  CONFIG  EBTR2 = OFF           ; Table Read Protection bit (Block 2 (004000-005FFFh) not protected from table reads executed in other blocks)
  CONFIG  EBTR3 = OFF           ; Table Read Protection bit (Block 3 (006000-007FFFh) not protected from table reads executed in other blocks)

; CONFIG7H
  CONFIG  EBTRB = OFF           ; Boot Block Table Read Protection bit (Boot block (000000-0007FFh) not protected from table reads executed in other blocks)

L1 EQU 0x14
L2 EQU 0x15

org 0x00	
	
goto Initial

; ======== ???????RB0?========
org 0x08
High_ISR:
    INCF 0X00
    
    MOVLW 0x02
    CPFSLT 0x00   
	CLRF 0X00

    BCF INTCON, INT0IF     ; ????????
    RETFIE FAST            ; ??????????? FAST?

; ======== ???????Timer2?========
org 0x18
Low_ISR:
    MOVF 0X00,W
    SUBLW 0XFF
    BZ BACK
    
    MOVF 0X00,W
    BZ ADD_EVEN
    BNZ ADD_ODD    
    
    ADD_EVEN:
	MOVLW D'61' ;EVEN
	MOVWF PR2
	MOVFF 0X01, 0X50
	RLNCF 0X50
	RLNCF 0X50
	RLNCF 0X50
	RLNCF 0X50
	
	MOVFF 0X50, LATD
	INCF 0X01
	INCF 0X01
	
	MOVLW 0X10
	CPFSEQ 0X01
	    GOTO BACK
	MOVLW 0X00
	MOVWF 0X01
	GOTO BACK
	
    ADD_ODD:
	MOVLW D'244' ;ODD
	MOVWF PR2
	MOVFF 0X02, 0X51
	RLNCF 0X51
	RLNCF 0X51
	RLNCF 0X51
	RLNCF 0X51
	
	MOVFF 0X51, LATD
	INCF 0X02
	INCF 0X02
	
	MOVLW 0X11
	CPFSEQ 0X02
	    GOTO BACK
	MOVLW 0X01
	MOVWF 0X02
	GOTO BACK
    
    BACK:
	BCF PIR1, TMR2IF       ; ?? Timer2 ????
    RETFIE

; delay
Delay500ms:
    MOVLW d'180'
    MOVWF L2
Loop2:
    MOVLW d'350'
    MOVWF L1
Loop1:
    NOP
    NOP
    NOP
    NOP
    NOP
    DECFSZ L1, 1
    BRA Loop1
    DECFSZ L2, 1
    BRA Loop2
    RETURN
    
    
; ======== ??? ========
Initial:
    MOVLW 0x0F
    MOVWF ADCON1           ; ?PORTA??Digital

    CLRF TRISA
    MOVLW 0X0F
    MOVWF TRISD
    CLRF LATD 
    BSF TRISB, 0           ; RB0???????

    ; ?????????
    BSF RCON, IPEN
    ; ???????????????
    BSF INTCON, GIEH
    BSF INTCON, GIEL

    ; === ???? INT0 ?????? ===
    BCF INTCON, INT0IF
    BSF INTCON, INT0IE     ; ?? INT0 ???RB0?
    ; INT0 ???????????INT0IP

    ; === Timer2 ?????? ===
    MOVLW b'11111111'
    MOVWF T2CON            ; Prescale / Postscale = 16
    MOVLW D'122'
    MOVWF PR2              ; ?????
    BSF IPR1, TMR2IP       ; Timer2 ????? = 1 ? ?
    BCF IPR1, TMR2IP       ; ?? 0 ? ?
    BSF PIE1, TMR2IE       ; ?? Timer2 ??
    BCF PIR1, TMR2IF       ; ?? Timer2 ????

    MOVLW b'00100000'
    MOVWF OSCCON            ; ?????? = 250kHz
    BSF T2CON, TMR2ON       ; ?? Timer2
    
    MOVLW 0XFF
    MOVWF 0X00  ;STATE
    
    MOVLW 0X00
    MOVWF 0X01  ;EVEN
    
    MOVLW 0X01
    MOVWF 0X02  ;ODD

MainLoop:
    BRA MainLoop

END



