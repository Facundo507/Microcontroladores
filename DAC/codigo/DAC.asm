
.include "m328pdef.inc"

.cseg

.org 0x100  ; Dirección de memoria para la LUT
lut_data:
    .db 0x49, 0x4A, 0x4B, 0x4B, 0x4A, 0x49, 0x49, 0x49, 0x49, 0x48, 0x47, 0x45, 0x44, 0x43, 0x43, 0x43 ,0x44, 0x44, 0x43, 0x41, 0x3E, 0x3D, 0x3B, 0x39, 0x38, 0x37, 0x37, 0x36, 0x36, 0x36, 0x37, 0x37, 0x37, 0x37, 0x37, 0x37, 0x37, 0x36, 0x35, 0x33, 0x32, 0x31, 0x31, 0x34, 0x3D, 0x4D, 0x65, 0x84, 0xA9, 0xCF, 0xEE, 0xFF, 0xFE, 0xEA, 0xC6, 0x9A, 0x6D, 0x44, 0x25, 0x11, 0x05, 0x00, 0x01, 0x06, 0x0D, 0x14, 0x1C, 0x24, 0x2D, 0x34, 0x37, 0x39, 0x3D, 0x40, 0x41, 0x42, 0x43, 0x44, 0x44, 0x45, 0x46, 0x47, 0x47, 0x47, 0x47, 0x47, 0x47, 0x47, 0x48, 0x48, 0x48, 0x49, 0x49, 0x4A, 0x4B, 0x4B, 0x4C, 0x4D, 0x4E, 0x4F, 0x50, 0x51, 0x52, 0x53, 0x54, 0x56, 0x58, 0x5B, 0x5D, 0x60, 0x64, 0x66, 0x68, 0x6B, 0x6D, 0x70, 0x73, 0x76, 0x79, 0x7B, 0x7D, 0x7E, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7E, 0x7D, 0x7C, 0x79, 0x77, 0x74, 0x71, 0x6D, 0x6A, 0x62, 0x5F, 0x5C, 0x59, 0x56, 0x54, 0x51, 0x4F, 0x4D, 0x4C, 0x4A, 0x49, 0x48, 0x47, 0x43, 0x43, 0x43, 0x44, 0x44, 0x44, 0x45, 0x45, 0x45, 0x45, 0x45, 0x45, 0x44, 0x44, 0x43, 0x43, 0x43, 0x43, 0x42, 0x42, 0x42, 0x41, 0x41, 0x41, 0x41, 0x41, 0x41, 0x41, 0x40, 0x40, 0x3F, 0x3F, 0x40, 0x40, 0x41, 0x41, 0x41, 0x41, 0x41, 0x41, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x41, 0x41, 0x41, 0x42, 0x43, 0x44, 0x45, 0x47, 0x48, 0x49

.end = lut_data + 128

.org 0x0000
    RJMP inicio
inicio:
       LDI R16, HIGH(RAMEND)    ; Activamos el SP
       OUT SPH, R16
       LDI R16, LOW(RAMEND)
       OUT SPL, R16

           ldi r16, 0xFF        ; Dejamos el puerto D como salida
           OUT DDRD, R16

       LDI R18, 0X01
       MOV YH, R18
       LDI R18, 0X00
       MOV YL, R18

       LDI YH, HIGH(lut_data)
       LDI YL, LOW(lut_data)

BUCLE:
      LD R17, Y
      OUT PORTD, R17
      CALL SEGUNDO
      LD R17, Y+

      ; Comparamos si hemos llegado al final de la LUT
      LDI R16, HIGH(end)
      CPYH R16
      BRNE CONTINUAR

      LDI R16, LOW(end)
      CPYL R16
      BRNE CONTINUAR

      ; Si hemos llegado al final de la LUT, reiniciamos Y
      LDI YH, HIGH(lut_data)
      LDI YL, LOW(lut_data)

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


