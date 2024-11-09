/*
 * main.c
 *
 * Created: 10/29/2024 6:04:45 PM
 *  Author: Facundo
 */ 

#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <string.h>
#include <avr/eeprom.h>

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
void Bienvenida();
void Enviar_Mensaje(const char* Cadena1, const char* Cadena2);
void almacenar_Contrasena(char* password);
int verificar_Contrasena(char* password);
void abrir_Cerradura();
void cambiar_Contrasena(char* new_password);
void activar_Alarma();
void inicializar_EEPROM();

int state = 0;   // Estado inicial
char pass[4];    
char new_password[4];
char password[6]; 
char password_ref[4];
int intentos = 0; 

int main(void) {
	init_Pins();
	init_LCD();
	inicializar_EEPROM();
	
	char caracter = '\0';
	int contador = 0;
	
	Bienvenida();
	
	while (1) {
		LCD_cmd(0x01);
		if (state == 0) {                    // Bienvenida
			Bienvenida();
			comprobar_Teclado(&state, &caracter);
			_delay_ms(500);
			
		} else if (state == 1) {             // Menu de opciones
			Enviar_Mensaje("* : Abrir", "# : Cambiar PASS");
			comprobar_Teclado(&state, &caracter);
			_delay_ms(500);
			
		} else if (state == 2) {             // Solicitar Contraseña para abrir
			if(contador == 0){
				Enviar_Mensaje("Ingrese PASS", "     ");
				_delay_ms(500);
			}
			comprobar_Teclado(&state, &caracter);
			if(caracter != '\0' && contador < 4){ 
				pass[contador] = '*';
				password[contador] = caracter; 
				Enviar_Mensaje("Ingrese PASS", pass);
				_delay_ms(500);
				contador++;
			}
			if(contador == 4 || contador == 4){
				if(verificar_Contrasena(password)){
					PORTB |= (1 << LED_Verde);
					PORTB &= ~(1 << LED_Rojo);
					Enviar_Mensaje("Contraseña", "Correcta");
					abrir_Cerradura();
					state = 0;
					_delay_ms(3000);
				} else {
					Enviar_Mensaje("Contraseña", "Incorrecta");
					intentos++;
					if (intentos >= 3) {
						PORTB |= (1 << LED_Rojo);
						activar_Alarma();
						PORTB &= ~(1 << LED_Rojo);
						intentos = 0; 
					} else {
						PORTB |= (1 << LED_Rojo);
						_delay_ms(3000);
						PORTB &= ~(1 << LED_Rojo);
					}

					state = 0; 
				}
				contador = 0;
				for(int i=0; i<4 ; i++){
					pass[i] = "";
				}
				LCD_cmd(0x01);
			}
		} else if (state == 3){				 // Solicitar Contraseña para cambiar
			if(contador == 0){
				Enviar_Mensaje("Ingrese PASS", " ");
				_delay_ms(500);
			}
			comprobar_Teclado(&state, &caracter);
			if(caracter != '\0' && contador < 4){
				pass[contador] = '*';
				password[contador] = caracter; 
				Enviar_Mensaje("Ingrese PASS", pass);
				_delay_ms(500);
				contador++;
			}
			if(contador == 4 || contador == 4){ 
				pass[contador] = '\0'; 
				if(verificar_Contrasena(password)){
					Enviar_Mensaje("Ingrese nueva", "Contraseña");
					_delay_ms(500);
					contador = 0; 
					state = 4; 
				} else {
					Enviar_Mensaje("Contraseña", "Incorrecta");
					intentos++;
					if (intentos >= 3) {
						PORTB |= (1 << LED_Rojo);
						activar_Alarma();
						PORTB &= ~(1 << LED_Rojo);
						intentos = 0;
						} else {
						PORTB |= (1 << LED_Rojo);
						_delay_ms(3000);
						PORTB &= ~(1 << LED_Rojo);
					}
					state = 0; 
					contador = 0; 
				}
				for(int i=0; i<4 ; i++){
					pass[i] = "";
				}
			}
		} else if (state == 4) { // Ingreso de nueva contraseña
			if(contador == 0){
				Enviar_Mensaje("Ingrese nueva", "Contraseña");
				_delay_ms(500);
			}
			comprobar_Teclado(&state, &caracter);
			if(caracter != '\0' && contador < 4){
				new_password[contador] = caracter; 
				Enviar_Mensaje("Nueva PASS", new_password);
				_delay_ms(500);
				contador++;
			}
			if(contador == 4 || contador == 4){ 
				cambiar_Contrasena(new_password);
				state = 0; 
				contador = 0;
				for(int i=0; i<4 ; i++){
					new_password[i] = "";
				} 
			}
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
	LCD_cmd(0x28);  // Inicializar en 4 bit
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
	PORTC = 0x0F;  // Configurar el puerto C para lectura (con pull-up)
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
	for (int i = 0; i < 4; i++){
		PORTD &= ~(1 << Fila[i]);
		_delay_us(5);
		for (int j = 0; j < 4; j++) {
			if (!(PINC & (1 << Col[j]))) {
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
		*caracter = Teclado[Fila_][Columna_];
		
		if (*state == 0 && *caracter == 'A') {
			*state = 1;
		}
		
		if (*state == 1) {
			if (*caracter == '*') {
				*state = 2;
			} else if (*caracter == '#') {
				*state = 3;
			}
		}
	} else {
		*caracter = '\0';
	}
}

void Bienvenida() {
	Enviar_Mensaje("Bienvenido", "Ingrese A");
}

void Enviar_Mensaje(const char* Cadena1, const char* Cadena2) {
	LCD_cmd(0x01);
	for (int i = 0; i < 16 && Cadena1[i] != '\0'; i++) {
		LCD_write(Cadena1[i]);
	}
	LCD_cmd(0xC0);
	for (int i = 0; i < 16 && Cadena2[i] != '\0'; i++) {
		LCD_write(Cadena2[i]);
	}
}

void almacenar_Contrasena(char* password) {
	eeprom_write_block(password, &password_ref, sizeof(password_ref));
}

int verificar_Contrasena(char* password) {
	char stored_password[5];
	eeprom_read_block((void*)stored_password, (const void*)&password_ref, sizeof(stored_password) - 1);
	stored_password[4] = '\0';

	if (strcmp(password, stored_password) == 0) {
		return 1;
	}
	return 0;
}

void abrir_Cerradura() {
	Enviar_Mensaje("Abriendo...", "Cerradura");
	PORTB |= (1 << LED_Verde);
	_delay_ms(3000);
	PORTB &= ~(1 << LED_Verde);
}

void cambiar_Contrasena(char* new_password) {
	if (strlen(new_password) >= 4 && strlen(new_password) <= 6) { 
		almacenar_Contrasena(new_password);
		Enviar_Mensaje("Contraseña", "Cambiada");
		_delay_ms(1000);
		} else {
		Enviar_Mensaje("Error:", "Longitud");
		_delay_ms(1000);
	}
}

void activar_Alarma() {
	PORTC |= (1 << Buzzer); // Activar el buzzer
	_delay_ms(5000); // Sonar por 5 segundos
	PORTC &= ~(1 << Buzzer); // Desactivar el buzzer
}

void inicializar_EEPROM() {
	char default_password[5] = "1234";
	char stored_password[5];
	eeprom_read_block(stored_password, &password_ref, sizeof(stored_password) - 1);
	stored_password[4] = '\0';

	if (strcmp(stored_password, default_password) != 0) {
		eeprom_write_block((const void*)default_password, (void*)&password_ref, sizeof(default_password) - 1);
	}
}