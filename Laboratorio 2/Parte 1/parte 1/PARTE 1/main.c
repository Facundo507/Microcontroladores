/*
 * main.c
 *
 * Created: 10/18/2024 7:51:53 PM
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

void init_pins() {
	// Configurar los pines como salida en el puerto B (asignación de pines)
	DDRB |= (1 << PB2) | (1 << PB3) | (1 << PB4) | (1 << PB5) | (1 << PB6) | (1 << PB7);

	// Configurar los pines para entrada en el puerto D si es necesario (ejemplo para botones)
	DDRD &= ~(1 << PD2) & ~(1 << PD3);  // Configura PD2 y PD3 como entrada
	PORTD |= (1 << PD2) | (1 << PD3);   // Activa pull-up en PD2 y PD3
}

void mover_solenoide(int bajar) {
	if (bajar) {
		PORTB |= (1 << PB2);   // Activar solenoide para bajar
		PORTB &= ~(1 << PB3);  // Desactivar solenoide para subir
		} else {
		PORTB &= ~(1 << PB2);  // Desactivar solenoide para bajar
		PORTB |= (1 << PB3);   // Activar solenoide para subir
	}
}

// Función para mover el plóter en las direcciones X e Y
void mover_plotter(int dir) {
	switch (dir) {
		case 0:  // Mover hacia abajo
		PORTD |= (1 << PD4);
		_delay_ms(1000);
		PORTD &= ~(1 << PD4);
		break;
		case 1:  // Mover hacia arriba
		PORTD |= (1 << PD5);
		_delay_ms(1000);
		PORTD &= ~(1 << PD5);
		break;
		case 2:  // Mover hacia la izquierda
		PORTD |= (1 << PD6);
		_delay_ms(1000);
		PORTD &= ~(1 << PD6);
		break;
		case 3:  // Mover hacia la derecha
		PORTD |= (1 << PD7);
		_delay_ms(1000);
		PORTD &= ~(1 << PD7);
		break;
		case 4:  // Mover hacia abajo-izquierda (diagonal)
		for(int i = 0; i < 14; i++){
			PORTD &= ~(1 << PD6);
			PORTD |= (1 << PD4);
			_delay_ms(50);
			PORTD &= ~(1 << PD4);
			PORTD |= (1 << PD6);
			_delay_ms(50);
		}
		PORTD &= (~(1 << PD4) & ~(1 << PD6));
		break;
		case 5:  // Mover hacia abajo-derecha (diagonal)
		for(int i = 0; i < 14; i++){
			PORTD &= ~(1 << PD7);
			PORTD |= (1 << PD4);
			_delay_ms(50);
			PORTD &= ~(1 << PD4);
			PORTD |= (1 << PD7);
			_delay_ms(50);
		}
		PORTD &= (~(1 << PD4) & ~(1 << PD7));
		break;
		case 6:  // Mover hacia arriba-izquierda (diagonal)
		for(int i = 0; i < 14; i++){
			PORTD &= ~(1 << PD6);
			PORTD |= (1 << PD5);
			_delay_ms(50);
			PORTD &= ~(1 << PD5);
			PORTD |= (1 << PD6);
			_delay_ms(50);
		}
		PORTD &= (~(1 << PD5) & ~(1 << PD6));
		break;
		case 7:  // Mover hacia arriba-derecha (diagonal)
		for(int i = 0; i < 14; i++){
			PORTD &= ~(1 << PD7);
			PORTD |= (1 << PD5);
			_delay_ms(50);
			PORTD &= ~(1 << PD5);
			PORTD |= (1 << PD7);
			_delay_ms(50);
		}
		PORTD &= (~(1 << PD5) & ~(1 << PD7));
		break;
		case 10:  // Mover hacia abajo
		PORTD |= (1 << PD4);  // Activar PD4 (Abajo)
		_delay_ms(50);
		PORTD &= ~(1 << PD4); // Desactivar PD4
		break;
		case 11:  // Mover hacia arriba
		PORTD |= (1 << PD5);  // Activar PD5 (Arriba)
		_delay_ms(50);
		PORTD &= ~(1 << PD5); // Desactivar PD5
		break;
		case 12:  // Mover hacia la izquierda
		PORTD |= (1 << PD6);  // Activar PD6 (Izquierda)
		_delay_ms(50);
		PORTD &= ~(1 << PD6); // Desactivar PD6
		break;
		case 13:  // Mover hacia la derecha
		PORTD |= (1 << PD7);  // Activar PD7 (Derecha)
		_delay_ms(50);
		PORTD &= ~(1 << PD7); // Desactivar PD7
		break;
	}
}

void Centrar(){
	mover_plotter(2);
	mover_plotter(2);
	mover_plotter(2);
	mover_plotter(2);
	mover_plotter(2);

	mover_plotter(2);
	mover_plotter(2);
	mover_plotter(2);
	mover_plotter(2);
	mover_plotter(2);

	mover_plotter(0);
	mover_plotter(0);
	mover_plotter(0);
	mover_plotter(0);
	mover_plotter(0);

	mover_plotter(0);
	mover_plotter(0);
	mover_plotter(0);
	mover_plotter(0);
	mover_plotter(0);
}

void dibujar_circulo() {
	Centrar();
	mover_solenoide(1);
	_delay_ms(1500);
	int radio = 20;
	// Aumentar la resolución usando más puntos
	for (double angle = 0; angle < 2 * M_PI; angle += 0.01) { // Incrementar el ángulo
		int x = (int)(radio * cos(angle));
		int y = (int)(radio * sin(angle));

		// Dibujar punto en la posición (x, y)
		if (x >= 0 && y >= 0) {
			mover_plotter(13);
			mover_plotter(11);
			} else if (x < 0 && y >= 0) {
			mover_plotter(12);
			mover_plotter(11);
			} else if (x >= 0 && y < 0) {
			mover_plotter(13);
			mover_plotter(10);
			} else {
			mover_plotter(12);
			mover_plotter(10);
		}
	}
	mover_solenoide(0);
}

void Flor(){
	Centrar();
	mover_solenoide(1);  // Bajar solenoide
	_delay_ms(1500);
	// Secuencia de movimientos del plóter
	mover_plotter(1);
	mover_plotter(2);
	mover_plotter(7);
	mover_plotter(6);
	mover_plotter(3);
	mover_plotter(1);
	mover_plotter(5);
	mover_plotter(7);
	mover_plotter(0);
	mover_plotter(3);
	mover_plotter(4);
	mover_plotter(5);
	mover_plotter(2);
	mover_plotter(0);
	mover_plotter(6);
	mover_plotter(4);
	//Hojas
	mover_plotter(2);
	mover_plotter(5);
	mover_plotter(3);
	mover_plotter(3);
	mover_plotter(7);
	mover_plotter(2);
	mover_plotter(4);
	mover_plotter(6);
	//Tallo
	mover_plotter(7);
	mover_plotter(0);
	mover_plotter(0);
	mover_solenoide(0);  // Subir solenoide
}

void dibujar_Triangulo(){
	Centrar();
	mover_solenoide(1);
	_delay_ms(1500);
	mover_plotter(7);
	mover_plotter(5);
	mover_plotter(2);
	mover_solenoide(0);
}

void dibujar_cruz(){
	Centrar();
	mover_solenoide(1);
	_delay_ms(1500);
	mover_plotter(7);
	mover_solenoide(0);
	_delay_ms(1500);
	mover_plotter(2);
	mover_solenoide(1);
	_delay_ms(1500);
	mover_plotter(5);
	mover_solenoide(0);	
}

int main(void) {
	// Configurar UART con la velocidad de baudios de 9600
	UART_init(103);
	char Opcion;
	
	while(1){
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
		
		Opcion = UART_receiveChar();
		if (Opcion == '1'){
			UART_sendString("Dibujando Triángulo: \n");
			dibujar_Triangulo();
			UART_sendString("Triángulo Finalizado: \n");
		}
		if (Opcion == '2'){
			UART_sendString("Dibujando Círculo: \n");
			dibujar_circulo();
			UART_sendString("Círculo Finalizado: \n");
		}
		if (Opcion == '3'){
			UART_sendString("Dibujando Cruz: \n");
			dibujar_cruz();
			UART_sendString("Cruz Finalizado: \n");
		}
		if (Opcion == '4'){
			UART_sendString("Dibujando Zorro: \n");
			UART_sendString("Zorro Finalizado: \n");
		}
		if (Opcion == '5'){
			UART_sendString("Dibujando Flor: \n");
			Flor();
			UART_sendString("Flor Finalizado: \n");
		}
		if (Opcion == '6'){
			Centrar();
			mover_solenoide(1);
			_delay_ms(1500);
			UART_sendString("* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * \n");
			UART_sendString("Lea las siguientes instrucciones para dibujar: \n");
			UART_sendString("w- Mover el puntero hacia arriba: \n");
			UART_sendString("s- Mover el puntero hacia abajo: \n");
			UART_sendString("d- Mover el puntero hacia la derecha: \n");
			UART_sendString("a- Mover el puntero hacia la izquierda: \n");
			UART_sendString("0- Volver al menú principal: \n");
			Opcion = UART_receiveChar();
			while (Opcion != 0){
				if (Opcion == 'w'){
					UART_sendString("w\n");
					mover_plotter(1);
				}
				if (Opcion == 's'){
					UART_sendString("s\n");
					mover_plotter(0);
				}
				if (Opcion == 'd'){
					UART_sendString("d\n");
					mover_plotter(3);
				}
				if (Opcion == 'a'){
					UART_sendString("a\n");
					mover_plotter(2);
					
				}
				Opcion = UART_receiveChar();
			}
			mover_solenoide(0);
			_delay_ms(1500);
		}
	} 
}
