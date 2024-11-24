#define F_CPU 16000000UL // Frecuencia del CPU

#include <avr/io.h>
#include <util/delay.h>

#define BAUD 9600 // 105us para 9600
#define MY_UBRR F_CPU/16/BAUD-1

#define TX_PIN PB0
#define RX_PIN PB1

#define d_baud 104
#define BitAuto 0
#define BitCaja 1
#define BitEtiqueta 2
#define BitVacio 3



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

#define PINCaja PD5
#define PINVacio PD6
#define PINEtiqueta 7
#define Apagar13 PORTB &= ~(1<< PB5);
#define Encender13 PORTB |= (1<< PB5);
//"P" de pistón
#define ExtenderPCaja PORTD |= (1 << PINCaja);
#define ContraerPCaja PORTD &= ~(1 << PINCaja);
#define VacioON PORTD |= (1 << PINVacio);
#define VacioOFF PORTD &= ~(1 << PINVacio);
#define ExtenderPEtiqueta PORTD |= (1 << PINEtiqueta);
#define ContraerPEtiqueta PORTD &= ~(1 << PINEtiqueta);
void ModoAutomatico(){
	for (int i =0; i <2; i++){
		Encender13
		_delay_ms(500);
		Apagar13
		_delay_ms(500); //Enciendo unos leds para indicar que se entró en el modo de etiquetar.
	}
	ExtenderPCaja
	_delay_ms(5000);
	ExtenderPEtiqueta
	_delay_ms(1500);
	VacioON //Es para que el pistón de la etiqueta agarre una etiqueta, la suelta con OFF
	_delay_ms(100);
	ContraerPEtiqueta
	_delay_ms(1000);
	ContraerPCaja
	_delay_ms(2000);
	ExtenderPEtiqueta
	_delay_ms(2000);
	VacioOFF
	ContraerPEtiqueta
}


void RecibirYMandarDatos(){
	uint8_t receivedGPIO = receiveByteGPIO();
	uint8_t EnviarDato = 0;
	USART_Transmit(receivedGPIO); // Enviar el dato recibido por GPIO
	if (receivedGPIO == 'a'){
		ModoAutomatico();
		EnviarDato |= (1<<BitAuto);
		
		USART_Transmit('a');
		return;
	}
	if (receivedGPIO & (1<< BitCaja)){
		ExtenderPCaja
		_delay_ms(1000);
		
		EnviarDato |= (1<<BitCaja);
		USART_Transmit('c');
	}
	else
	{
		ContraerPCaja
		_delay_ms(1000);
	}
	if (receivedGPIO & (1<< BitEtiqueta)){
		ExtenderPEtiqueta
		USART_Transmit('e');
		
		EnviarDato |= (1<<BitEtiqueta);
		_delay_ms(1000);
	}
	else
	{
		ContraerPEtiqueta
		_delay_ms(1000);
		
	}
	if (receivedGPIO & (1<< BitVacio)){
		VacioON
		_delay_ms(1000);
		EnviarDato |= (1<<BitVacio);
		USART_Transmit('v');
	}
	else{
		VacioOFF
		_delay_ms(1000);
	}
	sendByteGPIO(EnviarDato);
}
/*
void RecibirYMandarDatos(){
	uint8_t receivedGPIO = receiveByteGPIO();
	uint8_t EnviarDato = 0;
	USART_Transmit(receivedGPIO); // Enviar el dato recibido por GPIO
	if (receivedGPIO & (1<< BitAuto)){
		ModoAutomatico();
		EnviarDato |= (1<<BitAuto);
		return;
		USART_Transmit('A');
	}
	if (receivedGPIO & (1<< BitCaja)){
		ExtenderPCaja
		_delay_ms(1000);
		
		EnviarDato |= (1<<BitCaja);
		USART_Transmit('c');
		USART_Transmit('A');
	}
	else
	{
		ContraerPCaja
		_delay_ms(1000);
	}
	if (receivedGPIO & (1<< BitEtiqueta)){
		ExtenderPEtiqueta
		USART_Transmit('e');
		USART_Transmit('t');
		
		EnviarDato |= (1<<BitEtiqueta);
		_delay_ms(1000);
	}
	else
	{
		ContraerPEtiqueta
		_delay_ms(1000);
		
	}
	if (receivedGPIO & (1<< BitVacio)){
		VacioON
		_delay_ms(1000);
		EnviarDato |= (1<<BitVacio);
		USART_Transmit('v');
	}
	else{
		VacioOFF
		_delay_ms(1000);
	}
	sendByteGPIO(EnviarDato);
}*/
int main() {
	initSerial();
	USART_Init();

	while (1) {
		if (!(PINB & (1 << RX_PIN))) {
			uint8_t receivedGPIO = receiveByteGPIO();
			_delay_ms(1);
			USART_Transmit(receivedGPIO); // Enviar el
			_delay_ms(1);
		}

		if (UCSR0A & (1 << RXC0)) {
			uint8_t receivedUSART = USART_Receive();
			_delay_us(1000);
			sendByteGPIO(receivedUSART); // Enviar el b
			_delay_us(1000);
		}
	}

	return 0;
}