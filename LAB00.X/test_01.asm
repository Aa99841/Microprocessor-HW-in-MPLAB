List p=18f4520
    #include<p18f4520.inc>
        CONFIG OSC = INTIO67
        CONFIG WDT = OFF
        org 0x00
	
;	MOVLW 0x2b  ;store the value 0x2b in WORKING REGISTER
;	MOVLW D'15'
;	MOVLW b'00000111'
;	MOVWF 0x00  ;store the value of WORKING REGISTER in 0X00
;	
;	CLRF 0X00  ;clear 0x00
;	
;	INCF 0X00  ;0X00 +1
;	INCF 0X00  
;	
;	DECF 0X00  ;0x00 -1
;	DECF 0X00
;	
;	CLRF WREG  ;clear WORKING REGISTER 
;	CLRF 0X00
;	
;	MOVLW 0x12
;	MOVWF 0x00
;	
;	MOVLW 0X31
;	ADDWF 0X00, W  ;add WORKING REGISTER and 0X00 and put the result in WORKING REGISTER 
;	ADDWF 0X00, F  ;add WORKING REGISTER and 0X00 and put the result in 0X00
;	
;	MOVLW 0x2B
;	MOVWF 0x00
;	CLRF WREG
;	MOVF 0X00, W  ;copy 0X00 to WORKING REGISTER
;	
;	FIRSTLOOP:  ;LABEL
;	    MOVLW 0x04
;	    MOVWF 0x00
;	STARTFIRSTLOOP:
;	    DECFSZ 0X00  ;if(0x00 == 0) else 0x00 - 1
;	    GOTO STARTFIRSTLOOP  ; jump to LABEL STARTFIRSTLOOP
;	
;	INITIAL:  ;LABEL
;	    MOVLW 0x15
;	    MOVWF 0x00
;	    MOVLW 0x15
;	COMPARE:
;	    CPFSEQ 0X00  ;if (WORKING REGISTER == 0X00){INCF 0X01}
;		INCF 0X01
;	    NOP
;	    
;	SECONDLOOP:  ;LABEL
;	    MOVLW b'01101000'
;	    MOVWF 0x00
;	STARTSECONDLOOP:
;	    RRNCF 0X00  ;shift a BIT to right ,will not stop
;	    GOTO STARTSECONDLOOP  ; jump to LABEL START
;	    
;	THIRDLOOP:  ;LABEL
;	    MOVLW b'01101000'
;	    MOVWF 0x00
;	STARTTHIRDLOOP:
;	    RRNCF 0X00 ;shift a BIT to right ,will not stop
;	    BTFSS 0X00, 0 ;check the rightest BIT . It will skip next line,if it is 1 
;	    GOTO STARTTHIRDLOOP  ;jump to LABEL START
;	    INCF 0X01
;	
;	    
;	MOVLW 0X15
;	MOVWF 0X00
;	MOVLW 0X10
;	    
;	SUBWF 0X00,w
	
	CLRF 0X50
	DECF 0X50
	    
	end


