#define F_CPU 16000000UL
#include <xc.h>
#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>

#define MedioAmarillo ((SupAmarillo + InfAmarillo)/2)
#define MedioRojo ((SupRojo + InfRojo)/2)
#define MedioRosado ((SupRosado + InfRosado)/2)
#define MedioAzul ((SupAzul + InfAzul)/2)

#define SupAmarillo 239
#define InfAmarillo 220

#define SupRojo 270
#define InfRojo  240
#define SupRosado 300
#define InfRosado 271
#define SupAzul 390
#define InfAzul 320

// Entre 319 y 301 no se definió ningún color pero aun asi los colores azul y rosado se mostraban bien.

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



void UART_send16bitsComoString(uint16_t Entero1024) {
	char Cadena[6];  
	sprintf(Cadena, "%u", Entero1024); //envía a la cadena de char los valores de Entero1024
	UART_sendString(Cadena);        //Envía la cadena de caracteres
}

void SetupPWM(void){
	DDRB = 0xFF;
	TCCR1A = (1 << COM1A1) | (1 << COM1B1) | (1 << WGM11);
	TCCR1B = (1 << WGM13) | (1 << WGM12) | (0 << CS12) | (1 << CS11) | (1 << CS10);
	// los 3 menos significativos configuran el prescaler, en este
	// caso configurado en 1024, pin 3 y 4 configuracion del WGM1 para hacerlo en FASTPWM
	//
	ICR1 = 4999;
	OCR1A = 125; // Valor del Ciclo útil
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

int main(void){
	uint16_t ValorADC;
	
	SetupADC();
	SetupPWM();
	UART_init(103);
	while(1){
		
		ValorADC = LeerADC(0);
		UART_sendString("Valor de la fotocelda:");
		UART_send16bitsComoString(ValorADC);
		UART_sendString("\r\n");
		
		
		
		if ((ValorADC >= InfAmarillo) & (ValorADC <= SupAmarillo)){
		UART_sendString("Color leído: Amarillo");
		UART_sendString("\r\n");
		UART_sendString("Intervalo del color: (");
		UART_send16bitsComoString(InfAmarillo);
		UART_sendString(", ");
		UART_send16bitsComoString(SupAmarillo);
		UART_sendString(")");
		UART_sendString("\r\n");
		UART_sendString("Valor establecido del color: ");
		UART_send16bitsComoString(MedioAmarillo);
		UART_sendString("\r\n");
		UART_sendString("Diferencia con el valor medio establecido :");
		UART_send16bitsComoString(ValorADC-MedioAmarillo);
		
		UART_sendString("\r\n");
		
		OCR1A = 125;
		_delay_ms(300);
		
		UART_sendString("\r\n");
		UART_sendString("\r\n");
		
		}
		
		
		
		if ((ValorADC >= InfRojo) & (ValorADC <= SupRojo)){
			UART_sendString("Color leído: Rojo");
			UART_sendString("\r\n");
			UART_sendString("Intervalo del color: (");
			UART_send16bitsComoString(InfRojo);
			UART_sendString(", ");
			UART_send16bitsComoString(SupRojo);
			UART_sendString(")");
			UART_sendString("\r\n");
			UART_sendString("Valor establecido del color: ");
			UART_send16bitsComoString(MedioRojo);
			UART_sendString("\r\n");
			UART_sendString("Diferencia con el valor medio establecido :");
			
			UART_sendString("\r\n");
			UART_send16bitsComoString(ValorADC-MedioRojo);
			UART_sendString("\r\n");
			UART_sendString("\r\n");
			OCR1A = 375;
			_delay_ms(300);
		}
		if ((ValorADC >= InfRosado) & (ValorADC <= SupRosado)){
			UART_sendString("Color leído: Rosado");
			UART_sendString("\r\n");
			
			UART_sendString("Intervalo del color: (");
			UART_send16bitsComoString(InfRosado);
			UART_sendString(", ");
			UART_send16bitsComoString(SupRosado);
			UART_sendString(")");
			UART_sendString("\r\n");
			UART_sendString("Valor establecido del color: ");
			UART_send16bitsComoString(MedioRojo);
			UART_sendString("\r\n");
			UART_sendString("Diferencia con el valor medio establecido");
			
			
			UART_sendString("\r\n");
			UART_send16bitsComoString(ValorADC-MedioRosado);
			UART_sendString("\r\n");
			
			UART_sendString("\r\n");
			OCR1A = 500;
			_delay_ms(300);
		}
		if ((ValorADC >= InfAzul) & (ValorADC <= SupAzul)){
			UART_sendString("Color leído: Azul");
			UART_sendString("\r\n");
			
			UART_sendString("Intervalo del color: (");
			UART_send16bitsComoString(InfAzul);
			UART_sendString(", ");
			UART_send16bitsComoString(SupAzul);
			UART_sendString(")");
			UART_sendString("\r\n");
			UART_sendString("Valor establecido del color: ");
			UART_send16bitsComoString(MedioAzul);
			UART_sendString("\r\n");
			
			UART_sendString("Diferencia con el valor medio establecido:");
			
		
			UART_send16bitsComoString(ValorADC-MedioAzul);
			UART_sendString("\r\n");
			UART_sendString("\r\n");
			OCR1A = 625;
			_delay_ms(500);
			
			
		}
		
		
		
	}
	

	
}
