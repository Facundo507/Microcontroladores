#define F_CPU 16000000UL // Frecuencia del CPU

#include <avr/io.h>
#include <util/delay.h>

#define BAUD 9600 // 105us para 9600
#define MY_UBRR F_CPU/16/BAUD-1

void USART_Init() {
	UBRR0H = (MY_UBRR >> 8);
	UBRR0L = MY_UBRR;
	UCSR0B = (1 << RXEN0) | (1 << TXEN0);
	UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);
}

void USART_Transmit(unsigned char data) {
	while (!(UCSR0A & (1 << UDRE0)));
	UDR0 = data;
}

unsigned char USART_Receive(void) {
	while (!(UCSR0A & (1 << RXC0)));
	return UDR0;
}

int main() {
	USART_Init();
	USART_Transmit('M'); // Enviar un mensaje inicial para indicar que el maestro está listo

	while (1) {
		// Esperar a que haya un comando disponible para enviar
		if (PIND & (1 << PD2)) { // Supongamos que PD2 se usa para el avance
			USART_Transmit('m'); // Enviar comando para avanzar
			_delay_ms(500);
		}
		else if (PIND & (1 << PD3)) { // Supongamos que PD3 se usa para retroceso
			USART_Transmit('n'); // Enviar comando para retroceder
			_delay_ms(500);
		}
	}

	return 0;
}
