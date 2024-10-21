/*
 * main.c
 *
 * Created: 10/14/2024 6:34:38 PM
 *  Author: dasil
 */ 


#define F_CPU 16000000UL
#include <xc.h>
#include <avr/io.h>
#include <util/delay.h>
#define Temp0 0
#define Temp22 1
#define Temp30 2
#define Temp40 3
#define Temp50 4





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



void SetupPWM(void){
	DDRB = 0xFF;
	TCCR1A = (1 << COM1A1) | (1 << COM1B1) | (1 << WGM11);
	TCCR1B = (1 << WGM13) | (1 << WGM12) | (0 << CS12) | (1 << CS11) | (1 << CS10);
	// los 3 menos significativos configuran el prescaler, en este
	// caso configurado en 1024, pin 3 y 4 configuracion del WGM1 para hacerlo en FASTPWM
	//
	ICR1 = 1024; // Valor del top, este valor con el prescaler de 64 nos da una frecuenca de 1736,11hz, el más proximo a 1725hz.
	OCR1A = 72; // Valor del Ciclo útil
	OCR1B = 72;
}

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

void CompararVoltaje(float Voltaje){
	if ( (Voltaje >= 0.0) & (Voltaje < 1.0)){
		UART_sendString("0.");
		if ( (Voltaje >= 0.0) & (Voltaje < 0.1) ){
			UART_sendString("0V");
		}
		if ( (Voltaje >= 0.1) & (Voltaje < 0.2) ){
			UART_sendString("1V");
		}
		if ( (Voltaje >= 0.2) & (Voltaje < 0.3) ){
			UART_sendString("2V");
		}
		if ( (Voltaje >= 0.3) & (Voltaje < 0.4) ){
			UART_sendString("3V");
		}
		if ( (Voltaje >= 0.4) & (Voltaje < 0.5) ){
			UART_sendString("4V");
		}
		if ( (Voltaje >= 0.5) & (Voltaje < 0.6) ){
			UART_sendString("5V");
		}
		if ( (Voltaje >= 0.6) & (Voltaje < 0.7) ){
			UART_sendString("6V");
		}
		if ( (Voltaje >= 0.7) & (Voltaje < 0.8) ){
			UART_sendString("7V");
		}
		if ( (Voltaje >= 0.8) & (Voltaje < 0.9) ){
			UART_sendString("8V");
		}
		if ( (Voltaje >= 0.9) & (Voltaje < 1.0) ){
			UART_sendString("9V");
		}
	}
	if ( (Voltaje >= 1.0) & (Voltaje < 2.0)){
		UART_sendString("1.");
		if ( (Voltaje >= 1.0) & (Voltaje < 1.1) ){
			UART_sendString("0V");
		}
		if ( (Voltaje >= 1.1) & (Voltaje < 1.2) ){
			UART_sendString("1V");
		}
		if ( (Voltaje >= 1.2) & (Voltaje < 1.3) ){
			UART_sendString("2V");
		}
		if ( (Voltaje >= 1.3) & (Voltaje < 1.4) ){
			UART_sendString("3V");
		}
		if ( (Voltaje >= 1.4) & (Voltaje < 1.5) ){
			UART_sendString("4V");
		}
		if ( (Voltaje >= 1.5) & (Voltaje < 1.6) ){
			UART_sendString("5V");
		}
		if ( (Voltaje >= 1.6) & (Voltaje < 1.7) ){
			UART_sendString("6V");
		}
		if ( (Voltaje >= 1.7) & (Voltaje < 1.8) ){
			UART_sendString("7V");
		}
		if ( (Voltaje >= 1.8) & (Voltaje < 1.9) ){
			UART_sendString("8V");
		}
		if ( (Voltaje >= 1.9) & (Voltaje < 2.0) ){
			UART_sendString("9V");
		}
	}
	
	if ( (Voltaje >= 2.0) & (Voltaje < 3.0)){
		UART_sendString("2.");
		if ( (Voltaje >= 2.0) & (Voltaje < 2.1) ){
			UART_sendString("0V");
		}
		if ( (Voltaje >= 2.1) & (Voltaje < 2.2) ){
			UART_sendString("1V");
		}
		if ( (Voltaje >= 2.2) & (Voltaje < 2.3) ){
			UART_sendString("2V");
		}
		if ( (Voltaje >= 2.3) & (Voltaje < 2.4) ){
			UART_sendString("3V");
		}
		if ( (Voltaje >= 2.4) & (Voltaje < 2.5) ){
			UART_sendString("4V");
		}
		if ( (Voltaje >= 2.5) & (Voltaje < 2.6) ){
			UART_sendString("5V");
		}
		if ( (Voltaje >= 2.6) & (Voltaje < 2.7) ){
			UART_sendString("6V");
		}
		if ( (Voltaje >= 2.7) & (Voltaje < 2.8) ){
			UART_sendString("7V");
		}
		if ( (Voltaje >= 2.8) & (Voltaje < 2.9) ){
			UART_sendString("8V");
		}
		if ( (Voltaje >= 2.9) & (Voltaje < 3.0) ){
			UART_sendString("9V");
		}
		
	}
	if ( (Voltaje >= 3.0) & (Voltaje < 4.0)){
		UART_sendString("3.");
		if ( (Voltaje >= 3.0) & (Voltaje < 3.1) ){
			UART_sendString("0V");
		}
		if ( (Voltaje >= 3.1) & (Voltaje < 3.2) ){
			UART_sendString("1V");
		}
		if ( (Voltaje >= 3.2) & (Voltaje < 3.3) ){
			UART_sendString("2V");
		}
		if ( (Voltaje >= 2.3) & (Voltaje < 3.4) ){
			UART_sendString("3V");
		}
		if ( (Voltaje >= 3.4) & (Voltaje < 3.5) ){
			UART_sendString("4V");
		}
		if ( (Voltaje >= 3.5) & (Voltaje < 3.6) ){
			UART_sendString("5V");
		}
		if ( (Voltaje >= 3.6) & (Voltaje < 3.7) ){
			UART_sendString("6V");
		}
		if ( (Voltaje >= 3.7) & (Voltaje < 3.8) ){
			UART_sendString("7V");
		}
		if ( (Voltaje >= 3.8) & (Voltaje < 3.9) ){
			UART_sendString("8V");
		}
		if ( (Voltaje >= 3.9) & (Voltaje < 4.0) ){
			UART_sendString("9V");
			
		}
	}
	if ( (Voltaje >= 4.0) & (Voltaje < 5.0)){
		UART_sendString("4.");
		if ( (Voltaje >= 4.0) & (Voltaje < 4.1) ){
			UART_sendString("0V");
		}
		if ( (Voltaje >= 4.1) & (Voltaje < 4.2) ){
			UART_sendString("1V");
		}
		if ( (Voltaje >= 4.2) & (Voltaje < 4.3) ){
			UART_sendString("2V");
		}
		if ( (Voltaje >= 4.3) & (Voltaje < 4.4) ){
			UART_sendString("3V");
		}
		if ( (Voltaje >= 4.4) & (Voltaje < 4.5) ){
			UART_sendString("4V");
		}
		if ( (Voltaje >= 4.5) & (Voltaje < 4.6) ){
			UART_sendString("5V");
		}
		if ( (Voltaje >= 4.6) & (Voltaje < 4.7) ){
			UART_sendString("6V");
		}
		if ( (Voltaje >= 4.7) & (Voltaje < 4.8) ){
			UART_sendString("7V");
		}
		if ( (Voltaje >= 4.8) & (Voltaje < 4.9) ){
			UART_sendString("8V");
		}
		if ( (Voltaje >= 4.9) & (Voltaje < 5.0) ){
			UART_sendString("9V");
	}
}
}

int main(void){
	
	
	SetupPWM();
	SetupADC();
	uint16_t ValorADC;
	
	UART_init(103);
	
	// OCR1B es el ventilador
    while(1)
    {
		float Temperatura = ValorADC * (5.0 / 1023.0);
		
		ValorADC = LeerADC(1);
		//OCR1B = ValorADC;
		Temperatura = ValorADC * (5.0 / 1023.0);
				
		UART_sendString("\r\n");
		
		if ((Temperatura >= Temp0) & (Temperatura < Temp22)) {
			UART_sendString("Temperatura entre 0 y 22 grados\r\n");
			
			PORTB |= (1<< PORTB5);
		}
		if ((Temperatura >= Temp22) & (Temperatura < Temp30)) {
			UART_sendString("Temperatura entre 22 y 30\r\n");
			PORTB &= ~(1<< PORTB5); //PONER Un cero
		}
		
		if ((Temperatura >= Temp30) & (Temperatura < Temp40)) {
			UART_sendString("Temperatura entre 30 y 40\r\n");
			OCR1A = 300;
			PORTB &= ~(1<< PORTB5); //PONER Un cero
		}
		if (Temperatura >= Temp40 & Temperatura < Temp50) {
			UART_sendString("Temperatura entre 40 y 50\r\n");
			OCR1A = 700;
			PORTB &= ~(1<< PORTB5); //PONER Un cero
		}
		if (Temperatura >= Temp50) {
			UART_sendString("Temperatura Mayor a 50\r\n");
			OCR1A = 1023;
			PORTB &= ~(1<< PORTB5); //PONER Un cero
		}
		CompararVoltaje(Temperatura);
    }
}