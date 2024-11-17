#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#define SS 2		 // Pin conectado a SS habilitador de Esclavo 1
#define SS2 6		 // Pin conectado a SS2 habilitador de Esclavo 2
#define MOSI 3  	 // Pin conectado a MOSI Master output Slave input
#define MISO 4 		 // Pin conectado a MISO Master input Slave output
#define SCLK 5  	 // Pin conectado a SCLK Serial Clock

uint8_t dato1 = 0x00;
uint8_t dato2 = 0x00;

void SPI_MasterInit();
void SPI_MasterTransmit(char data, char slave);
uint8_t SPI_MasterReceive();


int main() {
	
	SPI_MasterInit();		      // Inicializa la comunicación SPI como maestro
	_delay_ms(10);
	
	char data_to_send = 0x00;
	
	while (1) {
		data_to_send = 'R';     // Datos a enviar a un Esclavo
		
		SPI_MasterTransmit(data_to_send, SS);
		_delay_ms(10);
		
		dato1 = SPI_MasterReceive(); // Leer datos
		_delay_ms(500);
		/*
		
		data_to_send = 0xEE;        // Datos a enviar al segundo Esclavo
		
		SPI_MasterTransmit(data_to_send, SS2);
		_delay_ms(10);
		
		dato2 = SPI_MasterReceive();  // Leer datos
		_delay_ms(500);
		*/
	}
	
	return 0;
}

void SPI_MasterInit() {
	// Configura el ATMega328P como maestro en el bus SPI
	DDRB |= (1 << MOSI) | (1 << MISO) | (1 << SCLK) | (1 << SS) | (1 << SS2);  // Configura pines de salida
	SPCR = (1 << SPIE) | (1 << SPE) | (1 << MSTR) | (1 << SPR0); // Habilita interrupcion, Habilita SPI, modo maestro, velocidad de reloj f/16
}

void SPI_MasterTransmit(char data, char slave) {
	
	PORTB &= ~(1 << slave);        // Inicia la transmisión de datos
	_delay_ms(10);
	SPDR = data;
	while (!(SPSR & (1 << SPIF))); // Espera a que se complete la transmisión
	_delay_ms(10);
	PORTB |= (1 << slave);
}

uint8_t SPI_MasterReceive(){
	
	while(!(SPSR & (1 << SPIF)));   //Esperando a que la recepción se complete

	return SPDR;                    //Devuelve información recibida
}