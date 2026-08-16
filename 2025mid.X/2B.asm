List p=18f4520
    #include<p18f4520.inc>
    CONFIG OSC = INTIO67
    CONFIG WDT = OFF
    org 0x00

MAIN:
    ;-----------------------------
    ; ??????? S[9] = {45,14,14,17,19,20,11,13,14}
    ;-----------------------------
    MOVLW 0x3B
    MOVWF 0x10
    MOVLW 0xF7
    MOVWF 0x11
    MOVLW 0xA2
    MOVWF 0x12
    
    MOVLW 0x50
    MOVWF 0x20
    MOVLW 0xA2
    MOVWF 0x21
    MOVLW 0xCB
    MOVWF 0x22
    
    MOVLW 0xA2
    MOVWF 0x30
    MOVLW 0xE9
    MOVWF 0x31
    MOVLW 0X13
    MOVWF 0x32
    
    MOVFF 0X10,0X00
    MOVFF 0X11,0X01
    MOVFF 0X12,0X02
    MOVFF 0X20,0X03
    MOVFF 0X21,0X04
    MOVFF 0X22,0X05
    MOVFF 0X30,0X06
    MOVFF 0X31,0X07
    MOVFF 0X32,0X08
    

    MOVLW 0x0A        ; count = 9
    MOVWF 0x42

    CALL SORT          ; ???????
    GOTO $

    ;==========================================================
    ; SORT?for (i=0;i<count;i++) for (j=i+1;j<count;j++)
    ;==========================================================
    SORT:
    LFSR 0, 0x000        ; FSR0 -> S[i]
    MOVF 0x42, W
    MOVWF 0x43            ; outer_count = count

    FOR_I:
    DECF 0x43
    BZ DONE_SORT           ; if i >= count then done
    MOVF 0x43, W
    MOVWF 0x44             ; inner_count = count - i

    ; ?? FSR1 ?? S[i+1]
    MOVFF FSR0L, FSR1L
    MOVFF FSR0H, FSR1H
    INCF FSR1L, F          ; FSR1 = FSR0 + 1

    FOR_J:
    DECF 0x44, F
    BZ NEXT_I              ; ? j >= count ????? i

    ; ?? S[i] ? S[j]
    MOVF INDF0, W
    CPFSLT INDF1           ; if (S[i] > S[j]) ???????????
        GOTO SKIP_SWAP

    ; swap(S[i], S[j])
    MOVF INDF0, W
    MOVWF 0x30             ; tmp = S[i]
    MOVF INDF1, W
    MOVWF INDF0            ; S[i] = S[j]
    MOVF 0x30, W
    MOVWF INDF1            ; S[j] = tmp

    SKIP_SWAP:
    INCF FSR1L, F          ; j++
    GOTO FOR_J

    NEXT_I:
    INCF FSR0L, F          ; i++
    GOTO FOR_I

    DONE_SORT:
    RETURN

    
END