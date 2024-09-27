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

.org 0x00                     
    rjmp inicio              

inicio:
    ldi r16, 0xFF             
    out DDRB, r16             ; Configurar el puerto B como salida
    out DDRC, r16             ; Configurar el puerto C como salida
    ldi r16, 0x00 
	out DDRD, r16			  ; Configurar el puerto D como entrada
	ldi r16, 0xFF
	out PORTD, r16			  ; Configurar todos los pines de D en pull-up            
    out PORTB, r16            ; Apagar todos los LEDs
    out PORTC, r16            ; Apagar motor
	ldi r24, STANDBY		  ; Inicializar el estado en STANDBY

	ldi r16, HIGH(RAMEND)
	sts SPH, r16
	ldi r16, LOW(RAMEND)
	sts SPL, r16

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
	ldi	r16, 0x00
    out PORTB, r16              ; Apagar todos los LEDs
    cbi PORTC, 0                ; Apagar motor
	sbis PIND, 3				; Si Boton2 = 1, skip	
    rjmp main_loop
	ldi r24, AJUSTE1			; Nuevo estado actual
	RCALL Timer_2s				; Debounce time
	rjmp main_loop


estado_ajuste1:
	ldi r16, 0x00
	out PORTB, r16
    sbi PORTB, 0				; Encender LED1
    sbi PORTB, 5                ; Encender LED6
	sbic PIND, 2			    ; Si Boton1 = 1 { 
	ldi r24, LAVADO1			;  ejecuta lavado
	sbic PIND, 2				;
	RCALL Timer_2s				;  Debounce time
	sbic PIND, 2				;
	rjmp main_loop				;  Vuele al main }
	sbis PIND, 3				; Si Boton2 = 1, Nuevo estado actual	
    rjmp main_loop
	ldi r24, AJUSTE2
	RCALL Timer_2s
	rjmp main_loop


estado_ajuste2:
	ldi r16, 0x00
	out PORTB, r16
    sbi PORTB, 0              ; Encender LED1
    sbi PORTB, 6              ; Encender LED7
    sbic PIND, 2			 
	ldi r24, LAVADO2			
	sbic PIND, 2			
	RCALL Timer_2s			
	sbic PIND, 2			
	rjmp main_loop				
	sbis PIND, 3					
    rjmp main_loop
	ldi r24, AJUSTE3
	RCALL Timer_2s
	rjmp main_loop

estado_ajuste3:
	ldi r16, 0x00
	out PORTB, r16
    sbi PORTB, 0              ; Encender LED1
    sbi PORTB, 7              ; Encender LED8
    sbic PIND, 2			   
	ldi r24, LAVADO3			
	sbic PIND, 2			
	RCALL Timer_2s				
	sbic PIND, 2				
	rjmp main_loop				
	sbis PIND, 3					
    rjmp main_loop
	ldi r24, AJUSTE1
	RCALL Timer_2s
	rjmp main_loop

estado_lavado1:
	ldi r17, 5				  ; Cantidad de ciclos
b1:
	sbis PIND, 4			  ; Sensor de puerta
	rjmp b1
	sbis PIND, 5			  ; Sensor agua
	rjmp b1
	ldi r16, 0x00
	out PORTB, r16
    sbi PORTC, 0              ; Encender motor
    sbi PORTB, 1              ; Encender LED2
    sbi PORTB, 5              ; Encender LED6
	rcall Timer_2s
	cbi PORTC, 0			  ; Apaga el motor
	rcall Timer_1s
	dec r17
	brne b1
	ldi r24, CENTRIFUGADO1
	rcall Timer_2s			  ; Tiempo de espera entre estados
    rjmp main_loop

estado_lavado2:
	ldi r17, 5				  ; Cantidad de ciclos
b2:
	sbis PIND, 4			  ; Sensor de puerta
	rjmp b2
	sbis PIND, 5			  ; Sensor agua
	rjmp b2
	ldi r16, 0x00
	out PORTB, r16
    sbi PORTC, 0              ; Encender motor
    sbi PORTB, 1              ; Encender LED2
    sbi PORTB, 6              ; Encender LED7
    rcall Timer_3s
	cbi PORTC, 0			  ; Apaga el motor
	rcall Timer_2s
	dec r17
	brne b2
	ldi r24, CENTRIFUGADO2
	rcall Timer_2s			  ; Tiempo de espera entre estados
    rjmp main_loop

estado_lavado3:
	ldi r17, 5				  ; Cantidad de ciclos
b3:
	sbis PIND, 4			  ; Sensor de puerta
	rjmp b3
	sbis PIND, 5			  ; Sensor agua
	rjmp b3
	ldi r16, 0x00
	out PORTB, r16
    sbi PORTC, 0              ; Encender motor
    sbi PORTB, 1              ; Encender LED2
    sbi PORTB, 7              ; Encender LED8
    rcall Timer_4s
	cbi PORTC, 0			  ; Apaga el motor
	rcall Timer_3s
	dec r17
	brne b3
	ldi r24, CENTRIFUGADO3
	rcall Timer_2s			  ; Tiempo de espera entre estados
    rjmp main_loop

estado_centrifugado1:
	ldi r16, 0x00
	out PORTB, r16
    sbi PORTC, 0              ; Encender motor
    sbi PORTB, 2              ; Encender LED3
    sbi PORTB, 5              ; Encender LED6
	rcall Timer_15s
	cbi PORTC, 0
	ldi r24, SECADO1
	rcall Timer_2s			  ; Tiempo de espera entre estados
    rjmp main_loop

estado_centrifugado2:
	ldi r16, 0x00
	out PORTB, r16
    sbi PORTC, 0              ; Encender motor
    sbi PORTB, 2              ; Encender LED3
    sbi PORTB, 6              ; Encender LED7
    rcall Timer_15s
	cbi PORTC, 0
	ldi r24, SECADO2
	rcall Timer_2s			  ; Tiempo de espera entre estados
    rjmp main_loop

estado_centrifugado3:
	ldi r16, 0x00
	out PORTB, r16
    sbi PORTC, 0              ; Encender motor
    sbi PORTB, 2              ; Encender LED3
    sbi PORTB, 7              ; Encender LED8
    rcall Timer_15s
	cbi PORTC, 0
	ldi r24, SECADO2
	rcall Timer_2s			  ; Tiempo de espera entre estados
    rjmp main_loop

estado_secado1:
	ldi r16, 0x00
	out PORTB, r16
    sbi PORTC, 0              ; Encender motor
    sbi PORTB, 3              ; Encender LED4
    sbi PORTB, 5              ; Encender LED6
	rcall Timer_5s
	cbi PORTC, 0
	rcall Timer_3s
	sbi PORTC, 0
	rcall Timer_3s
	cbi PORTC, 0
	ldi r24, FINAL
    rjmp main_loop

estado_secado2:
	ldi r16, 0x00
	out PORTB, r16
    sbi PORTC, 0              ; Encender motor
    sbi PORTB, 3              ; Encender LED4
    sbi PORTB, 6              ; Encender LED7
    rcall Timer_7s
	cbi PORTC, 0
	rcall Timer_5s
	sbi PORTC, 0
	rcall Timer_7s
	cbi PORTC, 0
	ldi r24, FINAL
    rjmp main_loop

estado_secado3:
	ldi r16, 0x00
	out PORTB, r16
    sbi PORTC, 0              ; Encender motor
    sbi PORTB, 3              ; Encender LED4
    sbi PORTB, 7              ; Encender LED8
    rcall Timer_9s
	cbi PORTC, 0
	rcall Timer_7s
	sbi PORTC, 0
	rcall Timer_9s
	cbi PORTC, 0
	ldi r24, FINAL
    rjmp main_loop

estado_final:
    cbi PORTC, 0              ; Apagar motor
    sbi PORTB, 4              ; Encender LED5
	sbic PIND, 4
	rjmp estado_final
	ldi r24, STANDBY
    rjmp main_loop

Timer_1s:
	ldi r16, 140
	ldi r17, 43
	ldi r18, 7
L1:
	dec r16
	brne L1
	dec r17
	brne L1
	dec r18
	brne L1
	RET
	
Timer_2s:
	ldi r16, 119
	ldi r17, 38
	ldi r18, 20
L2:
	dec r16
	brne L2
	dec r17
	brne L2
	dec r18
	brne L2
	RET

Timer_3s:
	ldi r16, 233
	ldi r17, 81
	ldi r18, 4
L3:
	dec r16
	brne L3
	dec r17
	brne L3
	dec r18
	brne L3
	RET

Timer_4s:
	ldi r16, 89
	ldi r17, 49
	ldi r18, 41
L4:
	dec r16
	brne L4
	dec r17
	brne L4
	dec r18
	brne L4
	RET

Timer_5s:
	ldi r16, 152
	ldi r17, 47
	ldi r18, 25
L5:
	dec r16
	brne L5
	dec r17
	brne L5
	dec r18
	brne L5
	RET

Timer_7s:
	ldi r16, 193
	ldi r17, 36
	ldi r18, 31
L7:
	dec r16
	brne L7
	dec r17
	brne L7
	dec r18
	brne L7
	RET

Timer_9s:
	ldi r16, 177
	ldi r17, 69
	ldi r18, 22
L9:
	dec r16
	brne L9
	dec r17
	brne L9
	dec r18
	brne L9
	RET

Timer_15s:
	ldi r16, 228
	ldi r17, 66
	ldi r18, 25
L15:
	dec r16
	brne L15
	dec r17
	brne L15
	dec r18
	brne L15
	RET

Timer_18s:
	ldi r16, 150
	ldi r17, 150
	ldi r18, 20
L18:
	dec r16
	brne L18
	dec r17
	brne L18
	dec r18
	brne L18
	RET

Timer_21s:
	ldi r16, 173
	ldi r17, 71
	ldi r18, 47
L21:
	dec r16
	brne L21
	dec r17
	brne L21
	dec r18
	brne L21
	RET