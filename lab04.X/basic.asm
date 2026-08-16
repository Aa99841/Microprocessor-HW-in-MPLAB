List p=18f4520
    #include<p18f4520.inc>
    CONFIG OSC = INTIO67
    CONFIG WDT = OFF
    org 0x00
    
    ; create a macro(function) for 
    ;	[0x000][0x001] = [0x002][0x003] & [0x004][0x005]
    ;   [0x010][0x011] = [0x000]*[0x001]
    
    And_Mul  macro xh, xl, yh, yl
    MOVLW xh
    MOVWF 0X02
    MOVLW xl
    MOVWF 0X03
    MOVLW yh
    MOVWF 0X04
    MOVLW yl
    MOVWF 0X05
    
    ;[0x004] & [0x002]
    MOVLW xh
    ANDWF 0X04,W
    MOVWF 0X00
    
    ;[0x005] & [0x003]
    MOVLW xl
    ANDWF 0X05,W
    MOVWF 0X01
    
    ;[0x000]*[0x001]
    MOVFF 0X00,WREG
    MULWF 0X01
    MOVFF PRODH, WREG
    MOVWF 0X10
    MOVFF PRODL, WREG
    MOVWF 0X11
    
    endm 
    
    main:
    And_Mul 0X50, 0X6F, 0X3A, 0XBC
    
    
end
