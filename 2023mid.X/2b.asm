    list p=18f4520
    #include <p18f4520.inc>
    CONFIG OSC = INTIO67
    CONFIG WDT = OFF

    org 0x00

    MAIN:
    ;-----------------------------
    ; ??????? S[9] = {45,14,14,17,19,20,11,13,14}
    ;-----------------------------
    MOVLB 0x3  ;BSR = 3
    MOVLW 0x45
    MOVWF 0x00,1
    MOVLW 0x14
    MOVWF 0x01,1
    MOVLW 0x14
    MOVWF 0x02,1
    MOVLW 0x17
    MOVWF 0x03,1
    MOVLW 0x19
    MOVWF 0x04,1
    MOVLW 0x20
    MOVWF 0x05,1
    MOVLW 0x11
    MOVWF 0x06,1
    MOVLW 0x13
    MOVWF 0x07,1
    MOVLW 0x14
    MOVWF 0x08,1

    MOVLW 0x0A        ; count = 9
    MOVWF 0x42

    CALL SORT          ; ???????
    GOTO $

    ;==========================================================
    ; SORT?for (i=0;i<count;i++) for (j=i+1;j<count;j++)
    ;==========================================================
    SORT:
    LFSR 0, 0x300        ; FSR0 -> S[i]
    MOVF 0x02, W
    MOVWF 0x03            ; outer_count = count

    FOR_I:
    DECF 0x03
    BZ DONE_SORT           ; if i >= count then done
    MOVF 0x03, W
    MOVWF 0x04             ; inner_count = count - i

    ; ?? FSR1 ?? S[i+1]
    MOVFF FSR0L, FSR1L
    MOVFF FSR0H, FSR1H
    INCF FSR1L, F          ; FSR1 = FSR0 + 1

    FOR_J:
    DECF 0x04, F
    BZ NEXT_I              ; ? j >= count ????? i

    ; ?? S[i] ? S[j]
    MOVF INDF0, W
    CPFSLT INDF1           ; if (S[i] > S[j]) ???????????
        GOTO SKIP_SWAP

    ; swap(S[i], S[j])
    MOVF INDF0, W
    MOVWF 0x10             ; tmp = S[i]
    MOVF INDF1, W
    MOVWF INDF0            ; S[i] = S[j]
    MOVF 0x10, W
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
