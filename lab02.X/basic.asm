;if(address & 2 == 0){
;    [address] = [address - 2] + [address - 1]
;}else{
;    [address] = [address - 2] - [address - 1]
;}
    
List p=18f4520
    #include<p18f4520.inc>
    CONFIG OSC = INTIO67
    CONFIG WDT = OFF
    org 0x00

    ; inital
    MOVLW 0xfe  ;store 0x02 in WREG
    MOVLB 0x1  ;BSR = 1 (change to bank 1)
    MOVWF 0x20,1  ;use BSR xelect bank, [0x120] = 0x02
    
    MOVLW 0xee  ;store 0x03 in WREG
    MOVLB 0x1  ;BSR = 1 (change to bank 1 ?)
    MOVWF 0x21,1  ;use BSR xelect bank, [0x121] = 0x03
    
    MOVLW 0xFF
    MOVWF 0x00
    
    LFSR 0, 0x120    ; FSR0 point to 0x120
    LFSR 1, 0x121    ; FSR1 point to 0x121
    LFSR 2, 0x122    ; FSR2 point to 0x122
    
    ; calutate loop
    LOOP:
	INCF 0X00  ; count++
	MOVLW 0x05
	
	; is loop need end?
	CPFSLT 0X00  ;SKIP IF < 
	    GOTO LAST  ;if ([0x00] > 0x05) goto last
	    
	; the affress is even or odd
	BTFSC 0X00, 0 ; check is even or not
	    GOTO ODD ; [0x00]'s rightest bit is 1
	    
	GOTO EVEN  ; [0x00]'s rightest bit is 0
	
    ODD:
	MOVF POSTINC1, W  ;WREG = FSR0, FSR0 = FSR0 -> NEXT
	SUBWF POSTINC0,W  ;WREG = WREG - FSR1, FSR0 = FSR0 -> NEXT
	MOVWF POSTINC2   ; [FSR2] = WREG
	GOTO LOOP
	
    EVEN:
	MOVF POSTINC0, W  ;WREG = FSR0, FSR0 = FSR0 -> NEXT
	ADDWF POSTINC1,W  ;WREG = FSR1 + WREG, FSR1 = FSR1 -> NEXT
	MOVWF POSTINC2   ; [FSR2] = WREG
	GOTO LOOP
	
    LAST:
	CLRF 0X00
    end
 
