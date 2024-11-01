/*
 * main.c
 *
 * Created: 10/29/2024 6:04:45 PM
 *  Author: Facundo
 */ 

#define F_CPU 16000000UL
#include <xc.h>
#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>

#define Enable PD7
#define RW PD6
#define RS PD5
#define Fila1 PD4
#define Fila2 PD3
#define Fila3 PD2
#define Fila4 PD1
#define Col1 PC0
#define Col2 PC1
#define Col3 PC2
#define Col4 PC3
#define LED_Rojo PB4
#define LED_Verde PB5
#define Buzzer PC4

void LCD_cmd(char cmd);
void init_LCD();
void init_Pins();
void LCD_write(char data);
void comprobar_Teclado(int* state, char* caracter);
void Bienvenida(char* Cadena1, char* Cadena2);
void Enviar_Mensaje(char* Cadena1, char* Cadena2);

int main(void) {
	init_Pins();
	init_LCD();
	int state = 0;
	char PasswordReference[6] = "1234";
	char Password[6] = "";  
	char caracter = '\0'; 
	int contador = 0;
	char Cadena1[16];
	char Cadena2[16];
	Bienvenida(Cadena1, Cadena2);
	while (1) {
		if (state == 0) {					// Bienvenida
			Bienvenida(Cadena1, Cadena2);
			comprobar_Teclado(&state, &caracter);
			Enviar_Mensaje(Cadena1, Cadena2);
			_delay_ms(500);
		} else if (state == 1) {			// Menú
			sprintf(Cadena1, "* : Abrir");
			sprintf(Cadena2, "# : Cambiar");
			comprobar_Teclado(&state, &caracter);
			Enviar_Mensaje(Cadena1, Cadena2);
			_delay_ms(500);
			sprintf(Cadena2, "");
		} else if (state == 2) {			// Abrir Cerradura
			sprintf(Cadena1, "Ingrese su PASS");
			comprobar_Teclado(&state, &caracter);
			if (caracter != '\0') {
				Password[contador] = caracter;

				for (int i = 0; i < contador; i++) {
					Cadena2[i] = '*';
				}
				caracter = '\0';
			}
			Enviar_Mensaje(Cadena1, Cadena2);
			_delay_ms(500);
		} else if (state == 3) {			// Cambiar Contraseña
			sprintf(Cadena1, "Ingrese su PASS");
			sprintf(Cadena2, "");
			comprobar_Teclado(&state, &caracter);
			Enviar_Mensaje(Cadena1, Cadena2);
			_delay_ms(500);
		} else if (state == 4) {				// Contraseña Incorrecta
			sprintf(Cadena1, "PASS incorrecta");
			sprintf(Cadena2, "Tries Left: ");
			comprobar_Teclado(&state, &caracter);
			Enviar_Mensaje(Cadena1, Cadena2);
			_delay_ms(500);
		} else if (state == 5) {				// Abriendo Cerradura
			//
		} else if (state == 6) {				// Contraseña Cambiada
			//
		}
	}
}

void LCD_cmd(char cmd) {
	PORTB = (cmd >> 4) & 0x0F; // Enviar los 4 bits más significativos
	PORTD &= ~(1 << RS);
	PORTD &= ~(1 << RW);
	PORTD |= (1 << Enable);
	_delay_ms(1);
	PORTD &= ~(1 << Enable);

	PORTB = cmd & 0x0F; // Enviar los 4 bits menos significativos
	PORTD |= (1 << Enable);
	_delay_ms(1);
	PORTD &= ~(1 << Enable);
}

void init_LCD() {
	_delay_ms(50);

	LCD_cmd(0x33);
	_delay_ms(50);

	LCD_cmd(0x32);
	_delay_ms(50);

	LCD_cmd(0x28); // Inicializar en 4 bit
	_delay_ms(50);

	LCD_cmd(0x0C);
	_delay_ms(50);

	LCD_cmd(0x06);
	_delay_ms(50);

	LCD_cmd(0x01);
	_delay_ms(50);

	LCD_cmd(0x80);
	_delay_ms(50);
}

void init_Pins() {
	DDRD = 0xFF;
	DDRB = 0xFF;
	DDRC = 0xF0;
	PORTC = 0x0F; // Configurar el puerto C para lectura (con pull-up)
}

void LCD_write(char data) {
	PORTB = (data >> 4) & 0x0F; // Enviar los 4 bits más significativos
	PORTD |= (1 << RS);
	PORTD &= ~(1 << RW);
	PORTD |= (1 << Enable);
	_delay_ms(1);
	PORTD &= ~(1 << Enable);

	PORTB = data & 0x0F; // Enviar los 4 bits menos significativos
	PORTD |= (1 << Enable);
	_delay_ms(1);
	PORTD &= ~(1 << Enable);
}

void comprobar_Teclado(int* state, char* caracter) {
	int Columna_ = -1;
	int Fila_ = -1;
	int Fila[4] = {Fila1, Fila2, Fila3, Fila4};
	int Col[4] = {Col1, Col2, Col3, Col4};
	char Teclado[4][4] = {
		{'1', '2', '3', 'A'},
		{'4', '5', '6', 'B'},
		{'7', '8', '9', 'C'},
		{'*', '0', '#', 'D'}
	};
	
	PORTD |= (1 << Fila1) | (1 << Fila2) | (1 << Fila3) | (1 << Fila4);
	for (int i = 0; i < 4; i++) {
		PORTD &= ~(1 << Fila[i]);
		_delay_us(5);
		for (int j = 0; j < 4; j++) {
			if (!(PINC & (1 << Col[j]))) { // Se detecta la tecla cuando el PIN está en LOW
				Columna_ = j;
				Fila_ = i;
				break;
			}
		}
		PORTD |= (1 << Fila[i]);
		if (Columna_ != -1) {
			break;
		}
	}
	
	if (Columna_ != -1) {
		if(*state == 0 && Teclado[Fila_][Columna_] == 'A') {
			*state = 1;
		} else if (*state == 1) {
			if(Teclado[Fila_][Columna_] == '*') {
				*state = 2;
			} else if(Teclado[Fila_][Columna_] == '#') {
				*state = 3;
			}
		} else if(*state == 2 || *state == 3) {
			*caracter = Teclado[Fila_][Columna_];
		} else if(*state == 4){
			
		} else if(*state == 5){
			
		} else if(*state == 6){
			
		}
	}
}

void Bienvenida(char* Cadena1, char* Cadena2) {
	sprintf(Cadena1, "Bienvenido");
	sprintf(Cadena2, "Presione A");
}

void Enviar_Mensaje(char* Cadena1, char* Cadena2) {
	LCD_cmd(0x01);
	for (int i = 0; i < 16 && Cadena1[i] != '\0'; i++) {
		LCD_write(Cadena1[i]);
	}
	LCD_cmd(0xC0);
	for (int i = 0; i < 16 && Cadena2[i] != '\0'; i++) {
		LCD_write(Cadena2[i]);
	}
}
