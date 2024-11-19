#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <util/twi.h>
#define PCF8574	0x27 // 0x3F para en otra lcd con i2c
#include "twi_lcd.h"

#define LuzMinima 600
#define BitLuz 0



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

void SetupADC(void){
	ADMUX |= (1 << REFS0); // Seleccionado Vcc
	ADMUX &= ~(1 << ADLAR); // Lectura derecha a 10 bits
	ADCSRA |= (1<< ADPS2) | (1 << ADPS1); //Prescaler 64
	
	ADCSRA |= (1 << ADEN); //Habilita ADC
}

uint16_t LeerADC(uint8_t canal){
	ADMUX = (ADMUX & 0xF0) | (canal & 0x0F);
	//Inicia la conversion en el canal "canal"
	ADCSRA |= (1 << ADSC);
	//Espera a que termine la conversion
	while (ADCSRA & (1 << ADSC));
	//Retorna el resultado
	return ADC;
	
	
}


int main() {
	
	twi_init(); //Cosas del LCD
	
	twi_lcd_init();
	
	twi_lcd_cmd(0x80);
	
	twi_lcd_clear();
	
	
	SetupADC();
	
	SPI_MasterInit();		      // Inicializa la comunicación SPI como maestro
	_delay_ms(10);
	
	char DatoEnviarSPI = 0x00;
	uint8_t valor = 13;  // Valor a convertir
	while (1) {
		//Comandos: 0x80 Primera linea
		// 0xC0
		
		uint16_t ValorADC = LeerADC(3);
		//--- Select 1nd Row
		twi_lcd_cmd(0x80);
		//--- Send a String to LCD
		twi_lcd_msg("Linea Miau!!");
		
		
		
		char buffer[4];       // Buffer para almacenar la cadena (máximo 3 dígitos + nulo)
		
		// Convertir el valor a cadena
		sprintf(buffer, "%u", valor);
		
		twi_lcd_cmd(0xC0);
		twi_lcd_msg(buffer);
		_delay_ms(1000);
		valor++;
		
		
		if (ValorADC <= LuzMinima){
			DatoEnviarSPI |= (1<<BitLuz);
			
		}
		else
		{
			DatoEnviarSPI &= (1<<BitLuz);
		}
		SPI_MasterTransmit(DatoEnviarSPI, SS);
		_delay_ms(10);
		/*dato1 = SPI_MasterReceive(); // Leer datos
		_delay_ms(50); */
		

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