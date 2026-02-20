/*
 * ATtiny85 Precision Pulse Generator (Heartbeat - FIXED)
 * Part 2.13 of the Bare Metal ATtiny85 Series
 * 
 * Logic:
 * - Timer1 in CTC (Clear Timer on Compare Match) mode.
 * - Clock: 8MHz, Prescaler: /16384 -> 488.28 Hz ticks.
 * - OCR1C = 243 (244 counts) -> 488.28 / 244 = 2.001 Hz interrupt.
 * - Toggling every 2.001 Hz interrupt results in a precise 1.000 Hz pulse.
 * 
 * Target: ATtiny85 @ 8MHz Internal Oscillator
 */

#ifndef F_CPU
#define F_CPU 8000000UL
#endif

#include <avr/io.h>
#include <avr/interrupt.h>

// ======================== CONFIGURATION ========================

#define OUTPUT_PIN      PB0   // Pin 5 (LED)

// ======================== INTERRUPTS ========================

ISR(TIMER1_COMPA_vect) {
    // This triggers every ~500ms
    PINB = (1 << OUTPUT_PIN); // Atomic Hardware Toggle
}

// ======================== INITIALIZATION ========================

static void timer1_init(void) {
    // 1. Set PB0 as Output
    DDRB |= (1 << OUTPUT_PIN);
    PORTB &= ~(1 << OUTPUT_PIN);

    // 2. Configure Timer1 for CTC Mode
    // TCCR1: CTC1=1 (Clear on Match with OCR1C)
    // CS13..CS10=1111 (Prescaler /16384)
    TCCR1 = (1 << CTC1) | (1 << CS13) | (1 << CS12) | (1 << CS11) | (1 << CS10);
    
    // 3. Set Compare Match Values
    // OCR1C defines the TOP (reset point) for CTC mode.
    // OCR1A defines when the OCIE1A interrupt triggers.
    OCR1C = 243; // 244 ticks @ 488.28Hz = 0.4997s
    OCR1A = 243; // Must match OCR1C to trigger at the top

    // 4. Enable Compare Match Interrupt
    TIMSK |= (1 << OCIE1A);
}

// ======================== MAIN ========================

int main(void) {
    timer1_init();
    sei(); // Enable global interrupts

    while (1) {
        // CPU is free. The heartbeat is 100% interrupt-driven.
        __asm__ __volatile__ ("nop"); 
    }

    return 0;
}
