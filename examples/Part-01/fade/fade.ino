/*
 * ATtiny85 Production Fade - 1MHz Internal
 * Security: Uses hardware peripherals to reduce CPU load and potential blocking exploits
 * Performance: Hardware PWM (Timer0) allows CPU-independent signal generation
 */

#ifndef F_CPU
#define F_CPU 1000000UL
#endif

#include <avr/io.h>
#include <util/delay.h>

int main(void) {
    // Set PB0 (OC0A - Pin 5) as Output
    DDRB |= (1 << PB0);

    // Security-First: Initialize PORTB to known state
    PORTB = 0;

    /*
     * Configure Timer0 for Fast PWM mode
     * WGM00 | WGM01: Fast PWM mode
     * COM0A1: Clear OC0A on Compare Match, set at Bottom (Non-inverting mode)
     */
    TCCR0A = (1 << COM0A1) | (1 << WGM00) | (1 << WGM01);

    /*
     * Start Timer0 with Prescaler /1
     * CS00: Clock Select /1
     * Frequency = 1MHz / (256 * 1) ~= 3.9kHz (flicker-free)
     */
    TCCR0B = (1 << CS00);

    uint8_t brightness = 0;
    int8_t step = 1;

    while (1) {
        // Update duty cycle register
        OCR0A = brightness;

        // Calculate next brightness level
        brightness += step;

        // Reverse direction at limits
        if (brightness == 0 || brightness == 255) {
            step = -step;
        }

        // Delay for smooth visual transition (adjust for speed)
        _delay_ms(5);
    }

    return 0;
}
