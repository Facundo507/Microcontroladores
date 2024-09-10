;
; Matriz.asm
;
; Created: 10/9/2024 15:26:50
; Author : Facundo
;


; Replace with your application code
;
; Contador_modificado.asm
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
       SEI		; Habilitar interrupciones globales

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
       LDI      r18, 0x64
       MOV      ZL, r18

	   CLR		r18

	   CALL		Cargar_valores

       ; Activar pull-up en los pines de INT0 e INT1
       SBI      PORTD, 2
       SBI      PORTD, 3

       ; Configurar interrupciones
       LDI      r16, 0X03
       OUT      EIMSK, r16       ; Habilitar INT0 e INT1
       LDI      r16, 0x0F
       STS      EICRA, r16       ; Configurar flancos de subida

	   LDI		r17, 0x00
	   LDI		r18, 0x64

Wait:	
	   MOV		YL, r17
	   MOV		ZL, r18
	   LD		r18, Z
	   LD		r17, Y
	   OUT		PORTB, r17
	   OUT		PORTC, r18
	   CALL		Mseg
	   LD		r18, Z+
	   LD		r17, Y+
	   OUT		PORTB, r17
	   OUT		PORTC, r18
	   CALL		Mseg
	   LD		r18, Z+
	   LD		r17, Y+
	   OUT		PORTB, r17
	   OUT		PORTC, r18
	   CALL		Mseg
	   LD		r18, Z+
	   LD		r17, Y+
	   OUT		PORTB, r17
	   OUT		PORTC, r18
	   CALL		Mseg
	   LD		r18, Z+
	   LD		r17, Y+
	   OUT		PORTB, r17
	   OUT		PORTC, r18
	   CALL		Mseg
	   LD		r18, Z+
	   LD		r17, Y+
	   OUT		PORTB, r17
	   OUT		PORTC, r18
	   CALL		Mseg
	   LD		r18, Z+
	   LD		r17, Y+
	   OUT		PORTB, r17
	   OUT		PORTC, r18
	   CALL		Mseg
	   LD		r18, Z+
	   LD		r17, Y+
	   OUT		PORTB, r17
	   OUT		PORTC, r18
	   CALL		Mseg

       RJMP		Wait

RSI_0:
	   LD		r17, Y+
	   LD		r18, Z+
       OUT      PORTB, r17
       OUT      PORTC, r18
       RETI

RSI_1:
       LD		r17, -Y
	   LD		r18, -Z
       OUT      PORTB, r17
       OUT      PORTC, r18
       RETI

Mseg:
       ldi		r21, 1
       ldi		r22, 1
       ldi		r23, 50
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
		; Letra H
		LDI     r20, 0b10011001
		ST      Y+, r20
		LDI     r20, 0b10011001
		ST      Y+, r20
		LDI     r20, 0b10011001
		ST      Y+, r20
		LDI     r20, 0b10000001
		ST      Y+, r20
		LDI     r20, 0b10011001
		ST      Y+, r20
		LDI     r20, 0b10011001
		ST      Y+, r20
		LDI     r20, 0b10011001
		ST      Y+, r20
		LDI     r20, 0b10011001
		ST      Y+, r20

		; Letra O
		LDI     r20, 0b11000011
		ST      Y+, r20
		LDI     r20, 0b10111101
		ST      Y+, r20
		LDI     r20, 0b10111101
		ST      Y+, r20
		LDI     r20, 0b10111101
		ST      Y+, r20
		LDI     r20, 0b10111101
		ST      Y+, r20
		LDI     r20, 0b10111101
		ST      Y+, r20
		LDI     r20, 0b11000011
		ST      Y+, r20
		LDI     r20, 0b11111111
		ST      Y+, r20


		; Letra L
		LDI     r20, 0b11000111
		ST      Y+, r20
		LDI     r20, 0b11000111
		ST      Y+, r20
		LDI     r20, 0b11000111
		ST      Y+, r20
		LDI     r20, 0b11000111
		ST      Y+, r20
		LDI     r20, 0b11000111
		ST      Y+, r20
		LDI     r20, 0b11000111
		ST      Y+, r20
		LDI     r20, 0b11111111
		ST      Y+, r20
		LDI     r20, 0b11111111
		ST      Y+, r20


		; Letra A
		LDI     r20, 0b10011001
		ST      Y+, r20
		LDI     r20, 0b10110110
		ST      Y+, r20
		LDI     r20, 0b10110110
		ST      Y+, r20
		LDI     r20, 0b10110110
		ST      Y+, r20
		LDI     r20, 0b11111111
		ST      Y+, r20
		LDI     r20, 0b10110110
		ST      Y+, r20
		LDI     r20, 0b10110110
		ST      Y+, r20
		LDI     r20, 0b11111111
		ST      Y+, r20


		; Letra U
		LDI     r20, 0b10111011
		ST      Y+, r20
		LDI     r20, 0b10111011
		ST      Y+, r20
		LDI     r20, 0b10111011
		ST      Y+, r20
		LDI     r20, 0b10111011
		ST      Y+, r20
		LDI     r20, 0b10111011
		ST      Y+, r20
		LDI     r20, 0b10111011
		ST      Y+, r20
		LDI     r20, 0b11000011
		ST      Y+, r20
		LDI     r20, 0b11111111
		ST      Y+, r20

		; Letra R
		LDI     r20, 0b10000011
		ST      Y+, r20
		LDI     r20, 0b10111011
		ST      Y+, r20
		LDI     r20, 0b10111011
		ST      Y+, r20
		LDI     r20, 0b10000111
		ST      Y+, r20
		LDI     r20, 0b10000000
		ST      Y+, r20
		LDI     r20, 0b10000000
		ST      Y+, r20
		LDI     r20, 0b11111111
		ST      Y+, r20
		LDI     r20, 0b11111111
		ST      Y+, r20

		; Letra U
		LDI     r20, 0b10111011
		ST      Y+, r20
		LDI     r20, 0b10111011
		ST      Y+, r20
		LDI     r20, 0b10111011
		ST      Y+, r20
		LDI     r20, 0b10111011
		ST      Y+, r20
		LDI     r20, 0b10111011
		ST      Y+, r20
		LDI     r20, 0b10111011
		ST      Y+, r20
		LDI     r20, 0b11000011
		ST      Y+, r20
		LDI     r20, 0b11111111
		ST      Y+, r20

		; Letra G
		LDI     r20, 0b11000011
		ST      Y+, r20
		LDI     r20, 0b10111101
		ST      Y+, r20
		LDI     r20, 0b10111111
		ST      Y+, r20
		LDI     r20, 0b10110001
		ST      Y+, r20
		LDI     r20, 0b10111101
		ST      Y+, r20
		LDI     r20, 0b10111101
		ST      Y+, r20
		LDI     r20, 0b11000011
		ST      Y+, r20
		LDI     r20, 0b11111111
		ST      Y+, r20

		; Letra U
		LDI     r20, 0b10111011
		ST      Y+, r20
		LDI     r20, 0b10111011
		ST      Y+, r20
		LDI     r20, 0b10111011
		ST      Y+, r20
		LDI     r20, 0b10111011
		ST      Y+, r20
		LDI     r20, 0b10111011
		ST      Y+, r20
		LDI     r20, 0b10111011
		ST      Y+, r20
		LDI     r20, 0b11000011
		ST      Y+, r20
		LDI     r20, 0b11111111
		ST      Y+, r20

		; Letra A
		LDI     r20, 0b10011001
		ST      Y+, r20
		LDI     r20, 0b10110110
		ST      Y+, r20
		LDI     r20, 0b10110110
		ST      Y+, r20
		LDI     r20, 0b10110110
		ST      Y+, r20
		LDI     r20, 0b11111111
		ST      Y+, r20
		LDI     r20, 0b10110110
		ST      Y+, r20
		LDI     r20, 0b10110110
		ST      Y+, r20
		LDI     r20, 0b11111111
		ST      Y+, r20

		; Letra Y
		LDI     r20, 0b10111011
		ST      Y+, r20
		LDI     r20, 0b10111011
		ST      Y+, r20
		LDI     r20, 0b10111011
		ST      Y+, r20
		LDI     r20, 0b11000011
		ST      Y+, r20
		LDI     r20, 0b11000011
		ST      Y+, r20
		LDI     r20, 0b11000011
		ST      Y+, r20
		LDI     r20, 0b11111111
		ST      Y+, r20


		; Filas
		LDI		r20, 0xF8
		ST		Z+, r20
		LDI		r20, 0xF9
		ST		Z+, r20
		LDI		r20, 0xFA
		ST		Z+, r20
		LDI		r20, 0xFB
		ST		Z+, r20
		LDI		r20, 0xFC
		ST		Z+, r20
		LDI		r20, 0xFD
		ST		Z+, r20
		LDI		r20, 0xFE
		ST		Z+, r20
		LDI		r20, 0xFF
		ST		Z+, r20
		RET

