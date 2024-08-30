.include "m328pdef.inc"

.cseg
.org 0x00

    ldi r16, 0xFF
    out DDRD, r16

loop:
    out portD, r16
    rcall delay

    rol r16
    brne loop

    ldi r16, 0xFF
    rjmp loop


delay:
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
    brne delay_2

    ret

