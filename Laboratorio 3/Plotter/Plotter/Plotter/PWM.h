#define F_CPU 1000000UL
#include <xc.h>
#include <avr/io.h>


void SetupPWM(void){
	DDRB |= (1 << PB1) | (1 << PB2) | (1 << PB3);
	TCCR1A = (1 << COM1A1) | (1 << COM1B1) | (1 << WGM11);
	TCCR1B = (1 << WGM13) | (1 << WGM12) | (0 << CS12) | (1 << CS11) | (1 << CS10);
	// los 3 menos significativos configuran el prescaler, en este
	// caso configurado en 1024, pin 3 y 4 configuracion del WGM1 para hacerlo en FASTPWM
	//
	ICR1 = 4999; // Valor del top, este valor con el prescaler de 64 nos da una frecuenca de 1736,11hz, el más proximo a 1725hz.
	OCR1A = 2500; // Valor del Ciclo útil
	OCR1B = 1250;
	
	TCCR2A = (1 << COM2A1) | (1 << WGM21) | (1 << WGM20);  //PWM no invertido en OC2A y fast pwm
	TCCR2B = (1 << CS22); //prescaler 64
	OCR2A = 127;
	
}