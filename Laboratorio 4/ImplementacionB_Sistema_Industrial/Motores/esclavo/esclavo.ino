#define F_CPU 16000000UL // Frecuencia del CPU

#include <avr/io.h>
#include <util/delay.h>

#define BAUD 9600 // 105us para 9600
#define MY_UBRR F_CPU/16/BAUD-1

#define TX_PIN PB0
#define RX_PIN PB1

#define d_baud 104

// Definiciones para el control del motor
#define MotorAdelante PORTD |= (1 << PD5);
#define MotorAtras PORTD |= (1 << PD6);
#define MotorOff PORTD &= ~((1 << PD5) | (1 << PD6));

// Definiciones de pines para los relés
#define ReleAdelante PD5
#define ReleAtras PD6
#define BotonEmergencia PD7
#define BotonRearme PB2
#define BotonInicioAdelante PB3
#define BotonInicioAtras PB4

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

void seguridadMotor() {
	if ((PIND & (1 << BotonEmergencia)) == 0) {
		MotorOff // Parada de emergencia
		while ((PINB & (1 << BotonRearme)) == 0); // Esperar a rearme
	}
}

int main() {
	initSerial();
	USART_Init();
	USART_Transmit('H');
	DDRD |= (1 << ReleAdelante) | (1 << ReleAtras) | (1 << BotonEmergencia);
	DDRB |= (1 << BotonRearme) | (1 << BotonInicioAdelante) | (1 << BotonInicioAtras);
	
	while (1) {
		seguridadMotor();

		if (!(PINB & (1 << RX_PIN))) {
			uint8_t receivedGPIO = receiveByteGPIO();
			USART_Transmit(receivedGPIO); // Enviar el dato recibido por GPIO
			if (receivedGPIO == 'm'){
				MotorAdelante
				_delay_ms(5000);
				MotorOff
			}
			else if (receivedGPIO == 'n'){
				MotorAtras
				_delay_ms(5000);
				MotorOff
			}
		}

		if (UCSR0A & (1 << RXC0)) {
			uint8_t receivedUSART = USART_Receive();
			sendByteGPIO(receivedUSART); // Enviar el dato recibido por USART
		}

		if ((PINB & (1 << BotonInicioAdelante)) == 0) {
			MotorAdelante
			_delay_ms(5000);
			MotorOff
		}
		else if ((PINB & (1 << BotonInicioAtras)) == 0) {
			MotorAtras
			_delay_ms(5000);
			MotorOff
		}
	}

	return 0;
}
