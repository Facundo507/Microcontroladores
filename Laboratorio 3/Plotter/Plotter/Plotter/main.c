/*
 * main.c
 *
 * Created: 11/3/2024 9:46:08 PM
 *  Author: dasil
 */ 
#define F_CPU 1000000UL
#include <xc.h>
#include <avr/io.h>

#include <util/delay.h>
#include <avr/interrupt.h>


#define RelojX PB3
#define DirX PB4
#define EnableX PB5

#define Solenoide PC0

#define RelojY PC3
#define DirY PC4
#define EnableY PC5

#define Limite_Y_A PD2
#define Limite_Y_D PD3
void BajarSolenoide(){
	PORTC &= ~((1 << Solenoide));
}
void SubirSolenoide(){
	PORTC |= ((1 << Solenoide));
}
void IzquierdaY(){
	PORTC &= ~(1 << DirY);
}

void DerechaY(){
	PORTC |= (1 << DirY);
}

void HabilitarX(){
	PORTB |= (1 << EnableX);
}
void HabilitarY(){
	PORTC |= (1 << EnableY);
}
void desHabilitarX(){
	PORTB &= ~(1 << EnableX);
	};
void desHabilitarY(){
	PORTC &= ~(1 << EnableY);
}
void AbajoX(){
	PORTB |= (1 << DirX);
}

void ArribaX(){
	PORTB &= ~(1 << DirX);
	}

#define LED PD5
uint8_t Centrador = 0;


void int01_init(){
	DDRD &= ~(1 << PD2); //PD2 como entrada (INT0)
	
	
	// Condiguracion interrupcion INT0 cuando haya un LOW
	
	EIMSK |= (1 << INT0) | (1 << INT1);
	sei();
}
#define Time 500
void ForcePWMY(void){
	
	PORTC &= ~(1 << RelojY);
	PORTB &= ~(1 << RelojX);
	_delay_us(Time);
	PORTC |= (1 << RelojY);
	PORTB |= (1 << RelojX);
	_delay_us(Time);
}
void Reloj(int Tiempo){
	for (int i = 0; i<Tiempo; i++){
		ForcePWMY();
	}
}
void RelojEspecialX(int Tiempo, int Achicador, int Coma){ //Si no hay coma, poner el mismo valor que en Achicador.
		for (int i = 0; i <= Tiempo; i++){
			PORTC &= ~(1 << RelojY);
			if ((i % Achicador == 0) || (i % Coma == 0)){
				PORTB &= ~(1 << RelojX);
			}
			_delay_us(400);
			PORTC |= (1 << RelojY);
			if ((i % Achicador == 0) || (i % Coma == 0)){
				PORTB |= (1 << RelojX);
			}
			_delay_us(400);
		}

			
		
}

void RelojEspecialY(int Tiempo, int Achicador, int Coma){
	for (int i = 0; i <= Tiempo; i++){
		PORTB &= ~(1 << RelojX);
		
		if ((i % Achicador == 0) || (i % Coma == 0)){
			PORTC &= ~(1 << RelojY);
		}
		_delay_ms(1);
		PORTB |= (1 << RelojX);
		if ((i % Achicador == 0) || (i % Coma == 0)){
			PORTC |= (1 << RelojY);
		}
		_delay_ms(1);
	}

	
	
}

ISR(INT0_vect) {
	
	//Sensor YV4, el del fondo.
	
	HabilitarX();
	AbajoX();
	Reloj(20);
	
	Centrador = 1;
	PORTD |= (1 << LED);
	_delay_ms(500);
	PORTD &= ~(1 << LED);
}
ISR(INT1_vect) {
	
	HabilitarX();
	ArribaX();
	Reloj(20);
	HabilitarX();
	
	_delay_ms(100);
	PORTD |= (1 << LED);
	_delay_ms(500);
	PORTD &= ~(1 << LED);
}


void TrianguloIs(void){
	BajarSolenoide();
	HabilitarX();
	HabilitarY();
	ArribaX();
	DerechaY();
	Reloj(1000);
	AbajoX();
	DerechaY();
	Reloj(1000);
	desHabilitarX();
	IzquierdaY();
	Reloj(2000);
	SubirSolenoide();
}

void Cuadrado(void){
	BajarSolenoide();
	desHabilitarY();
	HabilitarX();
	ArribaX();
	Reloj(1000);
	desHabilitarX();
	HabilitarY();
	DerechaY();
	Reloj(1000);
	desHabilitarY();
	HabilitarX();
	AbajoX();
	Reloj(1000);
	HabilitarY();
	desHabilitarX();
	IzquierdaY();
	Reloj(1000);
	desHabilitarX();
	desHabilitarY();
	SubirSolenoide();
}


void Circulo(void){
	HabilitarX();
	HabilitarY();
	BajarSolenoide();
	ArribaX();
	DerechaY();
	RelojEspecialX(80,12,12);
	RelojEspecialX(90,4,4);
	RelojEspecialX(90,2,2);
	RelojEspecialX(50,2,3);
	RelojEspecialX(50,1,1);
	
	
	RelojEspecialY(50,1,1);
	RelojEspecialY(50,2,3);
	RelojEspecialY(90,2,2);
	RelojEspecialY(90,4,4);
	RelojEspecialY(80,12,12);
	
	
	IzquierdaY();
	RelojEspecialY(80,12,12);
	RelojEspecialY(90,4,4);
	RelojEspecialY(90,2,2);
	RelojEspecialY(50,2,3);
	RelojEspecialY(50,1,1);
	
	RelojEspecialX(50,1,1);
	RelojEspecialX(50,2,3);
	RelojEspecialX(90,2,2);
	RelojEspecialX(90,4,4);
	RelojEspecialX(80,12,12);
	
	AbajoX();
	RelojEspecialX(80,12,12);
	RelojEspecialX(90,4,4);
	RelojEspecialX(90,2,2);
	RelojEspecialX(50,2,3);
	RelojEspecialX(50,1,1);
	RelojEspecialY(50,1,1);
	RelojEspecialY(50,2,3);
	RelojEspecialY(90,2,2);
	RelojEspecialY(90,4,4);
	RelojEspecialY(80,12,12);
	
	
	DerechaY();
	
	RelojEspecialY(80,12,12);
	RelojEspecialY(90,4,4);
	RelojEspecialY(90,2,2);
	RelojEspecialY(50,2,3);
	RelojEspecialY(50,1,1);
	RelojEspecialX(50,1,1);
	RelojEspecialX(50,2,3);
	RelojEspecialX(90,2,2);
	RelojEspecialX(90,4,4);
	RelojEspecialX(80,12,12);
	
	desHabilitarX();
	desHabilitarY();
	SubirSolenoide();
}


void CentradorX (void){
	while (Centrador != 1){
		HabilitarX();
		ArribaX();
		Reloj(1);
	}	
	Centrador = 0;
	AbajoX();
	Reloj(4500);
	desHabilitarX();
}

int main(void) {
	SubirSolenoide();
	DDRB |= (1 << RelojX) | (1 << DirX) | (1 << EnableX);
	DDRC |= (1 << Solenoide) | (1 << RelojY) | (1 << DirY) | (1 << EnableY);
	DDRD |= (1 << LED);
	_delay_ms(2000);
	int01_init();             // Inicializa la interrupción INT0
	//Circulo();
	//HabilitarY();
	//DerechaY();
	//Reloj(1500);
	HabilitarX();
	AbajoX();
	Reloj(100);
	Circulo();
	
	
	//RelojEspecial(1000);
	/*CentradorX();
	
	
	HabilitarX();
	AbajoX();
	Reloj(1000);
	desHabilitarX();
	HabilitarY();
	IzquierdaY();
	Reloj(800);
	Cuadrado();
	HabilitarY();
	DerechaY();
	Reloj(1200);
	TrianguloIs();
	*/
	
	while (1) {
				
		
}
}