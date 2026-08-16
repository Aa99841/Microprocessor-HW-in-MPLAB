;int clz8(uint8_t x) {
;    if (x == 0) return 8;
;    int n = 0;
;    if ((x & 0xF0) == 0) { n += 4; x <<= 4; }
;    if ((x & 0xC0) == 0) { n += 2; x <<= 2; }
;    if ((x & 0x80) == 0) { n += 1; }
;    return n;
;}
    
List p=18f4520
    #include<p18f4520.inc>
    CONFIG OSC = INTIO67
    CONFIG WDT = OFF
    org 0x00
    
    MOVLW 0xFF ;X
    MOVWF 0X00
    CLRF 0x10
    
    
    
    FOURZERO:
	MOVLW D'16'

	CPFSLT 0X00 ; SKIP IF 0X00 < WREG
	GOTO TWOZERO
	INCF 0X10
	INCF 0X10
	INCF 0X10
	INCF 0X10
	RLNCF 0X00
	RLNCF 0X00
	RLNCF 0X00
	RLNCF 0X00
    
    TWOZERO:
	MOVLW D'64'

	CPFSLT 0X00 ; SKIP IF 0X00 < WREG
	GOTO ONEZERO
	INCF 0X10
	INCF 0X10
	RLNCF 0X00
	RLNCF 0X00
    
    ONEZERO:
	MOVLW D'128'

	CPFSLT 0X00 ; SKIP IF 0X00 < WREG
	GOTO lAST
	INCF 0X10
	RLNCF 0X00
	
    ZERO:
	MOVLW D'0'

	CPFSEQ 0X00 ; SKIP IF 0X00 = WREG
	GOTO lAST
	INCF 0X10
	RLNCF 0X00
    
    lAST:
	CLRF WREG
    
    
    
    end


