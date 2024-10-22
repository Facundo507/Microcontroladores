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

uint16_t Temp0 = 250;
uint16_t Temp22 = 300;
uint16_t Temp30 = 400;
uint16_t Temp40 = 500;
uint16_t Temp50 = 600;

void UART_init(unsigned int ubrr) {
    UBRR0H = (unsigned char)(ubrr >> 8);
    UBRR0L = (unsigned char)ubrr;
    UCSR0B = (1 << RXEN0) | (1 << TXEN0);
    UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);
}

void UART_sendChar(char data) {
    while (!(UCSR0A & (1 << UDRE0))) {}
    UDR0 = data;
}

char UART_receiveChar(void) {
    while (!(UCSR0A & (1 << RXC0))) {}
    return UDR0;
}

void UART_sendString(const char *str) {
    while (*str) {
        UART_sendChar(*str++);
    }
}

void UART_send16bitsComoString(uint16_t Entero1024) {
    char Cadena[6];
    uint8_t i = 0;
    if (Entero1024 == 0) {
        UART_sendChar('0');
        return;
    }
    while (Entero1024 > 0) {
        Cadena[i++] = (Entero1024 % 10) + '0';
        Entero1024 /= 10;
    }
    for (int8_t j = i - 1; j >= 0; j--) {
        UART_sendChar(Cadena[j]);
    }
}

void CompararVoltaje(float Voltaje) {
    uint16_t entero = (uint16_t)Voltaje;
    uint16_t decimal = (uint16_t)((Voltaje - entero) * 100);
    UART_send16bitsComoString(entero);
    UART_sendChar('.');
    if (decimal < 10) {
        UART_sendChar('0');
    }
    UART_send16bitsComoString(decimal);
    UART_sendChar('V');
}

void SetupPWM(void) {
    DDRB = 0xFF;
    TCCR1A = (1 << COM1A1) | (1 << COM1B1) | (1 << WGM11);
    TCCR1B = (1 << WGM13) | (1 << WGM12) | (0 << CS12) | (1 << CS11) | (1 << CS10);
    ICR1 = 1024;
    OCR1A = 72;
    OCR1B = 0;
}

void SetupADC(void) {
    ADMUX |= (1 << REFS0);
    ADMUX &= ~(1 << ADLAR);
    ADCSRA |= (1 << ADPS2) | (1 << ADPS1);
    ADCSRA |= (1 << ADEN);
}

uint16_t LeerADC(uint8_t canal) {
    ADMUX = (ADMUX & 0xF0) | (canal & 0x0F);
    ADCSRA |= (1 << ADSC);
    while (ADCSRA & (1 << ADSC));
    return ADC;
}

void ajustarPuntoMedio(uint16_t puntoMedio) {
    Temp22 = puntoMedio;
    Temp30 = puntoMedio + 100;
    Temp40 = puntoMedio + 200;
    Temp50 = puntoMedio + 300;
}

int main(void) {
    SetupPWM();
    SetupADC();
    int8_t Segundo = 0;
    int8_t Minuto = 0;

    UART_init(103);
    UART_sendString("Bienvenido al programa, para cambiar los rangos de temperatura ideal presione la tecla A");
    UART_sendString("\r\n");
    UART_sendString("____ Tiempo ____ ValorADC ___ Voltaje ___ EstadoCalentador ______ EstadoVentilador\r\n ");
    UART_sendString("----------------------------");
    UART_sendString("\r\n");

    while (1) {
        if (UART_receiveChar() == 'A') {
            UART_sendString("\r\nIngrese el nuevo punto medio (en el rango de 0 a 1023): ");
            uint16_t nuevoPuntoMedio = 0;
            char c;
            while ((c = UART_receiveChar()) != '\r') {
                if (c >= '0' && c <= '9') {
                    nuevoPuntoMedio = nuevoPuntoMedio * 10 + (c - '0');
                    UART_sendChar(c);
                }
            }
            ajustarPuntoMedio(nuevoPuntoMedio);
            UART_sendString("\r\nPunto medio ajustado exitosamente.\r\n");
        }

        uint16_t ValorADC = LeerADC(1);
        float Voltaje = ValorADC * (5.0 / 1023.0);
        _delay_ms(2000);
        Segundo = Segundo + 5;
        if (Segundo >= 60) {
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

        if ((ValorADC >= Temp0) && (ValorADC < Temp22)) {
            UART_sendString("  |  ON  |   OFF         | ");
            PORTB |= (1 << PORTB5); // Encender calefactor
            OCR1A = 0; // Apagar ventilador
        } else if ((ValorADC >= Temp22) && (ValorADC < Temp30)) {
            UART_sendString("  |  OFF  |   OFF        | ");
            PORTB &= ~(1 << PORTB5); // Apagar calefactor
            OCR1A = 0; // Apagar ventilador
        } else if ((ValorADC >= Temp30) && (ValorADC < Temp40)) {
            UART_sendString("  |  OFF  |   Baja velocidad         | ");
            PORTB &= ~(1 << PORTB5); // Apagar calefactor
            OCR1A = 300; // Ventilador baja velocidad
        } else if ((ValorADC >= Temp40) && (ValorADC < Temp50)) {
            UART_sendString("  |  OFF  |   Media velocidad        | ");
            PORTB &= ~(1 << PORTB5); // Apagar calefactor
            OCR1A = 700; // Ventilador media velocidad
        } else if (ValorADC >= Temp50) {
            UART_sendString("  |  OFF  |   Max velocidad          | ");
            PORTB &= ~(1 << PORTB5); // Apagar calefactor
            OCR1A = 1023; // Ventilador alta velocidad
        }

        UART_sendString("\r\n");
    }
}
