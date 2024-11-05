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

#define AbajoX PORTB |= (1 << DirX);
#define ArribaX PORTB &= ~(1 << DirX);
#define HabilitarX PORTB |= (1 << EnableX);
#define HabilitarY PORTC |= (1 << EnableY);
#define desHabilitarX PORTB &= ~(1 << EnableX);
#define desHabilitarY PORTC &= ~(1 << EnableY);
#define DerechaY PORTC |= (1 << DirY);
#define IzquierdaY PORTC &= ~(1 << DirY);


#define LED PD5

void int0_init(){
	DDRD &= ~(1 << PD2); //PD2 como entrada (INT0)
	//PORTD |= (1 << PD2); //Resistencia pull-up
	
	// Condiguracion interrupcion INT0 flanco de subida.
	
	EICRA |= (1 << ISC01);
	EICRA |= (1 << ISC00);
	EICRA |= (1 << ISC11);
	EICRA |= (1 << ISC10);
	
	EIMSK |= (1 << INT0) | (1 << INT1);
	sei();
}

void ForcePWMY(void){
	if ((PIND & (1 << Limite_Y_D )) || (PIND & (1 << Limite_Y_A ))){
	PORTC &= ~(1 << RelojY);
	PORTB &= ~(1 << RelojX);
	_delay_ms(1);
	PORTC |= (1 << RelojY);
	PORTB |= (1 << RelojX);
	_delay_ms(1);
	}
}

ISR(INT0_vect) {
	desHabilitarX
	desHabilitarY
}
ISR(INT1_vect) {
	desHabilitarX
	desHabilitarY
}



int main(void) {
	DDRB |= (1 << RelojX) | (1 << DirX) | (1 << EnableX);
	DDRC |= (1 << Solenoide) | (1 << RelojY) | (1 << DirY) | (1 << EnableY);
	DDRD |= (1 << LED);
	
	int0_init();             // Inicializa la interrupción INT0
	while (1) {
		if ((PIND & (1 << Limite_Y_D )) || (PIND & (1 << Limite_Y_A ))){
			
			
			HabilitarX
			AbajoX
			PORTC |= ((1 << Solenoide));
			
			
			for (int i = 0; i<500; i++){
				ForcePWMY();
			}
			
			
			
		}
		
	}
}