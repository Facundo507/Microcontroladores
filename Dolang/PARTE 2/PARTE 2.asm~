.include "m328Pdef.inc"

.cseg
.org 0x0000

    ldi r16, 0xFF
    out DDRD, r16

loop:
    out PORTD, r16
    rcall DELAY

    lsl r16
    brne loop

    out PORTD, r16
    rcall DELAY
    ldi r16, 0xFF
    rjmp loop


DELAY:
    ldi r18, 0x2F
delay_2:
    ldi r19, 0xFF
delay_3:
    ldi r20, 0xFF
delay_4:
    dec r20
    brne delay_4
    dec r19
    brne delay_3
    dec r18
    brne DELAY_2
    ret


