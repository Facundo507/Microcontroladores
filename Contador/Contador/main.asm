;
; Contador.asm
;
; Created: 6/9/2024 1:03:39
; Author : Facundo
;


.include "m328pdef.inc" ; Define device ATmega328P
.ORG	 0X0000
RJMP     Inicio
.ORG     0x0002
RJMP     RSI_0
.ORG     0x0004
RJMP     RSI_1

Inicio:
       SEI		;Habilitar interrupciones globales

       ; Configurar SP
	   LDI		R16, HIGH(RAMEND)
       OUT		SPH, r16
       LDI		R16, LOW(RAMEND)
       OUT		SPL, r16

       LDI      r16, 0xFF
       OUT      DDRC, r16
       OUT      DDRB, r16
	   LDI		r17, 0b01000000
	   OUT		PORTC, r17

	   LDI		r18, 0x01
       MOV      YH, r18
       LDI      r18, 0x00
       MOV      YL, r18

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
       OUT		PORTC, r17
       SBI		PORTB, 0
       CALL		Mseg
       CBI		PORTB, 0
       CALL		Mseg
       RJMP		Wait

RSI_0:
	   LD		r17, Y+
       RETI

RSI_1:
       LD		r17, -Y
       RETI


Mseg:
       ldi		r21, 21
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
	   LDI		r28, 0x00
	   LDI		r29, 0x01
	   LDI		r20, 0b01000000
	   ST		Y, r20
	   LDI		r20, 0b01111001
	   ST		Y+, r20
	   LDI		r20, 0b00100100
	   ST		Y+, r20
	   LDI		r20, 0b00110000
	   ST		Y+, r20
	   LDI		r20, 0b00011001
	   ST		Y+, r20
	   LDI		r20, 0b00010010
	   ST		Y+, r20
	   LDI		r20, 0b00000010
	   ST		Y+, r20
	   LDI		r20, 0b01111000
	   ST		Y+, r20
	   LDI		r20, 0b00000000
	   ST		Y+, r20
	   LDI		r20, 0b00011000
	   ST		Y+, r20
	   LDI		r28, 0x00
	   LDI		r29, 0x01
	   RET




