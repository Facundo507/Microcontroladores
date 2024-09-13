;
; DAC.asm
;
; Created: 13/9/2024 15:15:35
; Author : Facundo
;

.include "m328pdef.inc"

.cseg

.org 0x0100  ; Dirección de memoria para la LUT
    RJMP inicio
	

inicio:
    LDI R16, HIGH(RAMEND)    ; Activamos el SP
    OUT SPH, R16
    LDI R16, LOW(RAMEND)
    OUT SPL, R16

    LDI R16, 0xFF        ; Dejamos el puerto D como salida
    OUT DDRD, R16

    LDI R18, 0X01
    MOV YH, R18
    LDI R18, 0X00
    MOV YL, R18
	RJMP guardar_codigos

BUCLE:
    LD R17, Y
    OUT PORTD, R17
    CALL SEGUNDO
    LD R17, Y+


    LDI R16, 0xCD
    CP YL, R16         ; Compara la parte baja de Y con LOW(lut_data + 128)
    BRNE CONTINUAR     ; Si no son iguales, continúa el bucle

    ; Si hemos llegado al final de la LUT, reiniciamos Y
    LDI YH, 0x01
    LDI YL, 0x00

CONTINUAR:
    RJMP BUCLE

SEGUNDO:
    LDI R21, 255
    LDI R22, 255
RETARDO:
    DEC R22
    BRNE RETARDO
    DEC R21
    BRNE RETARDO
	RET

guardar_codigos:
	ldi r28, 0x00     ; LOW (0x0100)
	ldi r29, 0x01     ; HIGH (0x0100)

	ldi r20, 0x49     ; Cargamos 0x49
	ST Y+, r20
	ldi r20, 0x4A     ; Cargamos 0x4A
	ST Y+, r20
	ldi r20, 0x4B     ; Cargamos 0x4B
	ST Y+, r20
	ldi r20, 0x4B     ; Cargamos 0x4B
	ST Y+, r20
	ldi r20, 0x4A     ; Cargamos 0x4A
	ST Y+, r20
	ldi r20, 0x49     ; Cargamos 0x49
	ST Y+, r20
	ldi r20, 0x49     ; Cargamos 0x49
	ST Y+, r20
	ldi r20, 0x49     ; Cargamos 0x49
	ST Y+, r20
	ldi r20, 0x49     ; Cargamos 0x49
	ST Y+, r20
	ldi r20, 0x48     ; Cargamos 0x48
	ST Y+, r20
	ldi r20, 0x47     ; Cargamos 0x47
	ST Y+, r20
	ldi r20, 0x45     ; Cargamos 0x45
	ST Y+, r20
	ldi r20, 0x44     ; Cargamos 0x44
	ST Y+, r20
	ldi r20, 0x43     ; Cargamos 0x43
	ST Y+, r20
	ldi r20, 0x43     ; Cargamos 0x43
	ST Y+, r20
	ldi r20, 0x43     ; Cargamos 0x43
	ST Y+, r20
	ldi r20, 0x44     ; Cargamos 0x44
	ST Y+, r20
	ldi r20, 0x44     ; Cargamos 0x44
	ST Y+, r20
	ldi r20, 0x43     ; Cargamos 0x43
	ST Y+, r20
	ldi r20, 0x41     ; Cargamos 0x41
	ST Y+, r20
	ldi r20, 0x3E     ; Cargamos 0x3E
	ST Y+, r20
	ldi r20, 0x3D     ; Cargamos 0x3D
	ST Y+, r20
	ldi r20, 0x3B     ; Cargamos 0x3B
	ST Y+, r20
	ldi r20, 0x39     ; Cargamos 0x39
	ST Y+, r20
	ldi r20, 0x38     ; Cargamos 0x38
	ST Y+, r20
	ldi r20, 0x37     ; Cargamos 0x37
	ST Y+, r20
	ldi r20, 0x37     ; Cargamos 0x37
	ST Y+, r20
	ldi r20, 0x36     ; Cargamos 0x36
	ST Y+, r20
	ldi r20, 0x36     ; Cargamos 0x36
	ST Y+, r20
	ldi r20, 0x36     ; Cargamos 0x36
	ST Y+, r20
	ldi r20, 0x37     ; Cargamos 0x37
	ST Y+, r20
	ldi r20, 0x37     ; Cargamos 0x37
	ST Y+, r20
	ldi r20, 0x37     ; Cargamos 0x37
	ST Y+, r20
	ldi r20, 0x37     ; Cargamos 0x37
	ST Y+, r20
	ldi r20, 0x37     ; Cargamos 0x37
	ST Y+, r20
	ldi r20, 0x37     ; Cargamos 0x37
	ST Y+, r20
	ldi r20, 0x36     ; Cargamos 0x36
	ST Y+, r20
	ldi r20, 0x35     ; Cargamos 0x35
	ST Y+, r20
	ldi r20, 0x33     ; Cargamos 0x33
	ST Y+, r20
	ldi r20, 0x32     ; Cargamos 0x32
	ST Y+, r20
	ldi r20, 0x31     ; Cargamos 0x31
	ST Y+, r20
	ldi r20, 0x31     ; Cargamos 0x31
	ST Y+, r20
	ldi r20, 0x34     ; Cargamos 0x34
	ST Y+, r20
	ldi r20, 0x3D     ; Cargamos 0x3D
	ST Y+, r20
	ldi r20, 0x4D     ; Cargamos 0x4D
	ST Y+, r20
	ldi r20, 0x65     ; Cargamos 0x65
	ST Y+, r20
	ldi r20, 0x84     ; Cargamos 0x84
	ST Y+, r20
	ldi r20, 0xA9     ; Cargamos 0xA9
	ST Y+, r20
	ldi r20, 0xCF     ; Cargamos 0xCF
	ST Y+, r20
	ldi r20, 0xEE     ; Cargamos 0xEE
	ST Y+, r20
	ldi r20, 0xFF     ; Cargamos 0xFF
	ST Y+, r20
	ldi r20, 0xFE     ; Cargamos 0xFE
	ST Y+, r20
	ldi r20, 0xEA     ; Cargamos 0xEA
	ST Y+, r20
	ldi r20, 0xC6     ; Cargamos 0xC6
	ST Y+, r20
	ldi r20, 0x9A     ; Cargamos 0x9A
	ST Y+, r20
	ldi r20, 0x6D     ; Cargamos 0x6D
	ST Y+, r20
	ldi r20, 0x44     ; Cargamos 0x44
	ST Y+, r20
	ldi r20, 0x25     ; Cargamos 0x25
	ST Y+, r20
	ldi r20, 0x11     ; Cargamos 0x11
	ST Y+, r20
	ldi r20, 0x05     ; Cargamos 0x05
	ST Y+, r20
	ldi r20, 0x00     ; Cargamos 0x00
	ST Y+, r20
	ldi r20, 0x01     ; Cargamos 0x01
	ST Y+, r20
	ldi r20, 0x06     ; Cargamos 0x06
	ST Y+, r20
	ldi r20, 0x0D     ; Cargamos 0x0D
	ST Y+, r20
	ldi r20, 0x14     ; Cargamos 0x14
	ST Y+, r20
	ldi r20, 0x1C     ; Cargamos 0x1C
	ST Y+, r20
	ldi r20, 0x24     ; Cargamos 0x24
	ST Y+, r20
	ldi r20, 0x2D     ; Cargamos 0x2D
	ST Y+, r20
	ldi r20, 0x34     ; Cargamos 0x34
	ST Y+, r20
	ldi r20, 0x37     ; Cargamos 0x37
	ST Y+, r20
	ldi r20, 0x39     ; Cargamos 0x39
	ST Y+, r20
	ldi r20, 0x3D     ; Cargamos 0x3D
	ST Y+, r20
	ldi r20, 0x40     ; Cargamos 0x40
	ST Y+, r20
	ldi r20, 0x41     ; Cargamos 0x41
	ST Y+, r20
	ldi r20, 0x42     ; Cargamos 0x42
	ST Y+, r20
	ldi r20, 0x43     ; Cargamos 0x43
	ST Y+, r20
	ldi r20, 0x44     ; Cargamos 0x44
	ST Y+, r20
	ldi r20, 0x44     ; Cargamos 0x44
	ST Y+, r20
	ldi r20, 0x45     ; Cargamos 0x45
	ST Y+, r20
	ldi r20, 0x46     ; Cargamos 0x46
	ST Y+, r20
	ldi r20, 0x47     ; Cargamos 0x47
	ST Y+, r20
	ldi r20, 0x47     ; Cargamos 0x47
	ST Y+, r20
	ldi r20, 0x47     ; Cargamos 0x47
	ST Y+, r20
	ldi r20, 0x47     ; Cargamos 0x47
	ST Y+, r20
	ldi r20, 0x47     ; Cargamos 0x47
	ST Y+, r20
	ldi r20, 0x47     ; Cargamos 0x47
	ST Y+, r20
	ldi r20, 0x47     ; Cargamos 0x47
	ST Y+, r20
	ldi r20, 0x48     ; Cargamos 0x48
	ST Y+, r20
	ldi r20, 0x48     ; Cargamos 0x48
	ST Y+, r20
	ldi r20, 0x48     ; Cargamos 0x48
	ST Y+, r20
	ldi r20, 0x49     ; Cargamos 0x49
	ST Y+, r20
	ldi r20, 0x49     ; Cargamos 0x49
	ST Y+, r20
	ldi r20, 0x4A     ; Cargamos 0x4A
	ST Y+, r20
	ldi r20, 0x4B     ; Cargamos 0x4B
	ST Y+, r20
	ldi r20, 0x4B     ; Cargamos 0x4B
	ST Y+, r20
	ldi r20, 0x4C     ; Cargamos 0x4C
	ST Y+, r20
	ldi r20, 0x4D     ; Cargamos 0x4D
	ST Y+, r20
	ldi r20, 0x4E     ; Cargamos 0x4E
	ST Y+, r20
	ldi r20, 0x4F     ; Cargamos 0x4F
	ST Y+, r20
	ldi r20, 0x50     ; Cargamos 0x50
	ST Y+, r20
	ldi r20, 0x51     ; Cargamos 0x51
	ST Y+, r20
	ldi r20, 0x52     ; Cargamos 0x52
	ST Y+, r20
	ldi r20, 0x53     ; Cargamos 0x53
	ST Y+, r20
	ldi r20, 0x54     ; Cargamos 0x54
	ST Y+, r20
	ldi r20, 0x56     ; Cargamos 0x56
	ST Y+, r20
	ldi r20, 0x58     ; Cargamos 0x58
	ST Y+, r20
	ldi r20, 0x5B     ; Cargamos 0x5B
	ST Y+, r20
	ldi r20, 0x5D     ; Cargamos 0x5D
	ST Y+, r20
	ldi r20, 0x60     ; Cargamos 0x60
	ST Y+, r20
	ldi r20, 0x64     ; Cargamos 0x64
	ST Y+, r20
	ldi r20, 0x66     ; Cargamos 0x66
	ST Y+, r20
	ldi r20, 0x68     ; Cargamos 0x68
	ST Y+, r20
	ldi r20, 0x6B     ; Cargamos 0x6B
	ST Y+, r20
	ldi r20, 0x6D     ; Cargamos 0x6D
	ST Y+, r20
	ldi r20, 0x70     ; Cargamos 0x70
	ST Y+, r20
	ldi r20, 0x73     ; Cargamos 0x73
	ST Y+, r20
	ldi r20, 0x76     ; Cargamos 0x76
	ST Y+, r20
	ldi r20, 0x79     ; Cargamos 0x79
	ST Y+, r20
	ldi r20, 0x7B     ; Cargamos 0x7B
	ST Y+, r20
	ldi r20, 0x7D     ; Cargamos 0x7D
	ST Y+, r20
	ldi r20, 0x7E     ; Cargamos 0x7E
	ST Y+, r20
	ldi r20, 0x7F     ; Cargamos 0x7F
	ST Y+, r20
	ldi r20, 0x7F     ; Cargamos 0x7F
	ST Y+, r20
	ldi r20, 0x7F     ; Cargamos 0x7F
	ST Y+, r20
	ldi r20, 0x7F     ; Cargamos 0x7F
	ST Y+, r20
	ldi r20, 0x7F     ; Cargamos 0x7F
	ST Y+, r20
	ldi r20, 0x7E     ; Cargamos 0x7E
	ST Y+, r20
	ldi r20, 0x7D     ; Cargamos 0x7D
	ST Y+, r20
	ldi r20, 0x7C     ; Cargamos 0x7C
	ST Y+, r20
	ldi r20, 0x79     ; Cargamos 0x79
	ST Y+, r20
	ldi r20, 0x77     ; Cargamos 0x77
	ST Y+, r20
	ldi r20, 0x74     ; Cargamos 0x74
	ST Y+, r20
	ldi r20, 0x71     ; Cargamos 0x71
	ST Y+, r20
	ldi r20, 0x6D     ; Cargamos 0x6D
	ST Y+, r20
	ldi r20, 0x6A     ; Cargamos 0x6A
	ST Y+, r20
	ldi r20, 0x62     ; Cargamos 0x62
	ST Y+, r20
	ldi r20, 0x5F     ; Cargamos 0x5F
	ST Y+, r20
	ldi r20, 0x5C     ; Cargamos 0x5C
	ST Y+, r20
	ldi r20, 0x59     ; Cargamos 0x59
	ST Y+, r20
	ldi r20, 0x56     ; Cargamos 0x56
	ST Y+, r20
	ldi r20, 0x54     ; Cargamos 0x54
	ST Y+, r20
	ldi r20, 0x51     ; Cargamos 0x51
	ST Y+, r20
	ldi r20, 0x4F     ; Cargamos 0x4F
	ST Y+, r20
	ldi r20, 0x4D     ; Cargamos 0x4D
	ST Y+, r20
	ldi r20, 0x4C     ; Cargamos 0x4C
	ST Y+, r20
	ldi r20, 0x4A     ; Cargamos 0x4A
	ST Y+, r20
	ldi r20, 0x49     ; Cargamos 0x49
	ST Y+, r20
	ldi r20, 0x48     ; Cargamos 0x48
	ST Y+, r20
	ldi r20, 0x47     ; Cargamos 0x47
	ST Y+, r20
	ldi r20, 0x43     ; Cargamos 0x43
	ST Y+, r20
	ldi r20, 0x43     ; Cargamos 0x43
	ST Y+, r20
	ldi r20, 0x43     ; Cargamos 0x43
	ST Y+, r20
	ldi r20, 0x44     ; Cargamos 0x44
	ST Y+, r20
	ldi r20, 0x44     ; Cargamos 0x44
	ST Y+, r20
	ldi r20, 0x44     ; Cargamos 0x44
	ST Y+, r20
	ldi r20, 0x45     ; Cargamos 0x45
	ST Y+, r20
	ldi r20, 0x45     ; Cargamos 0x45
	ST Y+, r20
	ldi r20, 0x45     ; Cargamos 0x45
	ST Y+, r20
	ldi r20, 0x45     ; Cargamos 0x45
	ST Y+, r20
	ldi r20, 0x45     ; Cargamos 0x45
	ST Y+, r20
	ldi r20, 0x45     ; Cargamos 0x45
	ST Y+, r20
	ldi r20, 0x44     ; Cargamos 0x44
	ST Y+, r20
	ldi r20, 0x44     ; Cargamos 0x44
	ST Y+, r20
	ldi r20, 0x43     ; Cargamos 0x43
	ST Y+, r20
	ldi r20, 0x43     ; Cargamos 0x43
	ST Y+, r20
	ldi r20, 0x43     ; Cargamos 0x43
	ST Y+, r20
	ldi r20, 0x43     ; Cargamos 0x43
	ST Y+, r20
	ldi r20, 0x42     ; Cargamos 0x42
	ST Y+, r20
	ldi r20, 0x42     ; Cargamos 0x42
	ST Y+, r20
	ldi r20, 0x42     ; Cargamos 0x42
	ST Y+, r20
	ldi r20, 0x41     ; Cargamos 0x41
	ST Y+, r20
	ldi r20, 0x41     ; Cargamos 0x41
	ST Y+, r20
	ldi r20, 0x41     ; Cargamos 0x41
	ST Y+, r20
	ldi r20, 0x41     ; Cargamos 0x41
	ST Y+, r20
	ldi r20, 0x41     ; Cargamos 0x41
	ST Y+, r20
	ldi r20, 0x41     ; Cargamos 0x41
	ST Y+, r20
	ldi r20, 0x41     ; Cargamos 0x41
	ST Y+, r20
	ldi r20, 0x40     ; Cargamos 0x40
	ST Y+, r20
	ldi r20, 0x40     ; Cargamos 0x40
	ST Y+, r20
	ldi r20, 0x3F     ; Cargamos 0x3F
	ST Y+, r20
	ldi r20, 0x3F     ; Cargamos 0x3F
	ST Y+, r20
	ldi r20, 0x40     ; Cargamos 0x40
	ST Y+, r20
	ldi r20, 0x40     ; Cargamos 0x40
	ST Y+, r20
	ldi r20, 0x41     ; Cargamos 0x41
	ST Y+, r20
	ldi r20, 0x41     ; Cargamos 0x41
	ST Y+, r20
	ldi r20, 0x41     ; Cargamos 0x41
	ST Y+, r20
	ldi r20, 0x41     ; Cargamos 0x41
	ST Y+, r20
	ldi r20, 0x41     ; Cargamos 0x41
	ST Y+, r20
	ldi r20, 0x41     ; Cargamos 0x41
	ST Y+, r20
	ldi r20, 0x40     ; Cargamos 0x40
	ST Y+, r20
	ldi r20, 0x40     ; Cargamos 0x40
	ST Y+, r20
	ldi r20, 0x40     ; Cargamos 0x40
	ST Y+, r20
	ldi r20, 0x40     ; Cargamos 0x40
	ST Y+, r20
	ldi r20, 0x40     ; Cargamos 0x40
	ST Y+, r20
	ldi r20, 0x40     ; Cargamos 0x40
	ST Y+, r20
	ldi r20, 0x40     ; Cargamos 0x40
	ST Y+, r20
	ldi r20, 0x41     ; Cargamos 0x41
	ST Y+, r20
	ldi r20, 0x41     ; Cargamos 0x41
	ST Y+, r20
	ldi r20, 0x41     ; Cargamos 0x41
	ST Y+, r20
	ldi r20, 0x42     ; Cargamos 0x42
	ST Y+, r20
	ldi r20, 0x43     ; Cargamos 0x43
	ST Y+, r20
	ldi r20, 0x44     ; Cargamos 0x44
	ST Y+, r20
	ldi r20, 0x45     ; Cargamos 0x45
	ST Y+, r20
	ldi r20, 0x47     ; Cargamos 0x47
	ST Y+, r20
	ldi r20, 0x48     ; Cargamos 0x48
	ST Y+, r20
	ldi r20, 0x49     ; Cargamos 0x49
	ST Y+, r20
	
	RJMP BUCLE
