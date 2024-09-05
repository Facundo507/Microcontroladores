.include "m328pdef.inc" ; Define device ATmega328P
.ORG   0X0000
RJMP     Inicio
.ORG     0x0002
RJMP     RSI_0
.ORG     0x0004
RJMP     RSI_1

Inicio:
       SEI
       LDI      R16, 0xFF
       OUT      DDRC, R16
       OUT      DDRB, R16
       LDI      R17, 0b0010010

       ;activamos los pull-up en los pines de INT0 e INT1
       SBI      PORTD, 2
       SBI      PORTD, 3
       LDI R16, HIGH(RAMEND)   ; Carga la parte alta de la dirección de la pila en R16
       OUT SPH, R16            ; Configura el registro SPH
       LDI R16, LOW(RAMEND)    ; Carga la parte baja de la dirección de la pila en R16
       OUT SPL, R16



       ;configuramos las interrupciones
       LDI      R16, 0X03
       OUT      EIMSK, R16       ;habilita las interrupciones INT0 e INT1
       LDI      R16, 0x0F
       STS      EICRA, R19       ; configura flancos de subida

Wait:
       OUT PORTC, r17
       SBI PORTB, 0
       CALL Mseg
       CBI PORTB, 0
       CALL Mseg
       RJMP Wait

RSI_0:
       RJMP Incremento
       RET

RSI_1:
       RJMP Decremento
       RET

Incremento:

Decremento:

Mseg:
       ldi  r21, 21
       ldi  r22, 75
       ldi  r23, 189
L1:
       dec  r23
       brne L1
       dec  r22
       brne L1
       dec  r21
       brne L1
       nop
       RET
       
