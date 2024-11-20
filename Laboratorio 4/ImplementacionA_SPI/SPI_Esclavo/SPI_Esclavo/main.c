#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include "UART.h"
#define SS 2		  // Pin conectado a SS habilitador de Esclavo 1
#define MOSI   3   // Pin conectado a MOSI Master output Slave input
#define MISO 4   // Pin conectado a MISO Master input Slave output
#define SCLK  5   // Pin conectado a SCLK Serial Clock
#define PINLuz 7
#define BitLuz 0
#define Ventilador 6
#define PINAlarma 5
#define BitALARMA 1
void SPI_SlaveInit() {
	// Configura el ATMega328P como maestro en el bus SPI
	DDRB |= (1 << MISO);  // Configura pin de salida
	SPCR = (1 << SPE); // Habilita SPI
}

uint8_t SPI_SlaveReceive(){
	
	while(!(SPSR & (1 << SPIF)));   //Esperando a que la recepci?n se complete

	return SPDR;                    //Devuelve informaci?n recibida
}

void ProcesarDatos_DHT(){
	uint8_t DHT_Datosint[5];
	char DHT_DatosChar[5][5];
	for (int i = 0; i<4; i++){
		DHT_Datosint[i] = SPI_SlaveReceive();
		sprintf(DHT_DatosChar[i], "%u", DHT_Datosint[i]);
	}
	
	UART_sendString("T: ");
	UART_sendString(DHT_DatosChar[2]);
	UART_sendString(",");
	UART_sendString(DHT_DatosChar[3]);
	UART_sendString("H: ");
	UART_sendString(DHT_DatosChar[0]);
	UART_sendString(",");
	UART_sendString(DHT_DatosChar[1]);
	
	if (DHT_Datosint[2]>24){
		PORTD |= (1 << Ventilador);
	}
	else{
		PORTD &= ~(1 << Ventilador);
	}
	
}
void ProcesarByte(void){
	
	unsigned char received = 0;
	received = SPI_SlaveReceive();
	UART_sendChar(received);
	
	if (received & (1 << BitLuz)) {
		UART_sendString(" EncenderLuz");
		PORTD |= (1 << PINLuz);
		
	}
	else if (~(received & (1 << BitLuz)) ) {
		PORTD &= ~(1 << PINLuz);
		UART_sendString(" ApagarLuz");
	}
	if (received & (1 << 1)){
		UART_sendString(" ALARMA ON");
		PORTD |= (1<<PINAlarma);
	}else
	{
		UART_sendString(" ALARMA OFF");
		PORTD &= ~(1<<PINAlarma);
	}
	
	UART_sendString("\r\n");
}

int main() {
	DDRD |= (1 << PINLuz) | (1 << Ventilador) | (1<<PINAlarma);
	SPI_SlaveInit();      // Inicializa la comunicaci?n SPI como maestro
	_delay_ms(10);
	UART_init(103);
	while (1) {
		ProcesarDatos_DHT();
		ProcesarByte();
		
		
	
	}
	
	return 0;
}
