/*
 * main.c
 *
 * Created: 11/3/2024 9:46:08 PM
 *  Author: dasil
 */ 
#define F_CPU 1000000UL
#include <xc.h>
#include <avr/io.h>

#include <util/delay.h>
#include <avr/interrupt.h>
#include "PWM.h"


#define RelojX PB3
#define DirX PB4
#define EnableX PB5

#define Solenoide PC0

#define RelojY PC3
#define DirY PC4
#define EnableY PC5

#define Limite_Y_A PD2
#define Limite_Y_D PD3

#define LED PD5


void UART_init(unsigned int ubrr) {
	// Configurar el valor de UBRR (Baud Rate Register)
	UBRR0H = (unsigned char)(ubrr >> 8);
	UBRR0L = (unsigned char)ubrr;
	UCSR0B = (1 << RXEN0) | (1 << TXEN0);
	UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);
}

void UART_sendChar(char data) {
	while (!(UCSR0A & (1 << UDRE0))){}
	UDR0 = data;
}

char UART_receiveChar(void) {
	while (!(UCSR0A & (1 << RXC0))){}
	return UDR0;
}

void UART_sendString(const char *str) {
	while (*str) {
		UART_sendChar(*str++);
	}
}

void int0_init(){
	DDRD &= ~(1 << PD2); //PD2 como entrada (INT0)
	//PORTD |= (1 << PD2); //Resistencia pull-up
	
	// Condiguracion interrupcion INT0 flanco de subida.
	
	EICRA |= (1 << ISC01);
	EICRA |= (1 << ISC00);
	
	EIMSK |= (1 << INT0);
	sei();
}

ISR(INT0_vect) {


}

int main(void) {
	UART_init(103);
	DDRB |= (1 << RelojX) | (1 << DirX) | (1 << EnableX);
	DDRC |= (1 << Solenoide) | (1 << RelojY) | (1 << DirY) | (1 << EnableY);
	DDRD |= (1 << LED);
	SetupPWM();
	
	int0_init();             // Inicializa la interrupción INT0
	while (1) {
		PORTC |= (1 << EnableY);
		PORTB |= (1 << EnableX) | (1 << DirX);
		PORTC |= (1 << EnableY) | (1 << DirY);
		PORTC |= ((1 << Solenoide));
		
		_delay_ms(500);
		
		PORTB &= ~(1 << EnableX);
		PORTC &= ~(1 << EnableY);
		
		
		_delay_ms(3000);
		//PORTC |= ((1 << EnableX));
	}
}