#define F_CPU 16000000UL // Frecuencia del microcontrolador (16 MHz)
#include <avr/io.h>
#include <util/delay.h>
#include <string.h>

#define NUM_BUTTONS 8
#define BUZZER_PIN PB2
#define REST 0

// Notas musicales
#define NOTE_D4  294
#define NOTE_G4  392
#define NOTE_AS4 466
#define NOTE_A4  440
#define NOTE_D5  587
#define NOTE_C5  523
#define NOTE_E5  659
#define NOTE_B4  494
#define NOTE_C4  262
#define NOTE_F4  349
#define NOTE_E4  330
#define NOTE_G5  784
#define NOTE_A5  880

// Melodía de Harry Potter
uint16_t melody_hp[] = {
    REST, 2, NOTE_D4, 4,
    NOTE_G4, -4, NOTE_AS4, 8, NOTE_A4, 4,
    NOTE_G4, 2, NOTE_D5, 4,
    NOTE_C5, -2, 
    NOTE_A4, -2
};
uint8_t notes_hp = sizeof(melody_hp) / sizeof(melody_hp[0]) / 2;

// Melodía de Tetris
uint16_t melody_tetris[] = {
    NOTE_E5, 4,  NOTE_B4, 8,  NOTE_C5, 8,  NOTE_D5, 4,  NOTE_C5, 8,  NOTE_B4, 8,
    NOTE_A4, 4,  NOTE_A4, 8,  NOTE_C5, 8,  NOTE_E5, 4,  NOTE_D5, 8,  NOTE_C5, 8
};
uint8_t notes_tetris = sizeof(melody_tetris) / sizeof(melody_tetris[0]) / 2;

// Notas asociadas a los botones
uint16_t buttonNotes[] = {NOTE_C4, NOTE_D4, NOTE_E4, NOTE_F4, NOTE_G4, NOTE_A4, NOTE_B4, NOTE_C5};

void UART_init(unsigned int ubrr) {
    UBRR0H = (unsigned char)(ubrr >> 8);
    UBRR0L = (unsigned char)ubrr;
    UCSR0B = (1 << RXEN0) | (1 << TXEN0); // Habilitar transmision y recepcion UART
    UCSR0C = (1 << UCSZ01) | (1 << UCSZ00); // Configuracion: 8 bits de datos, 1 bit de parada
}

void UART_sendChar(char data) {
    while (!(UCSR0A & (1 << UDRE0))) {}
    UDR0 = data;
}

void UART_sendString(const char *str) {
    while (*str) {
        UART_sendChar(*str++);
    }
}

char UART_receiveChar(void) {
    while (!(UCSR0A & (1 << RXC0))) {}
    return UDR0;
}

void init_pulsadores() {
    DDRD &= ~((1 << NUM_BUTTONS) - 1) << 2; // Configurar PD2-PD7 como entradas (botones)
    DDRB &= ~(1 << 0); // Configurar PB0 como entrada (boton)
    DDRB &= ~(1 << 1); // Configurar PB1 como entrada (boton)
    PORTD |= ((1 << NUM_BUTTONS) - 1) << 2; // Habilitar pull-up interno para PD2-PD7
    PORTB |= (1 << 0); // Habilitar pull-up interno para PB0
    PORTB |= (1 << 1); // Habilitar pull-up interno para PB1
}

void init_buzzer() {
    DDRB |= (1 << BUZZER_PIN); // Configurar PB2 como salida (buzzer)
}

void tocar_nota(uint16_t frecuencia, uint16_t duracion) {
    if (frecuencia == REST) {
        OCR1B = 0; // Apagar el buzzer
    } else {
        OCR1B = (F_CPU / (2 * 8 * frecuencia)) - 1;
    }

    TCCR1A = (1 << COM1B1) | (1 << WGM10); // Clear OC1B on Compare Match, modo PWM de 8 bits
    TCCR1B = (1 << WGM12) | (1 << CS11);   // Modo PWM de 8 bits, prescaler de 8

    for (uint16_t i = 0; i < duracion; i++) {
        _delay_ms(1);
    }
    OCR1B = 0; // Apagar el buzzer
}

void reproducir_cancion(uint16_t *melodia, uint8_t notas) {
    for (uint8_t thisNote = 0; thisNote < notas * 2; thisNote += 2) {
        int divider = melodia[thisNote + 1];
        int noteDuration = (divider > 0) ? (60000 * 4) / (144 * divider) : ((60000 * 4) / (144 * abs(divider))) * 1.5;
        tocar_nota(melodia[thisNote], noteDuration);
        _delay_ms(50); // Pausa entre notas
    }
}

void playWithButtons() {
    UART_sendString("Usando botones para reproducir notas individuales\r\n");
    while (1) {
        for (uint8_t i = 0; i < NUM_BUTTONS; i++) {
            uint8_t boton_actual = (i < 6) ? (PIND & (1 << (i + 2))) : (PINB & (1 << (i - 6)));
            if (boton_actual == 0) {
                tocar_nota(buttonNotes[i], 500);
                UART_sendString("Boton presionado: ");
                UART_sendChar('1' + i);
                UART_sendString("\r\n");
                _delay_ms(500);
                break;
            }
        }
        if (UCSR0A & (1 << RXC0)) {
            char exit_command = UART_receiveChar();
            if (exit_command == 'H' || exit_command == 'T') {
                break;
            }
        }
    }
}

void setup(void) {
    UART_init(103); // Inicializar UART a 9600 baudios
    init_pulsadores(); // Inicializar los botones
    init_buzzer(); // Inicializar el buzzer
    UART_sendString("Enviar 'H' para Harry Potter, 'T' para Tetris, o 'B' para usar botones\r\n");
}

void loop(void) {
    if (UCSR0A & (1 << RXC0)) {
        char comando = UART_receiveChar();
        if (comando == 'H') {
            UART_sendString("Reproduciendo Harry Potter Theme\r\n");
            reproducir_cancion(melody_hp, notes_hp);
        } else if (comando == 'T') {
            UART_sendString("Reproduciendo Tetris Theme\r\n");
            reproducir_cancion(melody_tetris, notes_tetris);
        } else if (comando == 'B') {
            playWithButtons();
        } else {
            UART_sendString("Opcion invalida. Enviar 'H', 'T', o 'B'\r\n");
        }
    }

    _delay_ms(50); 
}
