#include <avr/io.h>
#include <util/delay.h>

// Función para inicializar los pines
void init_pins() {
    // Configurar los pines como salida en el puerto B (asignación de pines)
    DDRB |= (1 << PB2) | (1 << PB3) | (1 << PB4) | (1 << PB5) | (1 << PB6) | (1 << PB7);

    // Configurar los pines para entrada en el puerto D si es necesario (ejemplo para botones)
    DDRD &= ~(1 << PD2) & ~(1 << PD3);  // Configura PD2 y PD3 como entrada
    PORTD |= (1 << PD2) | (1 << PD3);   // Activa pull-up en PD2 y PD3
}

// Función para mover el solenoide
void mover_solenoide(int bajar) {
    if (bajar) {
        PORTB |= (1 << PB2);   // Activar solenoide para bajar
        PORTB &= ~(1 << PB3);  // Desactivar solenoide para subir
    } else {
        PORTB &= ~(1 << PB2);  // Desactivar solenoide para bajar
        PORTB |= (1 << PB3);   // Activar solenoide para subir
    }
}

// Función para mover el plóter en las direcciones X e Y
void mover_plotter(int dir) {
    switch (dir) {
        case 0:  // Mover hacia abajo
            PORTB |= (1 << PB4);
            _delay_ms(1000);
            PORTB &= ~(1 << PB4);
            break;
        case 1:  // Mover hacia arriba
            PORTB |= (1 << PB5);
            _delay_ms(1000);
            PORTB &= ~(1 << PB5);
            break;
        case 2:  // Mover hacia la izquierda
            PORTB |= (1 << PB6);
            _delay_ms(500);
            PORTB &= ~(1 << PB6);
            break;
        case 3:  // Mover hacia la derecha
            PORTB |= (1 << PB7);
            _delay_ms(1000);
            PORTB &= ~(1 << PB7);
            break;
        case 4:  // Mover hacia abajo-izquierda (diagonal)
            PORTB |= (1 << PB4) | (1 << PB6);
            _delay_ms(1000);
            PORTB &= ~(1 << PB4) & ~(1 << PB6);
            break;
        case 5:  // Mover hacia abajo-derecha (diagonal)
            PORTB |= (1 << PB4) | (1 << PB7);
            _delay_ms(1000);
            PORTB &= ~(1 << PB4) & ~(1 << PB7);
            break;
        case 6:  // Mover hacia arriba-izquierda (diagonal)
            PORTB |= (1 << PB5) | (1 << PB6);
            _delay_ms(1000);
            PORTB &= ~(1 << PB5) & ~(1 << PB6);
            break;
        case 7:  // Mover hacia arriba-derecha (diagonal)
            PORTB |= (1 << PB5) | (1 << PB7);
            _delay_ms(1000);
            PORTB &= ~(1 << PB5) & ~(1 << PB7);
            break;
    }
}

int main() {
    // Inicializar los pines
    init_pins();

    // Ejemplo: mover solenoide hacia abajo y luego hacia la derecha
    mover_solenoide(1);  // Bajar solenoide
    // Secuencia de movimientos del plóter
    mover_plotter(1);
    mover_plotter(2);
    mover_plotter(7);
    mover_plotter(6);
    mover_plotter(3);
    mover_plotter(1);
    mover_plotter(5);
    mover_plotter(7);
    mover_plotter(0);
    mover_plotter(3);
    mover_plotter(4);
    mover_plotter(5);
    mover_plotter(2);
    mover_plotter(0);
    mover_plotter(6);
    mover_plotter(4);
    //Hojas       
    mover_plotter(2);
    mover_plotter(5);
    mover_plotter(3);
    mover_plotter(3);
    mover_plotter(7);
    mover_plotter(2);
    mover_plotter(4);
    mover_plotter(6);
    //Tallo  
    mover_plotter(7);
    mover_plotter(0);
    mover_plotter(0);
    //Centro  
    mover_solenoide(0);  // Subir solenoide
    mover_plotter(1);
    mover_plotter(1);
    mover_plotter(1);
    mover_plotter(1);
    mover_plotter(1);
    mover_solenoide(1);  // Bajar solenoide
    mover_plotter(2);
    mover_plotter(1);
    mover_plotter(3);
    mover_plotter(0);
    mover_plotter(2);
    mover_solenoide(0);  // Subir solenoide

    return 0;
}
