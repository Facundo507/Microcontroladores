#define F_CPU 16000000

#include <avr/io.h>
#include <avr/sleep.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#define LEDS_PORT PORTB
#define LEDS_DDR DDRB
#define LEDS_MASK 0x1F 

volatile uint8_t state = 0; 

void setup() {
	LEDS_DDR |= LEDS_MASK;

	LEDS_PORT &= ~LEDS_MASK;

	DDRD &= ~(1 << PD2);
	PORTD |= (1 << PD2);

	EICRA |= (1 << ISC01);  // ISC01 = 1, ISC00 = 0: flanco de bajada
	EIMSK |= (1 << INT0);   // Habilitar interrupción externa INT0

	sei();
}

void sleep_mode_config(uint8_t mode) {
	set_sleep_mode(mode);
	sleep_enable();
	sleep_cpu();  
	sleep_disable(); 
}

void led_on() {
	LEDS_PORT |= LEDS_MASK; 
}

void led_off() {
	LEDS_PORT &= ~LEDS_MASK;
}

ISR(INT0_vect) {
	state++;
	if (state > 3) {
		state = 0;
	}
}

int main(void) {
	setup();

	while (1) {
		switch (state) {
			case 0:
			// Estado 0: Encender LEDs por 1 segundo y luego apagarlos
			led_on();
			_delay_ms(1000);
			led_off();
			break;

			case 1:
			// Estado 1: Entrar en modo Idle durante 30 segundos
			sleep_mode_config(SLEEP_MODE_IDLE);
			break;

			case 2:
			// Estado 2: Entrar en modo Power-down durante 30 segundos
			sleep_mode_config(SLEEP_MODE_PWR_DOWN);
			break;

			case 3:
			// Estado 3: Entrar en modo Power-save durante 30 segundos
			sleep_mode_config(SLEEP_MODE_PWR_SAVE);
			break;
		}
	}
}