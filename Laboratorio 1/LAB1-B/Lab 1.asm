.include "m328pdef.inc"  ; Incluye las definiciones del ATmega328P

        LDI     r16, LOW(RAMEND)
        OUT     SPL, r16
        LDI     r16, HIGH(RAMEND)
        OUT     SPH, r16

        LDI     r16, 0xFF
        OUT     DDRD, r16
        LDI     r16, 0x0F
        OUT     DDRB, r16

mostrar_letra_B:
        LDI     r16, 0b00000111
        OUT     PORTD, r16
        LDI     r17, 0b00000001
        OUT     PORTB, r17
        CALL    delay

        LDI     r16, 0b01110111
        OUT     PORTD, r16
        LDI     r17, 0b00000010
        OUT     PORTB, r17
        CALL    delay

        LDI     r16, 0b01110111
        OUT     PORTD, r16
        LDI     r17, 0b00000011
        OUT     PORTB, r17
        CALL    delay

        LDI     r16, 0b00000111
        OUT     PORTD, r16
        LDI     r17, 0b00001000
        OUT     PORTB, r17
        CALL    delay

        LDI     r16, 0b01110111
        OUT     PORTD, r16
        LDI     r17, 0b00000001
        OUT     PORTB, r17
        CALL    delay

        LDI     r16, 0b01110111
        OUT     PORTD, r16
        LDI     r17, 0b00000010
        OUT     PORTB, r17
        CALL    delay

        LDI     r16, 0b00000111
        OUT     PORTD, r16
        LDI     r17, 0b00000011
        OUT     PORTB, r17
        CALL    delay

        LDI     r16, 0b00000111
        OUT     PORTD, r16
        LDI     r17, 0b00001000
        OUT     PORTB, r17
        CALL    delay

        RJMP    mostrar_letra_B

delay:
        LDI     r22, 0xFF
delay_loop1:
        LDI     r23, 0xFF
delay_loop2:
        DEC     r23
        BRNE    delay_loop2
        DEC     r22
        BRNE    delay_loop1
        RET

