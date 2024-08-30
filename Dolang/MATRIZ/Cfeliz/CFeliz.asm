.include "m328pdef.inc" ; Define device ATmega328P

.cseg
.org 0x00

ldi r16, 0xFF
out DDRD, r16
out DDRB, r16

loop:
     ldi r16, 0x07
     out PORTB, r16
     ldi r16, 0xFF
     out PORTD, r16
     rcall delay

     ldi r16, 0x06
     out PORTB, r16
     ldi r16, 0b11011011
     out PORTD, r16
     rcall delay

     ldi r16, 0x05
     out PORTB, r16
     ldi r16, 0b11011011
     out PORTD, r16
     rcall delay

     ldi r16, 0x04
     out PORTB, r16
     ldi r16, 0b11011011
     out PORTD, r16
     rcall delay

     ldi r16, 0x03
     out PORTB, r16
     ldi r16, 0xFF
     out PORTD, r16
     rcall delay

     ldi r16, 0x02
     out PORTB, r16
     ldi r16, 0b10111101
     out PORTD, r16
     rcall delay

     ldi r16, 0x01
     out PORTB, r16
     ldi r16, 0b11000011
     out PORTD, r16
     rcall delay

     ldi r16, 0x00
     out PORTB, r16
     ldi r16, 0xFF
     out PORTD, r16
     rcall delay
     rjmp loop

delay:
      ldi r20, 0xFF
      delay_2:
      dec r20
      brne delay_2
      ret
