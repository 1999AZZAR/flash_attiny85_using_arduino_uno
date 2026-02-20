/*
 * ATtiny85 Production SOS - 8MHz Internal
 * Security: Direct register access to minimize side-channel risk
 * Performance: Optimized bit-toggling via PINB for minimal latency
 */

#ifndef F_CPU
#define F_CPU 8000000UL // Tell the compiler we are at 8MHz
#endif

#include <avr/io.h>
#include <util/delay.h>

// Timing constants in milliseconds
#define DOT_DURATION 200
#define DASH_DURATION 600
#define ELEMENT_GAP 200
#define LETTER_GAP 600
#define WORD_GAP 1400

void signal_dot(void) {
    PINB = (1 << PB0); // Toggle On
    _delay_ms(DOT_DURATION);
    PINB = (1 << PB0); // Toggle Off
    _delay_ms(ELEMENT_GAP);
}

void signal_dash(void) {
    PINB = (1 << PB0); // Toggle On
    _delay_ms(DASH_DURATION);
    PINB = (1 << PB0); // Toggle Off
    _delay_ms(ELEMENT_GAP);
}

int main(void) {
    // Set PB0 (Pin 5) as Output
    DDRB |= (1 << PB0);

    // Security-First: Ensure all PORTB pins are low/tri-stated to start
    // Clears PB0 to 0 (Low) and others to 0 (Hi-Z/Input)
    PORTB = 0;

    while (1) {
        // S (...)
        signal_dot();
        signal_dot();
        signal_dot();
        _delay_ms(LETTER_GAP - ELEMENT_GAP); // Adjust for last element gap

        // O (---)
        signal_dash();
        signal_dash();
        signal_dash();
        _delay_ms(LETTER_GAP - ELEMENT_GAP);

        // S (...)
        signal_dot();
        signal_dot();
        signal_dot();

        // Gap between words
        _delay_ms(WORD_GAP - ELEMENT_GAP);
    }

    return 0;
}
