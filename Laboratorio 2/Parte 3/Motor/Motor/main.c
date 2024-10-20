/*
 * main.c
 *
 * Created: 10/20/2024 3:14:28 PM
 *  Author: Facundo
 */ 

#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>

#define BAUD 9600
#define bps F_CPU/16/BAUD-1

void setupUSART(unsigned int ubrr) {
	UBRR0H = (unsigned char)(ubrr >> 8);
	UBRR0L = (unsigned char)ubrr;
	UCSR0B = (1 << RXEN0) | (1 << TXEN0);
	UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);
}

void EnviarUSART(unsigned char data) {
	while (!(UCSR0A & (1 << UDRE0)));
	UDR0 = data;
}

void USARTPrint(const char *str) {
	while (*str) {
		EnviarUSART(*str++);
	}
}

uint16_t readAdc(uint8_t channel) {
	ADMUX = (ADMUX & 0xF0) | (channel & 0x0F);
	ADCSRA |= (1 << ADSC);
	while (ADCSRA & (1 << ADSC));
	return ADC;
}

void setupAdc() {
	ADMUX |= (1 << REFS0);
	ADMUX &= ~(1 << ADLAR);
	ADCSRA = (1 << ADEN) | (1 << ADPS2) | (1 << ADPS1);
}

void setupPwm() {
	DDRB |= (1 << PB1) | (1 << PB2);
	TCCR1A |= (1 << WGM10) | (1 << COM1A1) | (1 << COM1B1); // Fast PWM de 8 bits
	TCCR1B |= (1 << WGM12) | (1 << CS11); // Preescaler = 8
}

int main(void) {
	setupUSART(bps);
	setupAdc();
	setupPwm();
	
	while (1) {
		uint16_t valorPot1 = readAdc(0);
		uint16_t valorPot2 = readAdc(1);
		
		uint8_t valorPot1Ajustado = (valorPot1/4) - 1;
		uint8_t valorPot2Ajustado = (valorPot2/4) - 1;
		
		if(valorPot1Ajustado == 255){ // Estas condiciones son para evitar que cuando el sensor este al minimo se detecte como 255
			valorPot1Ajustado = 0;
		}
		if(valorPot2Ajustado == 255){
			valorPot2Ajustado = 0;
		}
		
		if (valorPot2 < valorPot1) {
			OCR1B = 0; 
			OCR1A = valorPot1Ajustado; // PWM horario
		} else if (valorPot2 > valorPot1) {
			OCR1A = 0;                 
			OCR1B = valorPot2Ajustado;  // PWM antihorario
		} else {
			OCR1A = 0; // Detener el motor si las posiciones son iguales
			OCR1B = 0;
		}
		

		char Mensaje[50];
		snprintf(Mensaje, sizeof(Mensaje), "Potenciometro 1: %u\n", valorPot1);
		USARTPrint(Mensaje);
		snprintf(Mensaje, sizeof(Mensaje), "Potenciometro 2: %u\n", valorPot2);
		USARTPrint(Mensaje);
		snprintf(Mensaje, sizeof(Mensaje), "PWM horario: %u\n", OCR1A);
		USARTPrint(Mensaje);
		snprintf(Mensaje, sizeof(Mensaje), "PWM antihorario: %u\n", OCR1B);
		USARTPrint(Mensaje);
		if(valorPot2 < valorPot1){
			snprintf(Mensaje, sizeof(Mensaje), "Sentido Horario\n\n");
			USARTPrint(Mensaje);
		} else {
			snprintf(Mensaje, sizeof(Mensaje), "Sentido Antihorario\n\n");
			USARTPrint(Mensaje);
		}
		_delay_ms(100);
	}
}