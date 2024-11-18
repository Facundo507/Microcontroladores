#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include "UART.h"
#define SS 2		  // Pin conectado a SS habilitador de Esclavo 1
#define MOSI   3   // Pin conectado a MOSI Master output Slave input
#define MISO 4   // Pin conectado a MISO Master input Slave output
#define SCLK  5   // Pin conectado a SCLK Serial Clock
#define PINLuz 7


void SPI_SlaveInit() {
	// Configura el ATMega328P como maestro en el bus SPI
	DDRB |= (1 << MISO);  // Configura pin de salida
	SPCR = (1 << SPE); // Habilita SPI
}

uint8_t SPI_SlaveReceive(){
	
	while(!(SPSR & (1 << SPIF)));   //Esperando a que la recepci?n se complete

	return SPDR;                    //Devuelve informaci?n recibida
}

int main() {
	DDRD |= (1 << PINLuz);
	SPI_SlaveInit();      // Inicializa la comunicaci?n SPI como maestro
	_delay_ms(10);
	UART_init(103);
	while (1) {
		unsigned char received = 0;
		received = SPI_SlaveReceive();
		UART_sendChar(received);
		UART_sendString("\r\n");
		if (received == 1) {
			UART_sendString("EncenderLuz");
			PORTD |= (1 << PINLuz);
			
		}
		else if (received == 0) {
			PORTD &= ~(1 << PINLuz);
			UART_sendString("ApagarLuz");
		}
		else{
			PORTD &= ~(1 << PINLuz);
			UART_sendString("Indeterminado");
		}
		

	}
	
	return 0;
}
