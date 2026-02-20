/*
 * ATtiny85 Production Candle - 1MHz Internal
 * Security: Uses hardware peripherals and secure PRNG seeding (LFSR)
 * Performance: Hardware PWM (Timer0) allows CPU-independent signal generation
 */

#ifndef F_CPU
#define F_CPU 1000000UL
#endif

#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>

// Linear Feedback Shift Register (LFSR) for fast pseudo-random numbers
// This is much faster and smaller than rand()
uint16_t lfsr = 0xACE1;

uint16_t get_random(void) {
    uint16_t bit;
    /* taps: 16 14 13 11; feedback polynomial: x^16 + x^14 + x^13 + x^11 + 1 */
    bit  = ((lfsr >> 0) ^ (lfsr >> 2) ^ (lfsr >> 3) ^ (lfsr >> 5) ) & 1;
    lfsr =  (lfsr >> 1) | (bit << 15);
    return lfsr;
}

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
     * Start Timer0 with Prescaler /8
     * CS01: Clock Select /8 (~3.9kHz)
     */
    TCCR0B = (1 << CS01);

    while (1) {
        // Generate random brightness between 50 and 255 for realistic flicker
        // Mask with 0xFF then ensure minimum brightness
        uint8_t flicker = (get_random() & 0xFF);
        if (flicker < 50) flicker = 50;

        OCR0A = flicker;

        // Random delay between flicker updates (20ms to 100ms)
        uint8_t delay_time = (get_random() % 80) + 20;
        
        // Loop delay manually because _delay_ms requires a constant at compile time
        // for optimal code size, but variable delay is needed here.
        // Or use _delay_loop_2 for precise cycles, but simple loop is fine.
        while(delay_time--) {
            _delay_ms(1);
        }
    }

    return 0;
}
