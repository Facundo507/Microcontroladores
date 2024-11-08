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

//Estas funciones son las básicas para activar el solenoide y mover por los 2 ejes al plotter.

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

//definicion de las interrupción int0 e int1, el eicra no es necesario activarlo, se deja en cero para que se active con un nivel bajo la interrupción,
// ya que el sensor de carrera se activa con un cero lógico

void int01_init(){
	DDRD &= ~(1 << PD2); //PD2 como entrada (INT0)
	
	
	// Condiguracion interrupcion INT0 cuando haya un LOW
	
	EIMSK |= (1 << INT0) | (1 << INT1);
	sei();
}
#define Time 2000

//Estas funciones generar el reloj para las funciones simples, arriba, abajo derecha izquierda, SE,SO,NE,NO, etc
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
//Reloj especial que permite cambiar de forma rústica el ángulo, pendiente es el valor que se le da a la pendiente a uno de los dos ejes, y la coma es por ejemplo si tengo una pendiente de
// 1,5 veces, puedo agregar Achicador 2, y coma 3, para que entre en el bucle en los múltiplos de 2 y 3.
//En el caso de RelojEspecialX, se le achica la pendiente del eje X con respecti al Y. o que es lo mismo, que El eje Y aumenta la pendiente con respecto a X.

void RelojEspecialX(int Tiempo, int Pendiente, int Coma){ //Si no hay coma, poner el mismo valor que en Achicador.
		for (int i = 0; i <= Tiempo; i++){
			PORTC &= ~(1 << RelojY);
			if ((i % Pendiente == 0) || (i % Coma == 0)){
				PORTB &= ~(1 << RelojX);
			}
			_delay_ms(1);
			PORTC |= (1 << RelojY);
			if ((i % Pendiente == 0) || (i % Coma == 0)){
				PORTB |= (1 << RelojX);
			}
			_delay_ms(1);
		}

			
		
}

void RelojEspecialY(int Tiempo, int Pendiente, int Coma){
	for (int i = 0; i <= Tiempo; i++){
		PORTB &= ~(1 << RelojX);
		
		if ((i % Pendiente == 0) || (i % Coma == 0)){
			PORTC &= ~(1 << RelojY);
		}
		_delay_ms(1);
		PORTB |= (1 << RelojX);
		if ((i % Pendiente == 0) || (i % Coma == 0)){
			PORTC |= (1 << RelojY);
		}
		_delay_ms(1);
	}

	
	
}
//Las interrupciones que permiten cambiar de sentido al plotter y moviendolo un ratito en el sentido cambiado, para que no llegue al limite del mismo.
ISR(INT0_vect) {
	
	//Sensor YV4, el que está mas al Norte del plotter.
	
	HabilitarX();
	AbajoX();
	Reloj(20);
	
	Centrador = 1;
	PORTD |= (1 << LED);
	_delay_ms(500);
	PORTD &= ~(1 << LED);
}
ISR(INT1_vect) {
	//El que está más al sur del plotter., o mas abajo
	HabilitarX();
	ArribaX();
	Reloj(20);
	HabilitarX();
	
	_delay_ms(100);
	PORTD |= (1 << LED);
	_delay_ms(500);
	PORTD &= ~(1 << LED);
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

//Con la función relojEspecial en el circulo se pudo implementar la rotación
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
}//

void Zorro(){
	BajarSolenoide();
	HabilitarX();
	HabilitarY();
	DerechaY();
	AbajoX();
	Reloj(50);
	desHabilitarX();
	Reloj(200);
	HabilitarX();
	ArribaX();
	Reloj(50);
	AbajoX();
	Reloj(50);
	desHabilitarX();
	Reloj(50);
	HabilitarX();
	ArribaX();
	Reloj(100);
	desHabilitarY();
	ArribaX();
	Reloj(50);
	HabilitarY();
	DerechaY();
	Reloj(50);
	desHabilitarY();
	ArribaX();
	Reloj(100);
	HabilitarY();
	IzquierdaY();
	Reloj(100);
	desHabilitarY();
	ArribaX();
	Reloj(50);
	HabilitarY();
	DerechaY();
	Reloj(50);
	desHabilitarY();
	Reloj(100);
	AbajoX();
	HabilitarY();
	IzquierdaY();
	Reloj(50);
	desHabilitarX();
	Reloj(100);
	HabilitarX();
	AbajoX();
	Reloj(100);
	desHabilitarY();
	AbajoX();
	Reloj(150);
	HabilitarY();
	DerechaY();
	Reloj(100);
	IzquierdaY();
	Reloj(50);
	desHabilitarY();
	ArribaX();
	Reloj(50);
	HabilitarY();
	IzquierdaY();
	Reloj(300);
	DerechaY();
	Reloj(100);
	desHabilitarY();
	ArribaX();
	Reloj(200);
	HabilitarY();
	IzquierdaY();
	Reloj(100);
	desHabilitarY();
	AbajoX();
	Reloj(100); //hasta aca bien
	desHabilitarX();
	HabilitarY();
	IzquierdaY();
	Reloj(50);
	ArribaX();
	HabilitarX();
	Reloj(100);
	desHabilitarY();
	AbajoX();
	Reloj(100);
	HabilitarY();
	Reloj(100);
	desHabilitarX();
	Reloj(150);
	desHabilitarY();
	HabilitarX();
	AbajoX();
	Reloj(50);
	DerechaY();
	HabilitarY();
	Reloj(150);
	desHabilitarX();
	Reloj(100);
	HabilitarX();
	AbajoX();
	IzquierdaY();
	HabilitarY();
	Reloj(50);
	desHabilitarY();
	Reloj(150);
	DerechaY();
	HabilitarY();
	Reloj(50);
	desHabilitarY();
	Reloj(150);
	IzquierdaY();
	HabilitarY();
	Reloj(50);
	HabilitarY();
	DerechaY();
	Reloj(50);
	desHabilitarX();
	DerechaY();
	Reloj(200);
	
	
	
	
	
	desHabilitarY();
	desHabilitarX();
	SubirSolenoide();
	}
	//Función del zorro, esta función motivó crear luego las funciones N,S,E,O, etc ya que se esc´ribía más codigo sin ellas.


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
//Funcion para centrar el plotter en el medio, no fue muy utilizada
void N(int Tiempo){
	desHabilitarY();
	ArribaX();
	HabilitarX();
	Reloj(Tiempo);
	desHabilitarX();
	desHabilitarY();
}
void S(int Tiempo){
	desHabilitarY();
	AbajoX();
	HabilitarX();
	Reloj(Tiempo);
	desHabilitarX();
	desHabilitarY();
}
void E(int Tiempo){
	desHabilitarX();
	DerechaY();
	HabilitarY();
	Reloj(Tiempo);
	desHabilitarX();
	desHabilitarY();
}
void O(int Tiempo){
	desHabilitarX();
	IzquierdaY();
	HabilitarY();
	Reloj(Tiempo);
	desHabilitarX();
	desHabilitarY();
}
void NE(int Tiempo){
	DerechaY();
	ArribaX();
	HabilitarY();
	HabilitarX();
	Reloj(Tiempo);
	desHabilitarX();
	desHabilitarY();
}
void NO(int Tiempo){
	IzquierdaY();
	ArribaX();
	HabilitarY();
	HabilitarX();
	Reloj(Tiempo);
	desHabilitarX();
	desHabilitarY();
}
void SEste(int Tiempo){
	DerechaY();
	AbajoX();
	HabilitarY();
	HabilitarX();
	Reloj(Tiempo);
	desHabilitarX();
	desHabilitarY();
}
void SO(int Tiempo){
	IzquierdaY();
	AbajoX();
	HabilitarY();
	HabilitarX();
	Reloj(Tiempo);
	desHabilitarX();
	desHabilitarY();
}
// las funciones O, NE , NO SEste y SO simplifican el hecho de tener que escribir más codigo que las de arriba, abajo, derecha e izquierda, no es necesario escribir habilitar e inhabilitar con estas funciones.
// ni escribir reloj, solo se le manda como parametro el tiempo a la misma funcion, pero solo se podrá mover en esos sentidos
void Triangulo(void){
	BajarSolenoide();
	HabilitarX();
	HabilitarY();
	
	ArribaX();
	DerechaY();
	RelojEspecialY(1000,2,2);
	AbajoX();
	RelojEspecialY(1000,2,2);
	desHabilitarX();
	O(1000);
	
	
	SubirSolenoide();
	desHabilitarX();
	desHabilitarY();
	
	
}

void Flor(){
	BajarSolenoide();
	SO(250);
	NO(250);
	S(400);
	
	//Curva de 180 grados para el tulipán o flor
	HabilitarX();
	HabilitarY();
	AbajoX();
	DerechaY();
	RelojEspecialY(40,12,12);
	RelojEspecialY(45,4,4);
	RelojEspecialY(45,2,2);
	RelojEspecialY(25,2,3);
	RelojEspecialY(25,1,1);
	RelojEspecialX(25,1,1);
	RelojEspecialX(25,2,3);
	RelojEspecialX(45,2,2);
	RelojEspecialX(45,4,4);
	RelojEspecialX(40,12,12);
	
	ArribaX();
	
	RelojEspecialX(40,12,12);
	RelojEspecialX(45,4,4);
	RelojEspecialX(45,2,2);
	RelojEspecialX(25,2,3);
	RelojEspecialX(25,1,1);
	RelojEspecialY(25,1,1);
	RelojEspecialY(25,2,3);
	RelojEspecialY(45,2,2);
	RelojEspecialY(45,4,4);
	RelojEspecialY(40,12,12);
	//Fin Curva
	
	
	N(400);
	SO(250);
	SubirSolenoide();
	S(400);
	BajarSolenoide();
	S(150);
	NO(100);
	S(150);
	SEste(100);
	S(400);
	SubirSolenoide();
	N(200);
	BajarSolenoide();
	NE(100);
	N(150);
	SO(100);
	N(250);

	
	
	SubirSolenoide();
	desHabilitarY();
	desHabilitarY();
}

void MostrarTodo(){ // Función que muestra todas las figuras
	Cuadrado();
	E(1200);
	Triangulo();
	E(1700);
	Circulo();
	SO(500);
	S(500);
	Zorro();
	O(1500);
	N(800);
	Flor();
	
	NO(1200);
	
}

int main(void) {
	SubirSolenoide();
	DDRB |= (1 << RelojX) | (1 << DirX) | (1 << EnableX);
	DDRC |= (1 << Solenoide) | (1 << RelojY) | (1 << DirY) | (1 << EnableY);
	DDRD |= (1 << LED);
	_delay_ms(2000);
	int01_init();             // Inicializa la interrupción INT0
	
	//E(1200);
	//O(1000);
	//N(1500);
	//MostrarTodo();
	
	MostrarTodo();
	
	while (1) {
		
		
}
}