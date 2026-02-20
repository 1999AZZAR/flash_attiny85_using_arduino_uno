/*
 * ATtiny85 Precision Pulse Generator (Heartbeat)
 * Part 2.13 of the Bare Metal ATtiny85 Series
 * 
 * Logic:
 * - Timer1 in CTC (Clear Timer on Compare Match) mode.
 * - Generates a highly precise 1Hz signal (heartbeat).
 * - Uses Timer1 Interrupt to toggle PB1 (OC1A) for zero-jitter timing.
 * 
 * Target: ATtiny85 @ 8MHz Internal Oscillator
 */

#ifndef F_CPU
#define F_CPU 8000000UL
#endif

#include <avr/io.h>
#include <avr/interrupt.h>

// ======================== CONFIGURATION ========================

#define OUTPUT_PIN      PB1   // Pin 6 (OC1A)
#define HEARTBEAT_HZ    1     // 1 Pulse per second (1Hz)

// For 8MHz clock:
// 8,000,000 / 8192 (prescaler) = 976.5625 ticks per second.
// We want 1Hz toggle, so we need 976.5625 / 2 = 488.28 ticks per half-cycle.
// Since Timer1 is 8-bit (max 255), we use a software divider inside the ISR.

volatile uint16_t g_overflow_count = 0;
#define TARGET_OVERFLOWS 4 // (976 / 250) approx 4 interrupts per half-cycle

// ======================== INTERRUPTS ========================

ISR(TIMER1_COMPA_vect) {
    static uint16_t counter = 0;
    counter++;
    
    // 8MHz / 16384 prescaler = 488.28 Hz
    // 488 counts = 1 second total.
    // 244 counts = 500ms (Toggle point for 1Hz)
    if (counter >= 244) {
        PINB = (1 << OUTPUT_PIN); // Atomic Toggle
        counter = 0;
    }
}

// ======================== INITIALIZATION ========================

static void timer1_init(void) {
    // 1. Set PB1 as Output
    DDRB |= (1 << OUTPUT_PIN);
    PORTB &= ~(1 << OUTPUT_PIN);

    // 2. Configure Timer1 for CTC Mode
    // TCCR1: CTC1=1 (Clear on Match), CS13..CS10=1111 (Prescaler /16384)
    TCCR1 = (1 << CTC1) | (1 << CS13) | (1 << CS12) | (1 << CS11) | (1 << CS10);
    
    // 3. Set Compare Match Value
    // OCR1C is the resolution for Timer1 in CTC mode.
    // At /16384 prescaler, 1 tick = 2.048ms.
    // We don't need to change OCR1C frequently, we use it as a fixed base tick.
    OCR1C = 255; 

    // 4. Enable Compare Match Interrupt
    TIMSK |= (1 << OCIE1A);
}

// ======================== MAIN ========================

int main(void) {
    timer1_init();
    sei(); // Enable global interrupts

    while (1) {
        // CPU can do other tasks or sleep here.
        // The heartbeat is handled entirely by hardware interrupts.
        __asm__ __volatile__ ("nop"); 
    }

    return 0;
}
