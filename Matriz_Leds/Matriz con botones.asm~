;
; Contador.asm
;
; Created: 6/9/2024 1:03:39
; Author : Facundo
;

.ORG	 0X0000
RJMP     Inicio
.ORG     0x0002
RJMP     RSI_0
.ORG     0x0004
RJMP     RSI_1

.include "m328Pdef.inc"

Inicio:
       SEI		;Habilitar interrupciones globales

       ; Configurar SP
	   LDI		R16, HIGH(RAMEND)
       OUT		SPH, r16
       LDI		R16, LOW(RAMEND)
       OUT		SPL, r16

       LDI      r16, 0xFF
       OUT      DDRB, r16
	   LDI		r16, 0x0F
	   OUT      DDRC, r16

	   LDI		r18, 0x01
       MOV      YH, r18
       LDI      r18, 0x00
       MOV      YL, r18
	   LDI		r18, 0x01
       MOV      ZH, r18
       LDI      r18, 0x21
       MOV      ZL, r18

	   CLR		r18

	   CALL		Cargar_valores

       ;activamos los pull-up en los pines de INT0 e INT1
       SBI      PORTD, 2
       SBI      PORTD, 3


       ;Configuramos las interrupciones
       LDI      r16, 0X03
       OUT      EIMSK, r16       ;habilita las interrupciones INT0 e INT1
       LDI      r16, 0x0F
       STS      EICRA, r16       ; configura flancos de subida

Wait:
	   LD		r17, Y
	   LD		r18, Z
	   OUT		PORTB, r17
	   OUT		PORTC, r18
       RJMP		Wait

RSI_0:
	   LD		r17, Y+
	   LD		r18, Z+

       RETI

RSI_1:
       LD		r17, -Y
	   LD		r18, -Z
       RETI


Mseg:
       ldi		r21, 30
       ldi		r22, 75
       ldi		r23, 189
L1:
       dec		r23
       brne		L1
       dec		r22
       brne		L1
       dec		r21
       brne		L1
       nop
       RET

Cargar_valores:
    ; Letra G
    LDI     r20, 0b00111100
    ST      Y+, r20
    LDI     r20, 0b01000010
    ST      Y+, r20
    LDI     r20, 0b01000000
    ST      Y+, r20
    LDI     r20, 0b01001110
    ST      Y+, r20
    LDI     r20, 0b01000010
    ST      Y+, r20
    LDI     r20, 0b01000010
    ST      Y+, r20
    LDI     r20, 0b00111100
    ST      Y+, r20
    LDI     r20, 0b00000000
    ST      Y+, r20

    ; Letra U
    LDI     r20, 0b01000010
    ST      Y+, r20
    LDI     r20, 0b01000010
    ST      Y+, r20
    LDI     r20, 0b01000010
    ST      Y+, r20
    LDI     r20, 0b01000010
    ST      Y+, r20
    LDI     r20, 0b01000010
    ST      Y+, r20
    LDI     r20, 0b01000010
    ST      Y+, r20
    LDI     r20, 0b00111100
    ST      Y+, r20
    LDI     r20, 0b00000000
    ST      Y+, r20

    ; Letra I
    LDI     r20, 0b00111100
    ST      Y+, r20
    LDI     r20, 0b00011000
    ST      Y+, r20
    LDI     r20, 0b00011000
    ST      Y+, r20
    LDI     r20, 0b00011000
    ST      Y+, r20
    LDI     r20, 0b00011000
    ST      Y+, r20
    LDI     r20, 0b00011000
    ST      Y+, r20
    LDI     r20, 0b00111100
    ST      Y+, r20
    LDI     r20, 0b00000000
    ST      Y+, r20

    ; Letra D
    LDI     r20, 0b00111000
    ST      Y+, r20
    LDI     r20, 0b01000100
    ST      Y+, r20
    LDI     r20, 0b01000010
    ST      Y+, r20
    LDI     r20, 0b01000010
    ST      Y+, r20
    LDI     r20, 0b01000010
    ST      Y+, r20
    LDI     r20, 0b01000100
    ST      Y+, r20
    LDI     r20, 0b00111000
    ST      Y+, r20
    LDI     r20, 0b00000000
    ST      Y+, r20

    ; Letra O
    LDI     r20, 0b00111100
    ST      Y+, r20
    LDI     r20, 0b01000010
    ST      Y+, r20
    LDI     r20, 0b01000010
    ST      Y+, r20
    LDI     r20, 0b01000010
    ST      Y+, r20
    LDI     r20, 0b01000010
    ST      Y+, r20
    LDI     r20, 0b01000010
    ST      Y+, r20
    LDI     r20, 0b00111100
    ST      Y+, r20
    LDI     r20, 0b00000000
    ST      Y+, r20

    RET



