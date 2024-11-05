#define F_CPU 1000000UL
#include <xc.h>
#include <avr/io.h>


void SetupPWM(void){
	
	
	TCCR2A = (1 << COM2A1) | (1 << WGM21) | (1 << WGM20);  //PWM no invertido en OC2A y fast pwm
	TCCR2B = (1 << CS21) | (1 << CS20)  ; //prescaler 64
	OCR2A = 127;
	
}