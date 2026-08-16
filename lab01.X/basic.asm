;    compare x1+x2 = A1 and y1-y2 = A2
;    if A1 > A2 store 0xff in 0x20
    
    List p=18f4520 
    #include<p18f4520.inc>
    CONFIG OSC = INTIO67
    CONFIG WDT = OFF
    org 0x00
    
    MOVLW 0xB6  ;WREG = 0x01
    MOVWF 0x00  ;x1
    
    MOVLW 0x0C  ;WREG = 0x02
    MOVWF 0x01  ;x2
    
    ADDWF 0X00, W  ;WREG = 0x02, x1 + 0x02
    MOVWF 0x10  ;A1
    
    MOVLW 0xD3  ;WREG = 0x03
    MOVWF 0x02  ;y1
    
    MOVLW 0xB7  ;WREG = 0x04
    MOVWF 0x03  ;y2
    
    SUBWF 0X02, W  ;WREG = 0x04, y1 - 0x04
    MOVWF 0x11  ;A2
    
    CLRF WREG
    MOVF 0X11, w
    
    CPFSGT 0X10  ;if (WORKING REGISTER > 0X10), skip if 0X11(A2) < 0x10(A1) 
	GOTO BIGER
    NOP
    
    SMALLER: ;A1 > A2
	MOVLW 0xFF
	GOTO LAST
	  
    BIGER: ;A2 >= A1
	MOVLW 0x01
	GOTO LAST
	
    LAST:
	MOVWF 0x20
end


