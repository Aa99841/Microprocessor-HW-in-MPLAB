;  count the zero before first 1 appear
    
    List p=18f4520
    #include<p18f4520.inc>
    CONFIG OSC = INTIO67
    CONFIG WDT = OFF
    org 0x00
    
    MOVLW 0xFF
    MOVWF 0x00
    
;    if(0x00 == 0){
;	goto LOOP
;    }else{
;	goto ALLZERO
;    }
    TSTFSZ 0X00 ;test 0x00, Skip if 0
	GOTO LOOP
    GOTO ALLZERO
    
    LOOP:
	RLNCF 0X00 ;shift a BIT to right
	BTFSC 0X00, 0 ;check the rightest BIT . It will skip next line,if it is 0 
	GOTO LAST
	INCF 0X10
	GOTO LOOP
	
    ALLZERO:
	MOVLW D'8'
	MOVWF 0x10

    LAST:
	CLRF WREG
	
    
    end


