;
; LAB1.asm
;
; Created: 21/9/2024 20:51:52
; Author : Facundo
;


; Constantes de los estados
; Estado = r24
.equ STANDBY = 0
.equ AJUSTE1 = 2
.equ AJUSTE2 = 3
.equ AJUSTE3 = 4
.equ LAVADO1 = 5
.equ LAVADO2 = 6
.equ LAVADO3 = 7
.equ CENTRIFUGADO1 = 8
.equ CENTRIFUGADO2 = 9
.equ CENTRIFUGADO3 = 10
.equ SECADO1 = 11
.equ SECADO2 = 12
.equ SECADO3 = 13
.equ FINAL = 14

.equ F_CPU = 16000000
.equ baud = 9600
.equ bps = (F_CPU/16/baud) - 1

.org 0x00                     
    rjmp inicio              

inicio:
    ldi r16, 0xFF             
    out DDRB, r16             ; Configurar el puerto B como salida
    out DDRC, r16             ; Configurar el puerto C como salida
    ldi r16, 0xC0 
	out DDRD, r16			  ; Configurar el puerto D como entrada y salida
	ldi r16, 0x00
	out PORTD, r16			  ; Configurar los pines de entrada D en pull-down            
    out PORTB, r16            ; Apagar todos los LEDs
    out PORTC, r16            ; Apagar motor
	ldi r24, STANDBY		  ; Inicializar el estado en STANDBY

	ldi r16, HIGH(RAMEND)
	sts SPH, r16
	ldi r16, LOW(RAMEND)
	sts SPL, r16
	; Configuracion del USART
	ldi r16, LOW(bps)
	sts UBRR0L, r16
	ldi r16, HIGH(bps)
	sts UBRR0H, r16
	ldi r16, (1<<RXEN0) | (1<<TXEN0)
	sts UCSR0B, r16

main_loop:
	ldi r18, STANDBY
    cp r24, r18
	brne skip1
	rjmp estado_standby
skip1:
    ldi r18, AJUSTE1
    cp r24, r18
	brne skip2
	rjmp estado_ajuste1
skip2:
    ldi r18, AJUSTE2
    cp r24, r18
	brne skip3
	rjmp estado_ajuste2
skip3:
    ldi r18, AJUSTE3
    cp r24, r18
	brne skip4
	rjmp estado_ajuste3
skip4:
    ldi r18, LAVADO1
    cp r24, r18
	brne skip5
	rjmp estado_lavado1
skip5:
    ldi r18, LAVADO2
    cp r24, r18
	brne skip6
	rjmp estado_lavado2
skip6:
    ldi r18, LAVADO3
    cp r24, r18
	brne skip7
	rjmp estado_lavado3
skip7:
    ldi r18, CENTRIFUGADO1
    cp r24, r18
	brne skip8
	rjmp estado_centrifugado1
skip8:
    ldi r18, CENTRIFUGADO2
    cp r24, r18
	brne skip9
	rjmp estado_centrifugado2
skip9:
    ldi r18, CENTRIFUGADO3
    cp r24, r18
	brne skipA
	rjmp estado_centrifugado3
skipA:
    ldi r18, SECADO1
    cp r24, r18
	brne skipB
	rjmp estado_secado1
skipB:
    ldi r18, SECADO2
    cp r24, r18
	brne skipC
	rjmp estado_secado2
skipC:
    ldi r18, SECADO3
    cp r24, r18
	brne skipD
	rjmp estado_secado3
skipD:
    ldi r18, FINAL
    cp r24, r18
	brne skipE
	rjmp estado_final
skipE:
    rjmp main_loop

; Definir las rutinas para cada estado
estado_standby:
	rcall Enviar_Estado_Standby
	ldi	r16, 0x00
    out PORTB, r16              ; Apagar todos los LEDs
	out PORTD, r16
    cbi PORTC, 0                ; Apagar motor
	sbis PIND, 3				; Si Boton2 = 1, skip	
    rjmp main_loop
	ldi r24, AJUSTE1			; Nuevo estado actual
	RCALL Timer_1s				; Debounce time
	rjmp main_loop


estado_ajuste1:
	rcall Enviar_Estado_Ajuste
	ldi r16, 0x00
	out PORTB, r16
	out PORTD, r16
    sbi PORTB, 0				; Encender LED1
    sbi PORTB, 5                ; Encender LED6
	sbic PIND, 2			    ; Si Boton1 = 1 { 
	ldi r24, LAVADO1			;  ejecuta lavado
	sbic PIND, 2				;
	RCALL Timer_1s				;  Debounce time
	sbic PIND, 2				;
	rjmp main_loop				;  Vuele al main }
	sbis PIND, 3				; Si Boton2 = 1, Nuevo estado actual	
    rjmp main_loop
	ldi r24, AJUSTE2
	RCALL Timer_1s
	rjmp main_loop


estado_ajuste2:
	rcall Enviar_Estado_Ajuste
	ldi r16, 0x00
	out PORTD, r16
	out PORTB, r16
    sbi PORTB, 0              ; Encender LED1
    sbi PORTD, 6              ; Encender LED7
    sbic PIND, 2			 
	ldi r24, LAVADO2			
	sbic PIND, 2			
	RCALL Timer_1s			
	sbic PIND, 2			
	rjmp main_loop				
	sbis PIND, 3					
    rjmp main_loop
	ldi r24, AJUSTE3
	RCALL Timer_1s
	rjmp main_loop

estado_ajuste3:
	rcall Enviar_Estado_Ajuste
	ldi r16, 0x00
	out PORTB, r16
	out PORTD, r16
    sbi PORTB, 0              ; Encender LED1
    sbi PORTD, 7              ; Encender LED8
    sbic PIND, 2			   
	ldi r24, LAVADO3			
	sbic PIND, 2			
	RCALL Timer_1s				
	sbic PIND, 2				
	rjmp main_loop				
	sbis PIND, 3					
    rjmp main_loop
	ldi r24, AJUSTE1
	RCALL Timer_1s
	rjmp main_loop

estado_lavado1:
	rcall Enviar_Estado_Lavando
	ldi r20, 5				  ; Cantidad de ciclos
b1:
	sbis PIND, 4			  ; Sensor de puerta
	rjmp b1
	sbis PIND, 5			  ; Sensor agua
	rjmp b1
	ldi r16, 0x00
	out PORTD, r16
	out PORTB, r16
    sbi PORTC, 0              ; Encender motor
	cbi PORTC, 1
    sbi PORTB, 1              ; Encender LED2
    sbi PORTB, 5              ; Encender LED6
	rcall Timer_2s
	cbi PORTC, 0			  ; Apaga el motor
	cbi PORTC, 1
	rcall Timer_1s
	dec r20
	brne b1
	ldi r24, CENTRIFUGADO1
	rcall Timer_1s			  ; Tiempo de espera entre estados
    rjmp main_loop

estado_lavado2:
	rcall Enviar_Estado_Lavando
	ldi r20, 5				  ; Cantidad de ciclos
b2:
	sbis PIND, 4			  ; Sensor de puerta
	rjmp b2
	sbis PIND, 5			  ; Sensor agua
	rjmp b2
	ldi r16, 0x00
	out PORTB, r16
	out PORTD, r16
    sbi PORTC, 0              ; Encender motor
	cbi PORTC, 1
    sbi PORTB, 1              ; Encender LED2
    sbi PORTD, 6              ; Encender LED7
    rcall Timer_3s
	cbi PORTC, 0			  ; Apaga el motor
	cbi PORTC, 1
	rcall Timer_2s
	dec r20
	brne b2
	ldi r24, CENTRIFUGADO2
	rcall Timer_1s			  ; Tiempo de espera entre estados
    rjmp main_loop

estado_lavado3:
	rcall Enviar_Estado_Lavando
	ldi r20, 5				  ; Cantidad de ciclos
b3:
	sbis PIND, 4			  ; Sensor de puerta
	rjmp b3
	sbis PIND, 5			  ; Sensor agua
	rjmp b3
	ldi r16, 0x00
	out PORTD, r16
	out PORTB, r16
    sbi PORTC, 0              ; Encender motor
	cbi PORTC, 1
    sbi PORTB, 1              ; Encender LED2
    sbi PORTD, 7              ; Encender LED8
    rcall Timer_4s
	cbi PORTC, 0			  ; Apaga el motor
	cbi PORTC, 1
	rcall Timer_3s
	dec r20
	brne b3
	ldi r24, CENTRIFUGADO3
	rcall Timer_1s			  ; Tiempo de espera entre estados
    rjmp main_loop

estado_centrifugado1:
	rcall Enviar_Estado_Centrifugando
	ldi r16, 0x00
	out PORTB, r16
	out PORTD, r16
    sbi PORTC, 0              ; Encender motor
	cbi PORTC, 1
    sbi PORTB, 2              ; Encender LED3
    sbi PORTB, 5              ; Encender LED6
	rcall Timer_15s
	cbi PORTC, 0
	cbi PORTC, 1
	ldi r24, SECADO1
	rcall Timer_1s			  ; Tiempo de espera entre estados
    rjmp main_loop

estado_centrifugado2:
	rcall Enviar_Estado_Centrifugando
	ldi r16, 0x00
	out PORTD, r16
	out PORTB, r16
    sbi PORTC, 0              ; Encender motor
	cbi PORTC, 1
    sbi PORTB, 2              ; Encender LED3
    sbi PORTD, 6              ; Encender LED7
    rcall Timer_15s
	cbi PORTC, 0
	cbi PORTC, 1
	ldi r24, SECADO2
	rcall Timer_1s			  ; Tiempo de espera entre estados
    rjmp main_loop

estado_centrifugado3:
	rcall Enviar_Estado_Centrifugando
	ldi r16, 0x00
	out PORTB, r16
	out PORTD, r16
    sbi PORTC, 0              ; Encender motor
	cbi PORTC, 1
    sbi PORTB, 2              ; Encender LED3
    sbi PORTD, 7              ; Encender LED8
    rcall Timer_15s
	cbi PORTC, 0
	cbi PORTC, 1
	ldi r24, SECADO2
	rcall Timer_1s			  ; Tiempo de espera entre estados
    rjmp main_loop

estado_secado1:
	rcall Enviar_Estado_Secando
	ldi r16, 0x00
	out PORTD, r16
	out PORTB, r16
    sbi PORTC, 0              ; Encender motor
	cbi PORTC, 1
    sbi PORTB, 3              ; Encender LED4
    sbi PORTB, 5              ; Encender LED6
	rcall Timer_5s
	cbi PORTC, 0
	cbi PORTC, 1
	rcall Timer_3s
	cbi PORTC, 0
	sbi PORTC, 1
	rcall Timer_5s
	cbi PORTC, 0
	cbi PORTC, 1
	ldi r24, FINAL
    rjmp main_loop

estado_secado2:
	rcall Enviar_Estado_Secando
	ldi r16, 0x00
	out PORTD, r16
	out PORTB, r16
    sbi PORTC, 0              ; Encender motor
	cbi PORTC, 1
    sbi PORTB, 3              ; Encender LED4
    sbi PORTD, 6              ; Encender LED7
    rcall Timer_7s
	cbi PORTC, 0
	cbi PORTC, 1
	rcall Timer_5s
	cbi PORTC, 0
	sbi PORTC, 1
	rcall Timer_7s
	cbi PORTC, 0
	cbi PORTC, 1
	ldi r24, FINAL
    rjmp main_loop

estado_secado3:
	rcall Enviar_Estado_Secando
	ldi r16, 0x00
	out PORTB, r16
	out PORTD, r16
    sbi PORTC, 0
	cbi PORTC, 1              ; Encender motor
    sbi PORTB, 3              ; Encender LED4
    sbi PORTD, 7              ; Encender LED8
    rcall Timer_9s
	cbi PORTC, 0
	cbi PORTC, 1
	rcall Timer_7s
	cbi PORTC, 0
	sbi PORTC, 1
	rcall Timer_9s
	cbi PORTC, 0
	cbi PORTC, 1
	ldi r24, FINAL
    rjmp main_loop

estado_final:
	rcall Enviar_Estado_Completado
	ldi r16, 0x00
	out PORTD, r16
	out PORTB, r16
    cbi PORTC, 0              ; Apagar motor
    sbi PORTB, 4              ; Encender LED5
	sbic PIND, 4
	rjmp estado_final
	ldi r24, STANDBY
    rjmp main_loop

Timer_1s:
	ldi r16, 250
	ldi r17, 250
	ldi r18, 85
L1:
	dec r16
	brne L1
	dec r17
	brne L1
	dec r18
	brne L1
	RET
	
Timer_2s:
	rcall Timer_1s
	rcall Timer_1s
	ret

Timer_3s:
	rcall Timer_1s
	rcall Timer_2s
	ret

Timer_4s:
	rcall Timer_2s
	rcall Timer_2s
	ret

Timer_5s:
	rcall Timer_4s
	rcall Timer_1s
	ret

Timer_7s:
	rcall Timer_5s
	rcall Timer_2s
	ret

Timer_9s:
	rcall Timer_7s
	rcall Timer_2s
	ret

Timer_15s:
	rcall Timer_7s
	rcall Timer_7s
	rcall Timer_1s
	ret

Timer_18s:
	rcall Timer_15s
	rcall Timer_3s
	RET

Timer_21s:
	rcall Timer_15s
	rcall Timer_5s
	rcall Timer_1s
	ret

Enviar_Estado_Standby:
	ldi r16, 'S'
    rcall Enviar
	ldi r16, 'T'
    rcall Enviar
	ldi r16, 'A'
    rcall Enviar
	ldi r16, 'N'
    rcall Enviar
	ldi r16, 'D'
    rcall Enviar
	ldi r16, 'B'
    rcall Enviar
	ldi r16, 'Y'
    rcall Enviar
	ldi r16, 0x0A
    rcall Enviar
	ret

Enviar_Estado_Ajuste:
	ldi r16, 'A'
    rcall Enviar
	ldi r16, 'J'
    rcall Enviar
	ldi r16, 'U'
    rcall Enviar
	ldi r16, 'S'
    rcall Enviar
	ldi r16, 'T'
    rcall Enviar
	ldi r16, 'E'
    rcall Enviar
	ldi r16, 0x0A
    rcall Enviar
	ret

Enviar_Estado_Lavando:
	ldi r16, 'L'
    rcall Enviar
	ldi r16, 'A'
    rcall Enviar
	ldi r16, 'V'
    rcall Enviar
	ldi r16, 'A'
    rcall Enviar
	ldi r16, 'N'
    rcall Enviar
	ldi r16, 'D'
    rcall Enviar
	ldi r16, 'O'
    rcall Enviar
	ldi r16, 0x0A
    rcall Enviar
	ret

Enviar_Estado_Centrifugando:
	ldi r16, 'C'
    rcall Enviar
	ldi r16, 'E'
    rcall Enviar
	ldi r16, 'N'
    rcall Enviar
	ldi r16, 'T'
    rcall Enviar
	ldi r16, 'R'
    rcall Enviar
	ldi r16, 'I'
    rcall Enviar
	ldi r16, 'F'
    rcall Enviar
	ldi r16, 'U'
    rcall Enviar
	ldi r16, 'G'
    rcall Enviar
	ldi r16, 'A'
    rcall Enviar
	ldi r16, 'N'
    rcall Enviar
	ldi r16, 'D'
    rcall Enviar
	ldi r16, 'O'
    rcall Enviar
	ldi r16, 0x0A
    rcall Enviar
	ret

Enviar_Estado_Secando:
	ldi r16, 'S'
    rcall Enviar
	ldi r16, 'E'
    rcall Enviar
	ldi r16, 'C'
    rcall Enviar
	ldi r16, 'A'
    rcall Enviar
	ldi r16, 'N'
    rcall Enviar
	ldi r16, 'D'
    rcall Enviar
	ldi r16, 'O'
    rcall Enviar
	ldi r16, 0x0A
    rcall Enviar
	ret

Enviar_Estado_Completado:
	ldi r16, 'C'
    rcall Enviar
	ldi r16, 'O'
    rcall Enviar
	ldi r16, 'M'
    rcall Enviar
	ldi r16, 'P'
    rcall Enviar
	ldi r16, 'L'
    rcall Enviar
	ldi r16, 'E'
    rcall Enviar
	ldi r16, 'T'
    rcall Enviar
	ldi r16, 'A'
    rcall Enviar
	ldi r16, 'D'
    rcall Enviar
	ldi r16, 'O'
    rcall Enviar
	ldi r16, 0x0A
    rcall Enviar
	ret

Enviar:
    lds r17, UCSR0A
    sbrs r17, UDRE0
    rjmp Enviar
    sts UDR0, r16     
    ret          
