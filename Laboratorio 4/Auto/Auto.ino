#define F_CPU 16000000UL // Frecuencia del CPU

#include <avr/io.h>
#include <util/delay.h>

// Configuración de UART
#define BAUD 38400
#define MY_UBRR F_CPU/16/BAUD-1

// Pines para control del Puente H
#define IN1 PD3
#define IN2 PD5
#define IN3 PD6
#define IN4 PB1

void USART_Init() {
    // Configurar baud rate
    UBRR0H = (MY_UBRR >> 8);
    UBRR0L = MY_UBRR;

    // Habilitar transmisión y recepción
    UCSR0B = (1 << RXEN0) | (1 << TXEN0);

    // Configurar formato: 8 datos, 1 stop bit
    UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);
}

void USART_Transmit(unsigned char data) {
    while (!(UCSR0A & (1 << UDRE0))); // Esperar que el registro esté listo
    UDR0 = data;                     // Enviar dato
}

void USART_TransmitString(const char *str) {
    while (*str) {
        USART_Transmit(*str++); // Enviar cada carácter
    }
}

unsigned char USART_Receive(void) {
    while (!(UCSR0A & (1 << RXC0))); // Esperar dato recibido
    return UDR0;
}

void configurarMotores() {
    // Configurar pines del puente H como salida
    DDRD |= (1 << IN1) | (1 << IN2) | (1 << IN3);
    DDRB |= (1 << IN4);
}

void avanzar() {
    PORTD |= (1 << IN1);  // Motor A adelante
    PORTD &= ~(1 << IN2);
    PORTD |= (1 << IN3);  // Motor B adelante
    PORTB &= ~(1 << IN4);
    USART_TransmitString("Avanzando\r\n");
}

void retroceder() {
    PORTD &= ~(1 << IN1); // Motor A atrás
    PORTD |= (1 << IN2);  // Motor A atrás
    PORTD &= ~(1 << IN3); // Motor B atrás
    PORTB |= (1 << IN4);  // Motor B atrás
    USART_TransmitString("Retrocediendo\r\n");
}

void girarIzquierda() {
    PORTD &= ~(1 << IN1); // Motor A atrás
    PORTD |= (1 << IN2);  // Motor A atrás
    PORTD |= (1 << IN3);  // Motor B adelante
    PORTB &= ~(1 << IN4);
    USART_TransmitString("Girando a la izquierda\r\n");
}

void girarDerecha() {
    PORTD |= (1 << IN1);  // Motor A adelante
    PORTD &= ~(1 << IN2);
    PORTD &= ~(1 << IN3); // Motor B atrás
    PORTB |= (1 << IN4);  // Motor B atrás
    USART_TransmitString("Girando a la derecha\r\n");
}

void detenerMotores() {
    PORTD &= ~(1 << IN1) & ~(1 << IN2) & ~(1 << IN3);
    PORTB &= ~(1 << IN4);
}

int main() {
    USART_Init();       // Inicializar UART
    configurarMotores(); // Configurar motores

    while (1) {
        if (UCSR0A & (1 << RXC0)) { // Verificar si hay datos recibidos
            unsigned char comando = USART_Receive();
            switch (comando) {
                case 'W': // Avanzar
                    avanzar();
                    _delay_ms(1000);
                    break;
                case 'S': // Retroceder
                    retroceder();
                    _delay_ms(1000);
                    break;
                case 'D': // Girar izquierda
                    girarIzquierda();
                    _delay_ms(200);
                    break;
                case 'A': // Girar derecha
                    girarDerecha();
                    _delay_ms(200);
                    break;
                default:  // Comando desconocido
                    detenerMotores();
                    break;
            }
        }
    }

    return 0;
}
