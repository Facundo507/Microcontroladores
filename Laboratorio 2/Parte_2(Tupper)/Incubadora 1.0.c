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
#include <stdio.h>

#define EnCaliente 0 // Cuando las resistencias del puente de weathson calientan, la caracterización cambia:
#define Temp0 100 + EnCaliente
#define Temp22 160 + EnCaliente	
#define Temp30 165 + EnCaliente
#define Temp40 171 + EnCaliente
#define Temp50 179 + EnCaliente


/*

10 v AMPLIFICADOR
12 V MOTOR 6 V LUZ

62 
60
58.5 187 
55 
50.5 
50
57 
55 
54
53 182
52 
48.5 177
47 
45
44.5 175
43 173 
42 
40  
39 170
37  
36 
35  
34 

33 166
32
31 166
30
23 =
29 = 164
28 = 
27 =   
26 = 
25 =  162
24 = 
23 = 
22 = 160
21
20
19
18 158
17
16
15 = 


*/
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

int16_t Celsius(uint16_t ValorADC){
	switch (ValorADC)
	{
	case 188:
		return 58; 
		break;
		
	case 187:
	return 57;
	break;
	
	case 186:
	return 56;
	break;
	
	case 185:
	return 55;
	break;
	
	case 184:
	return 54;
	break;
	
	case 183:
	case 182:
	return 53;
	break;
	
	
	
	case 181:
	return 52;
	break;
	
	case 180:
	return 51;
	break;
	
	case 179:
	return 50;
	break;
	
	case 178:
	return 49;
	break;
	case 177:
	return 48;
	break;
	case 176:
	return 47;
	break;
	case 175:
	return 45;
	break;
	
	case 174:
	return 44;
	break;
	case 173:
	return 43;
	break;
	case 172:
	return 42;
	break;
	case 171:
	return 41;
	break;
	case 170:
	return 40;
	break;
	case 169:
	return 37;
	break;
	case 168:
	return 36;
	break;
	case 167:
	return 34;
	break;
	case 166:
	return 33;
	break;
	case 165:
	return 30;
	break;
	case 164:
	return 29;
	break;
	case 163:
	return 28;
	break;
	case 162:
	return 26;
	break;
	case 161:
	return 24;
	break;
	case 160:
	return 22;
	break;
	case 159:
	return 20;
	break;
	case 158:
	return 18;
	break;
	case 157:
	return 17;
	break;
	case 156:
	return 17;
	break;
	case 155:
	return 16;
	break;
	case 154:
	return 15;
	break;
	case 153:
	return 14;
	break;
	case 152:
	return 13;
	break;
	default:
	return -300;
	
	
	
	
	
	
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
	OCR1B = 0;
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
	int8_t TminIdeal = Temp22;
	int8_t TmaxIdeal = Temp30;
	
	SetupPWM();
	SetupADC();
	//uint16_t ValorADC;
	int8_t Segundo = 0;
	int8_t Minuto = 0;
	
	
	
	
	UART_init(103);
	UART_sendString("Bienvenido al programa, para cambiar los rangos de temperatura ideal presione la tecla A");
	UART_sendString("Por un minuto se calentará la incubadora");
	
	UART_sendString("\r\n");
	UART_sendString("____ Tiempo ____ ValorADC ___ Voltaje ___Temperatura ______ EstadoCalentador ______ EstadoVentilador\r\n ");
	UART_sendString("----------------------------");
	UART_sendString("\r\n");
	// OCR1B es el ventilador
    while(1)
    {
		uint16_t ValorADC = LeerADC(1);
		
		
		float Voltaje = ValorADC * (5.0 / 1023.0);
		
		Segundo = Segundo + 5;
		if (Segundo >= 60){
			Minuto++;
			Segundo = 0;
		}
		
		UART_sendString("  |  ");
		UART_send16bitsComoString(Minuto);
		UART_sendString(" : ");
		UART_send16bitsComoString(Segundo);
		UART_sendString("   |   ");
		UART_send16bitsComoString(ValorADC);
		UART_sendString("   |   ");
		CompararVoltaje(Voltaje);
		UART_sendString("   |   ");
		UART_send16bitsComoString(Celsius(ValorADC));
		UART_sendString("   |   ");
		
		
		if ((Segundo <= 55 ) & (Minuto < 2)){
			PORTB |= (1<< PORTB4);
		}
		else
		{
			if ((ValorADC >= Temp0) & (ValorADC < Temp22)) {
				UART_sendString(" Frio  |  ON  |   OFF         | ");
				
				PORTB |= (1<< PORTB4);
				OCR1A = 0;
			}
			if ((ValorADC >= Temp22) & (ValorADC < Temp30)) {
				UART_sendString("MEDIO  |  OFF  |   OFF        | ");
				PORTB &= ~(1<< PORTB4); //PONER Un cero
				OCR1A = 0;
			}
			
			if ((ValorADC >= Temp30) & (ValorADC < Temp40)) {
				UART_sendString("   |  OFF  |   Baja velocidad         | ");
				OCR1A = 1012;
				PORTB &= ~(1<< PORTB4); //PONER Un cero
			}
			if ((ValorADC >= Temp40) & (ValorADC < Temp50)) {
				UART_sendString("  |  OFF  |   Media velocidad        | ");
				OCR1A = 1016;
				PORTB &= ~(1<< PORTB4); //PONER Un cero
			}
			if (ValorADC >= Temp50) {
				UART_sendString("  |  OFF  |           Max velocidad          | ");
				OCR1A = 1023;
				PORTB &= ~(1<< PORTB4); //PONER Un cero
			}
			
		}
		UART_sendString("\r\n ");
		if (UCSR0A & (1 << RXC0)) {
			char receivedChar = UDR0;
			UART_sendString("Seleccionó la opción de Cambiar Intervalo de temperatura ideales: \r\n");
			UART_sendString("Seleccione temperatura mínimo ideal: \r\n");
			TminIdeal = UART_receiveChar();
			UART_sendString("Seleccione temperatura máxima ideal: \r\n");
			TmaxIdeal = UART_receiveChar();
			UART_sendString("Datos recibidos, Programa se sigue ejecutando...: \r\n");
		}
	
		
		
		//OCR1B = ValorADC;
			
		
		
	
	
	_delay_ms(5000);
	
    }
}

	
