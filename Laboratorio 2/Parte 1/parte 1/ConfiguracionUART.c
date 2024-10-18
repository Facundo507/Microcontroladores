/*
 * main.c
 *
 * Created: 10/18/2024 7:50:45 PM
 *  Author: Facundo
 */ 

#define F_CPU 16000000UL
#include <xc.h>
#include <avr/io.h>
#include <util/delay.h>




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


int main(void)
	
	{
		// Configurar UART con la velocidad de baudios de 9600
		UART_init(103);
		char Opcion;
		UART_sendString("* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * \n");
		UART_sendString(" \n");
		UART_sendString("Bienvenido al programa para dibujar sobre el Plotter con ATmega328p\r\n");
		UART_sendString(" \n");
		UART_sendString("* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * \n");
		UART_sendString(" \n");
		UART_sendString("Seleccione una de las siguientes opciones: \n");
		UART_sendString("1- Dibujar Triángulo \n");
		UART_sendString("2- Dibujar Círculo \n");
		UART_sendString("3- Dibujar Cruz \n");
		UART_sendString("4- Dibujar Zorro \n");
		UART_sendString("5- Dibujar Flor \n");
		UART_sendString("6- Dibujo personalizado \n");
		
		while(1){
			Opcion = UART_receiveChar();
			if (Opcion == '1'){
				UART_sendString("Dibujando Triángulo: \n");
				
				
				
				UART_sendString("Triángulo Finalizado: \n");
			}
			if (Opcion == '2'){
				UART_sendString("Dibujando Círculo: \n");
				
				UART_sendString("Círculo Finalizado: \n");
			}
			if (Opcion == '3'){
				UART_sendString("Dibujando Cruz: \n");
				
				
				UART_sendString("Cruz Finalizado: \n");
			}
			if (Opcion == '4'){
				UART_sendString("Dibujando Zorro: \n");
				
				
				UART_sendString("Zorro Finalizado: \n");
			}
			if (Opcion == '5'){
				UART_sendString("Dibujando Flor: \n");
				
				
				UART_sendString("Flor Finalizado: \n");
			}
			if (Opcion == '6'){
				UART_sendString("Lea las siguientes instrucciones para dibujar: \n");
				UART_sendString("w- Mover el puntero hacia arriba: \n");
				UART_sendString("s- Mover el puntero hacia abajo: \n");
				UART_sendString("d- Mover el puntero hacia la derecha: \n");
				UART_sendString("d- Mover el puntero hacia la izquierda: \n");
				UART_sendString("0- Volver al menú principal: \n");
				Opcion = UART_receiveChar();
				while (Opcion != 0){
					if (Opcion == 'w'){
						UART_sendString("w\n");
						
					}
					if (Opcion == 's'){
						UART_sendString("s\n");
					}
					if (Opcion == 'd'){
						UART_sendString("d\n");
					}
					if (Opcion == 'a'){
						UART_sendString("a\n");
						
				}
				Opcion = UART_receiveChar();
			}
		}
	} }
