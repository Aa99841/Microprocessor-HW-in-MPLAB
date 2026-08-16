#INCLUDE <p18f4520.inc>
    CONFIG OSC = INTIO67
    CONFIG WDT = OFF
    org 0x00    ;PC = 0x00 

setup1:
    LFSR 0, 0x000    ; FSR0 point to 0x000
    LFSR 1, 0x010    ; FSR1 point to 0x010
    LFSR 2, 0x020    ; FSR2 point to 0x020
    MOVLW 0x10       ; WREG = 0x10

start:
    INCF POSTINC0 
    ; [0x000] += 1; FSR0 point to 0x001
 
    INCF PREINC1 
    ; FSR1 point to 0x011 ;[0x011] += 1
 
    INCF POSTDEC2 
    ; [0x020] += 1 ; FSR2 point to 0x01F
 
    INCF INDF2 
    ; [0x01F] += 1 ; 
    ; FSR2 point to 0x01F(unchanged)
 
    INCF PLUSW2 ;[FSR2 + WREG] += 1
    ; [0x01F+0x10] += 1 
    ; FSR2 point to 0x01F(unchanged)
end



