; let sequences A and B be sorted together
; 0x200 -> A
; 0x210 -> B
; [0x00] = count ; [0x10] = A count ; [0x11] = B count
    
List p=18f4520
    #include<p18f4520.inc>
    CONFIG OSC = INTIO67
    CONFIG WDT = OFF
    org 0x00
    
    MOVLB 0x2  ;BSR = 2 
    
    ;Sequence A:
    MOVLW 0x00
    MOVWF 0x00,1
    MOVLW 0x33
    MOVWF 0x01,1
    MOVLW 0x58
    MOVWF 0x02,1
    MOVLW 0x7A
    MOVWF 0x03,1
    MOVLW 0xC4
    MOVWF 0x04,1
    MOVLW 0xF0
    MOVWF 0x05,1
    
    ;Sequence B:
    MOVLW 0x09
    MOVWF 0x10,1
    MOVLW 0x58
    MOVWF 0x11,1
    MOVLW 0x6E
    MOVWF 0x12,1
    MOVLW 0xB8
    MOVWF 0x13,1
    MOVLW 0xDD
    MOVWF 0x14,1
    
    MOVLW 0xFF
    MOVWF 0x00 ;LOOP COUNT
    
    MOVLW 0x06
    MOVWF 0x10  ;A COUNT
    MOVLW 0x05
    MOVWF 0x11  ;B COUNT
    
    LFSR 0, 0x200    ; FSR0 point to 0x200(A)
    LFSR 1, 0x210    ; FSR1 point to 0x210(B)
    LFSR 2, 0x220    ; FSR2 point to 0x200(Result)
    
    LOOP:
	INCF 0X00
	MOVLW 0x0B
	CPFSLT 0X00  ;SKIP IF <
	    GOTO LAST
	    
	MOVF INDF0, W  ;WREG = FSR0
	SUBWF INDF1,W  ;WREG = WREG - FSR1
	BC AFIRST  ;FSR0 <= FSR1 => IF (WREG <= ESR1) carry = 1  
	BNC BFIRST  ;FSR0 > FSR1 => IF (WREG > ESR1) carry = 0
    
    AFIRST:
	MOVF POSTINC0, W  ;WREG = FSR0
	MOVWF POSTINC2, W  ;FRS2 = WREG, FRS2 = FRS2->NEXT
	DECF 0X10
	MOVF 0X10, W
	BZ ALLB ; if (0x10 == 0) goto ALLB
	GOTO LOOP
    
    BFIRST:	
	MOVF POSTINC1, W  ;WREG = FSR1
	MOVWF POSTINC2, W  ;FRS2 = WREG, FRS2 = FRS2->NEXT
	DECF 0X11
	MOVF 0X11, W
	BZ ALLA  ; if (0x11 == 0) goto ALLA
	GOTO LOOP

    ALLA:
	MOVF POSTINC0, W  ;WREG = FSR0
	MOVWF POSTINC2, W  ;FRS2 = WREG, FRS2 = FRS2->NEXT
	DECF 0X10
	MOVF 0X10, W
	BZ LAST
	GOTO ALLA
	
    ALLB:
	MOVF POSTINC1, W  ;WREG = FSR0
	MOVWF POSTINC2, W  ;FRS2 = WREG, FRS2 = FRS2->NEXT
	DECF 0X11
	MOVF 0X11, W
	BZ LAST
	GOTO ALLB
	
    LAST:
	CLRF 0X00
	
    END


