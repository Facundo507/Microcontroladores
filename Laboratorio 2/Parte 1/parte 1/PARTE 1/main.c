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
	DDRD |= (1 << PD2) | (1 << PD3) | (1 << PD4) | (1 << PD5) | (1 << PD6) | (1 << PD7);
}

void mover_solenoide(int bajar) {
	if (bajar) {
		PORTD |= (1 << PD2);   // Activar solenoide para bajar
		PORTD &= ~(1 << PD3);  // Desactivar solenoide para subir
		} else {
		PORTD &= ~(1 << PD2);  // Desactivar solenoide para bajar
		PORTD |= (1 << PD3);   // Activar solenoide para subir
	}
}

// Funci�n para mover el pl�ter en las direcciones X e Y
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
		for(int i = 0; i < 20; i++){
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
		for(int i = 0; i < 20; i++){
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
		for(int i = 0; i < 20; i++){
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
		for(int i = 0; i < 20; i++){
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
		_delay_ms(100);
		PORTD &= ~(1 << PD4); // Desactivar PD4
		break;
		case 11:  // Mover hacia arriba
		PORTD |= (1 << PD5);  // Activar PD5 (Arriba)
		_delay_ms(100);
		PORTD &= ~(1 << PD5); // Desactivar PD5
		break;
		case 12:  // Mover hacia la izquierda
		PORTD |= (1 << PD6);  // Activar PD6 (Izquierda)
		_delay_ms(100);
		PORTD &= ~(1 << PD6); // Desactivar PD6
		break;
		case 13:  // Mover hacia la derecha
		PORTD |= (1 << PD7);  // Activar PD7 (Derecha)
		_delay_ms(100);
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
}

void dibujar_circulo() {
	Centrar();
	mover_solenoide(1);
	_delay_ms(1500);
	
	mover_plotter(13);
	mover_plotter(13);
	mover_plotter(13);
	
	mover_plotter(10);
	
	mover_plotter(13);
	mover_plotter(13);
	
	mover_plotter(10);
	
	mover_plotter(13);
	mover_plotter(13);
	
	mover_plotter(10);
	
	mover_plotter(13);
	
	mover_plotter(10);
	mover_plotter(10);
	
	mover_plotter(13);
	
	mover_plotter(10);
	mover_plotter(10);
	
	mover_plotter(13);
	
	mover_plotter(10);
	mover_plotter(10);
	mover_plotter(10);
	mover_plotter(10);
	mover_plotter(10);
	mover_plotter(10);
	
	mover_plotter(12);
	
	mover_plotter(10);
	mover_plotter(10);
	
	mover_plotter(12);
	
	mover_plotter(10);
	mover_plotter(10);
	
	mover_plotter(12);
	
	mover_plotter(10);
	
	mover_plotter(12);
	mover_plotter(12);
	
	mover_plotter(10);
	
	mover_plotter(12);
	mover_plotter(12);
	
	mover_plotter(10);
	
	mover_plotter(12);
	mover_plotter(12);
	mover_plotter(12);
	mover_plotter(12);
	mover_plotter(12);
	mover_plotter(12);
	
	mover_plotter(11);
	
	mover_plotter(12);
	mover_plotter(12);
	
	mover_plotter(11);
	
	mover_plotter(12);
	mover_plotter(12);
	
	mover_plotter(11);
	
	mover_plotter(12);
	
	mover_plotter(11);
	mover_plotter(11);
	
	mover_plotter(12);
	
	mover_plotter(11);
	mover_plotter(11);
	
	mover_plotter(12);
	
	mover_plotter(11);
	mover_plotter(11);
	mover_plotter(11);
	mover_plotter(11);
	mover_plotter(11);
	mover_plotter(11);
	
	mover_plotter(13);
	
	mover_plotter(11);
	mover_plotter(11);
	
	mover_plotter(13);
	
	mover_plotter(11);
	mover_plotter(11);
	
	mover_plotter(13);
	
	mover_plotter(11);
	
	mover_plotter(13);
	mover_plotter(13);
	
	mover_plotter(11);
	
	mover_plotter(13);
	mover_plotter(13);
	
	mover_plotter(11);
	
	mover_plotter(13);
	mover_plotter(13);
	mover_plotter(13);
	
	_delay_ms(1500);
	mover_solenoide(0);
	
}

void dibujar_Flor(){
	Centrar();
	mover_solenoide(1);  // Bajar solenoide
	_delay_ms(1500);
	// Secuencia de movimientos del pl�ter
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

void dibujar_Zorro(){
	Centrar();
	mover_solenoide(1);
	_delay_ms(1500);
	mover_plotter(5);
	mover_plotter(5);
	mover_plotter(5);
	mover_plotter(5);
	mover_plotter(5);
	mover_plotter(0);
	mover_plotter(7);
	mover_plotter(6);
	mover_plotter(6);
	mover_plotter(1);
	mover_plotter(1);
	mover_plotter(1);
	mover_plotter(7);
	mover_plotter(7);
	mover_plotter(3);
	mover_plotter(3);
	mover_plotter(7);
	mover_plotter(0);
	mover_plotter(0);
	mover_plotter(4);
	mover_plotter(0);
	mover_plotter(5);
	mover_plotter(5);
	mover_plotter(0);
	mover_plotter(0);
	mover_plotter(4);
	mover_plotter(0);
	mover_plotter(4);
	mover_plotter(4);
	mover_plotter(2);
	mover_plotter(6);
	mover_plotter(4);
	mover_plotter(2);
	mover_plotter(2);
	mover_plotter(2);
	mover_plotter(2);
	mover_plotter(6);
	mover_plotter(4);
	mover_plotter(2);
	mover_plotter(2);
	mover_plotter(7);
	mover_plotter(1);
	mover_plotter(1);
	mover_plotter(1);
	mover_plotter(6);
	mover_plotter(1);
	mover_plotter(1);
	mover_plotter(1);
	mover_plotter(7);
	mover_plotter(2);
	mover_plotter(2);
	mover_plotter(6);
	mover_plotter(6);
	mover_plotter(6);
	mover_plotter(1);
	mover_plotter(3);
	mover_plotter(3);
	mover_plotter(3);
	mover_plotter(7);
	mover_plotter(7);
	mover_plotter(1);
	mover_plotter(1);
	mover_plotter(5);
	mover_plotter(5);
	mover_plotter(3);
	mover_plotter(1);
	mover_plotter(1);
	mover_plotter(5);
	mover_plotter(5);
	mover_plotter(0);
	mover_plotter(0);
	mover_plotter(0);
	mover_plotter(0);
	mover_plotter(4);
	mover_plotter(4);
	mover_solenoide(0);
	_delay_ms(1500);
}
void dibujar_circulo_cuadrado(int radio);
void dibujar_movimientos(int x, int y);
void mover_plotter_en_xy(int x, int y);

int main(void) {
	// Configurar los pines
	init_pins();
	// Configurar UART con la velocidad de baudios de 9600
	UART_init(103);
	char Opcion;
	
	while(1){
		_delay_ms(1000);
		UART_sendString("* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * \n");
		UART_sendString(" \n");
		UART_sendString("Bienvenido al programa para dibujar sobre el Plotter con ATmega328p\r\n");
		UART_sendString(" \n");
		UART_sendString("* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * \n");
		UART_sendString(" \n");
		UART_sendString("Seleccione una de las siguientes opciones: \n");
		UART_sendString("1- Dibujar Tri�ngulo \n");
		UART_sendString("2- Dibujar C�rculo \n");
		UART_sendString("3- Dibujar Cruz \n");
		UART_sendString("4- Dibujar Zorro \n");
		UART_sendString("5- Dibujar Flor \n");
		UART_sendString("6- Dibujo personalizado \n");
		
		Opcion = UART_receiveChar();
		if (Opcion == '1'){
			UART_sendString("Dibujando Tri�ngulo: \n");
			dibujar_Triangulo();
			UART_sendString("Tri�ngulo Finalizado: \n");
		}
		if (Opcion == '2'){
			UART_sendString("Dibujando C�rculo: \n");
			dibujar_circulo();
			UART_sendString("C�rculo Finalizado: \n");
		}
		if (Opcion == '3'){
			UART_sendString("Dibujando Cruz: \n");
			dibujar_cruz();
			UART_sendString("Cruz Finalizado: \n");
		}
		if (Opcion == '4'){
			UART_sendString("Dibujando Zorro: \n");
			dibujar_Zorro();
			UART_sendString("Zorro Finalizado: \n");
		}
		if (Opcion == '5'){
			UART_sendString("Dibujando Flor: \n");
			dibujar_Flor();
			UART_sendString("Flor Finalizado: \n");
		}
		if (Opcion == '6'){
			UART_sendString("* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * \n");
			UART_sendString("Lea las siguientes instrucciones para dibujar: \n");
			UART_sendString("w- Mover el puntero hacia arriba: \n");
			UART_sendString("s- Mover el puntero hacia abajo: \n");
			UART_sendString("d- Mover el puntero hacia la derecha: \n");
			UART_sendString("a- Mover el puntero hacia la izquierda: \n");
			UART_sendString("0- Volver al men� principal: \n");
			UART_sendString("Centrando... ");
			Centrar();
			mover_solenoide(1);
			UART_sendString("Listo!\n");
			_delay_ms(1500);
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

void dibujar_circulo_cuadrado(int radio) {
	Centrar();  // Centrar antes de dibujar
	mover_solenoide(1);  // Bajar solenoide
	_delay_ms(1500);

	int x = radio;
	int y = 0;
	int error = 0;

	// Dibujar el c�rculo solo con movimientos en los ejes
	while (x >= y) {
		// Dibujar los cuadrantes usando solo movimientos en X y Y
		dibujar_movimientos(x, y);
		
		y++;
		error += 2*y + 1;
		if (2*(error - x) + 1 > 0) {
			x--;
			error -= 2*x + 1;
		}
	}

	mover_solenoide(0);  // Subir solenoide
}

void dibujar_movimientos(int x, int y) {
	// Dibujar los cuadrantes del c�rculo con movimientos discretos
	mover_plotter_en_xy(x, y);   // (x, y)
	mover_plotter_en_xy(-x, y);  // (-x, y)
	mover_plotter_en_xy(x, -y);  // (x, -y)
	mover_plotter_en_xy(-x, -y); // (-x, -y)
	mover_plotter_en_xy(y, x);   // (y, x)
	mover_plotter_en_xy(-y, x);  // (-y, x)
	mover_plotter_en_xy(y, -x);  // (y, -x)
	mover_plotter_en_xy(-y, -x); // (-y, -x)
}

void mover_plotter_en_xy(int x, int y) {
	// Esta funci�n mueve el pl�ter solo en direcciones discretas (arriba, abajo, izquierda, derecha)
	if (x > 0) {
		for (int i = 0; i < x; i++) {
			mover_plotter(3);  // Mover hacia la derecha
		}
		} else {
		for (int i = 0; i < -x; i++) {
			mover_plotter(2);  // Mover hacia la izquierda
		}
	}

	if (y > 0) {
		for (int i = 0; i < y; i++) {
			mover_plotter(1);  // Mover hacia arriba
		}
		} else {
		for (int i = 0; i < -y; i++) {
			mover_plotter(0);  // Mover hacia abajo
		}
	}
}