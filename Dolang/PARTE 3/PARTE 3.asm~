.include "m328pdef.inc"

.cseg
.org 0x00

.equ DDRD_ADDR = 0x0A   ; Dirección del DDRD
.equ PORTD_ADDR = 0x0B  ; Dirección del PORTD


ldi r16, 0xFF
out DDRD_ADDR, r16

loop:
    ldi r16, 0xFF        ;0xFF (11111111)
    out PORTD_ADDR, r16
    rcall delay

    ldi r16, 0x7E        ;0x7E (01111110)
    out PORTD_ADDR, r16
    rcall delay

    ldi r16, 0xBD        ;0xBD (10111101)
    out PORTD_ADDR, r16
    rcall delay

    ldi r16, 0xDB        ;0xDB (11011011)
    out PORTD_ADDR, r16
    rcall delay

    ldi r16, 0xE7        ;0xE7 (11100111)
    out PORTD_ADDR, r16
    rcall delay

    ldi r16, 0xDB        ;0xDB (11011011)
    out PORTD_ADDR, r16
    rcall delay

    ldi r16, 0xBD        ;0xBD (10111101)
    out PORTD_ADDR, r16
    rcall delay

    ldi r16, 0x7E        ;0xFD (01111110)
    out PORTD_ADDR, r16
    rcall delay

    rjmp loop

delay:
    ldi r18, 0x2F
delay_2:
    ldi r17, 0xFF
delay_3:
    ldi r20, 0xFF
delay_4:
    dec r20
    brne delay_4
    dec r17
    brne delay_3
    dec r18
    brne delay_2
    ret
