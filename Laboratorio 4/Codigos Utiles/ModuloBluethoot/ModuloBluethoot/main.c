#define F_CPU 16000000UL // Frecuencia del CPU

#include <avr/io.h>
#include <util/delay.h>

#define BAUD 9600 // 105us para 9600
#define MY_UBRR F_CPU/16/BAUD-1

#define TX_PIN PB0
#define RX_PIN PB1

#define d_baud 27

void initSerial() {
	// Configura PB0 como salida (TX)
	DDRB |= (1 << TX_PIN);
	// Configura PB1 como entrada (RX)
	DDRB &= ~(1 << RX_PIN);
}

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

void sendByteGPIO(uint8_t data) {
	// Enviar un byte a través de PB0 (TX)
	PORTB &= ~(1 << TX_PIN); // Start bit
	_delay_us(d_baud); // Esperar

	for (int i = 0; i < 8; i++) {
		if (data & (1 << i)) {
			PORTB |= (1 << TX_PIN); // Enviar bit alto
			} else {
			PORTB &= ~(1 << TX_PIN); // Enviar bit bajo
		}
		_delay_us(d_baud); // Esperar por el siguiente bit
	}

	PORTB |= (1 << TX_PIN); // Stop bit
	_delay_us(d_baud); // Esperar antes de enviar el siguiente byte
}

uint8_t receiveByteGPIO() {
	uint8_t data = 0;

	// Esperar a que el pin RX (PB1) sea bajo (start bit)
	while (PINB & (1 << RX_PIN));
	_delay_us(d_baud); // Esperar a que el primer bit se estabilice

	for (int i = 0; i < 8; i++) {
		if (PINB & (1 << RX_PIN)) {
			data |= (1 << i); // Leer bit
		}
		_delay_us(d_baud); // Esperar por el siguiente bit
	}

	// Esperar a que el pin RX sea alto (stop bit)
	while (!(PINB & (1 << RX_PIN)));

	return data;
}

int main() {
	initSerial();
	USART_Init();
	USART_Transmit('H');
	while (1) {
		if (!(PINB & (1 << RX_PIN))) {
			uint8_t receivedGPIO = receiveByteGPIO();
			USART_Transmit(receivedGPIO); // Enviar el dato recibido por GPIO
		}

		if (UCSR0A & (1 << RXC0)) {
			uint8_t receivedUSART = USART_Receive();
			sendByteGPIO(receivedUSART); // Enviar el dato recibido por USART
		}
	}

	return 0;
}