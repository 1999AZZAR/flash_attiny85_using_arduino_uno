#include <avr/io.h>
#include <avr/sleep.h>
#include <avr/interrupt.h>

/*
 * External Wakeup via Pin Change Interrupt (PCINT)
 * 
 * Logic:
 * 1. Chip enters SLEEP_MODE_PWR_DOWN (Lowest current mode).
 * 2. CPU and all clocks stop.
 * 3. Wakes up immediately when PB3 (Pin 2) is grounded.
 * 
 * Target Current: < 1uA in sleep.
 */

ISR(PCINT0_vect) {
    // This ISR handles Pin Change Interrupts on PORTB
    // We don't need logic here, the CPU just needs to wake up.
}

void setup_sleep() {
    DDRB &= ~(1 << PB3);         // PB3 (Pin 2) as Input
    PORTB |= (1 << PB3);         // Enable Internal Pull-up
    
    GIMSK |= (1 << PCIE);        // Enable Pin Change Interrupts
    PCMSK |= (1 << PCINT3);      // Watch specifically PB3 (PCINT3)
    
    sei();                       // Global interrupts enabled
}

int main() {
    DDRB |= (1 << PB0);          // PB0 as indicator LED
    setup_sleep();

    while(1) {
        // Blink to show we are awake
        PORTB |= (1 << PB0);
        for(volatile uint32_t i=0; i<5000; i++); 
        PORTB &= ~(1 << PB0);

        // Enter deepest sleep possible
        set_sleep_mode(SLEEP_MODE_PWR_DOWN);
        sleep_enable();
        
        // Disable BOD during sleep to save ~20uA
        MCUCR |= (1 << BODS) | (1 << BODSE);
        MCUCR &= ~(1 << BODSE);
        MCUCR |= (1 << BODS);
        
        sleep_cpu();             // Sleep until PB3 is pressed
        
        // --- WAKES UP HERE ---
        sleep_disable();
    }
}
