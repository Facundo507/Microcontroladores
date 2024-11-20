#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <util/twi.h>
#define PCF8574	0x27 // 0x3F para en otra lcd con i2c
#include "twi_lcd.h"
#define DHT_PIN PD2
#define IR_PIN PD3
#include <stdio.h>
#define LuzMinima 600
#define BitLuz 0
#define BitIR 1




#define SS 2		 // Pin conectado a SS habilitador de Esclavo 1
#define SS2 6
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

void DHT_start(){
	DDRD |= (1 << DHT_PIN);
	PORTD &= ~(1 << DHT_PIN);
	_delay_ms(18);
	PORTD |= (1<<DHT_PIN);
	_delay_us(40);
}
uint8_t DHT_Respuesta(){
	DDRD &= ~(1 << DHT_PIN);
	_delay_us(40);
	if (!(PIND & (1 << DHT_PIN))){
		_delay_us(80);
		if ((PIND & (1 << DHT_PIN))){
			_delay_us(80);
			
			return 1;
			
			
		}
		
		
	}
	
	return 0;
}


void DHT_read(uint8_t* ListaResultado) {
	for (int j = 0; j < 5; j++) {
		uint8_t result = 0;
		for (int i = 0; i < 8; i++) {
			while (!(PIND & (1 << DHT_PIN)));
			_delay_us(30);
			if (PIND & (1 << DHT_PIN)) {
				result |= (1 << (7 - i));
			}
			while (PIND & (1 << DHT_PIN));
		}
		ListaResultado[j] = result;
	}
}

void Procesar_DatosDHT(){
	uint8_t ListaDHT[5];
	char buffer[5][8];
	
	DHT_start();
	DHT_Respuesta();
	DHT_read(ListaDHT);
	
	for (int i = 0; i<4; i++){
		if (i <= 2){
			twi_lcd_cmd(0xC0);
			twi_lcd_msg("Enviando Temp   ");
			_delay_ms(300);
		}
		else if (i >2){
			twi_lcd_cmd(0xC0);
			twi_lcd_msg("Enviando Humedad");
			_delay_ms(300);
		}
		SPI_MasterTransmit(ListaDHT[i], SS);
		_delay_ms(10);
		sprintf(buffer[i], "%u", ListaDHT[i]);
		twi_lcd_cmd(0xC0);
		twi_lcd_msg("Enviado!     ");
		
	}
	
	twi_lcd_cmd(0x80);
	twi_lcd_msg("T: ");
	twi_lcd_msg(buffer[2]);
	twi_lcd_msg("H: ");
	twi_lcd_msg(buffer[0]);
	
	
	
}

void EnviarByteSPI (uint8_t Byte){
	twi_lcd_cmd(0xC0);
	twi_lcd_msg("Env Byte LDRBuzz ");
	
	_delay_ms(300);
	SPI_MasterTransmit(Byte, SS);
	_delay_ms(10);
	twi_lcd_cmd(0xC0);
	twi_lcd_msg("Datos Enviados!  ");
	
	
}

uint8_t Procesar_DatosLDRIR(uint8_t DatoEnviarSPI){		
		uint16_t ValorADC = LeerADC(3);
		
		twi_lcd_cmd(0x8B);

		char buffer[6];       // Buffer para almacenar la cadena (máximo 3 dígitos + nulo)
		// Convertir el valor a cadena
		sprintf(buffer, "%u", ValorADC);
		//twi_lcd_cmd(0xC0);
		
		if (ValorADC > LuzMinima){
			DatoEnviarSPI |= (1<<BitLuz);
			
		}
		else
		{
			DatoEnviarSPI &= ~(1<<BitLuz);
		}
		if (PIND & (1<< IR_PIN)){
			twi_lcd_msg("OFF");
			
			_delay_ms(100);
			DatoEnviarSPI &= ~(1<<BitIR); //Detectapresencia con un cero lógico
		}
		else
		{
			twi_lcd_msg("ON ");
			
			_delay_ms(100);
			DatoEnviarSPI |= (1<<BitIR);
			
		}
		return DatoEnviarSPI;
		
}


int main() {
	
	twi_init(); //Cosas del LCD
	
	twi_lcd_init();
	
	twi_lcd_cmd(0x80);
	twi_lcd_clear();
	
	_delay_ms(2000);
	SetupADC();
	

	SPI_MasterInit();		      // Inicializa la comunicación SPI como maestro
	_delay_ms(10);
	
	char DatoEnviarSPI = 0x00;
	
	//char Datos_DHT[8][5];
	
	
	while (1) {
		//Comandos: 0x80 Primera linea
		// 0xC0 Segunda linea
		DatoEnviarSPI = Procesar_DatosLDRIR(DatoEnviarSPI);
		Procesar_DatosDHT();
		EnviarByteSPI(DatoEnviarSPI);
		_delay_ms(500);
		
		
		
		
		
		
	
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