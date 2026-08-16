List p=18f4520
    #include<p18f4520.inc>
    CONFIG OSC = INTIO67
    CONFIG WDT = OFF
    org 0x00
	
    ;COUNT THE SQUARE ROOT OF INPUT
    ;X_n+1 = floor((X_n + floor(N / X_n)) / 2)
    ; X_n+1 = X_n => Ans: X_n
    
    ; input:
    ;	N = [0x020][0x021]
    ;	X_n = [0x022][0x023]
    ;	X_n+1 = [0x030][0x031]
    ;	ans = [0x024][0x025]
    
    main:
	MOVLW 0X30
	MOVWF 0X20 ;NH
	MOVLW 0X21
	MOVWF 0X21 ;NL
	MOVLW 0X26
	MOVWF 0X22 ;xH
	MOVLW 0X5D 
	MOVWF 0X23 ;xL
	
	RCALL newtonSqrt
	GOTO LAST
	NOP
	
    newtonSqrt:
	MOVFF 0X20, 0X00
	MOVFF 0X21, 0X01
	MOVFF 0X22, 0X02
	MOVFF 0X23, 0X03
	
	; caluate A1 = floor(N / X_n)
	CLRF 0X10
	CLRF 0X11
	RCALL division
	
	; caluate A2 = X_n + A1
	MOVF 0X11,W
	ADDWF 0X23,W
	MOVWF 0X31
	
	MOVF 0X10,W
	ADDWFC 0X22,W
	MOVWF 0X30
	
	; calute A3 = floor(A2 / 2)
	BCF     STATUS, C ; clear the carry bit
	RRCF     0X30, F ; rotate right though the carry bit
	RRCF     0X31, F ; rotate right though the carry bit
	
    SqrtCOMPARE1:
	MOVF 0X22,W
	CPFSEQ 0X30
	GOTO UPDATE
	GOTO SqrtCOMPARE2
	
    SqrtCOMPARE2:
	MOVF 0X23,W
	CPFSEQ 0X31
	GOTO UPDATE
	GOTO SqrtDONE    
	
	
    UPDATE:
	MOVFF 0X30, 0X22
	MOVFF 0X31, 0X23
	GOTO newtonSqrt
    
    SqrtDONE:
	MOVF 0X30,W
	MOVWF 0X24
	MOVF 0X31,W
	MOVWF 0X25
	RETURN
	
	
    division:
	MOVF 0X00, W  ;WREG = A
	SUBWF 0X02,W  ;WREG = WREG - B
	BNC DIV_SUB    ;B < A
	MOVF 0X00, W
	CPFSEQ 0X02
	GOTO DIV_DONE
	GOTO COMPARE2
	
    COMPARE2:
	MOVF 0X03, W  ;WREG = D
	SUBWF 0X01,W  ;WREG = WREG - C
	BC DIV_SUB     ;C >= D
	GOTO DIV_DONE
	
    DIV_SUB:
	MOVLW 0X01
	ADDWF 0X11,W
	MOVWF 0X11
	
	MOVLW 0X00
	ADDWFC 0X10,W
	MOVWF 0X10
	
	MOVF    0X03, W
	SUBWF   0X01, F
	MOVF    0X02, W
	SUBWFB  0X00, F
	
	GOTO division
	
    DIV_DONE:
	MOVFF 0X00,WREG
	MOVWF 0X12
	MOVFF 0X01,WREG
	MOVWF 0X13
	RETURN

    LAST:
	NOP
end

