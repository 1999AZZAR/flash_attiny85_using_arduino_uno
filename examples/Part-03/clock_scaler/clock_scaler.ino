#include <avr/io.h>
#include <avr/sleep.h>
#include <avr/interrupt.h>

/*
 * ATtiny85 Dynamic Clock Scaling & Power Down
 * 
 * Logic:
 * 1. Start at 8MHz.
 * 2. Downscale to 125kHz (Prescaler 64) for background tasks.
 * 3. Enter Power-Down mode (Deep Sleep).
 * 4. Wake up via interrupt, restore 8MHz for heavy lifting.
 */

void setup_pins() {
    // Set all pins to output low to prevent leakage from floating pins
    DDRB = 0xFF;
    PORTB = 0x00;
}

void enter_deep_sleep() {
    ADCSRA &= ~(1 << ADEN);     // Disable ADC
    ACSR |= (1 << ACD);         // Disable Analog Comparator
    
    set_sleep_mode(SLEEP_MODE_PWR_DOWN);
    sleep_enable();
    
    // Disable Brown-out Detection during sleep (saves ~20uA)
    // Note: This requires timed sequence
    MCUCR |= (1 << BODS) | (1 << BODSE);
    MCUCR &= ~(1 << BODSE);
    MCUCR |= (1 << BODS);
    
    sleep_cpu(); // Nighty night!
    
    // --- WAKES UP HERE ---
    sleep_disable();
}

void set_clock_speed(uint8_t prescaler) {
    // prescaler: 0=8MHz, 6=125kHz (2^6=64)
    CLKPR = (1 << CLKPCE); // Enable clock change
    CLKPR = prescaler;     // Set new prescaler
}

int main() {
    setup_pins();
    
    while(1) {
        // High Speed Mode (8MHz)
        set_clock_speed(0);
        PORTB |= (1 << PB0);  // Quick Flash
        for(volatile uint32_t i=0; i<10000; i++); 
        PORTB &= ~(1 << PB0);

        // Slow Processing Mode (125kHz) - Prescaler 64
        set_clock_speed(6); 
        for(volatile uint16_t i=0; i<1000; i++); // Wait at slow speed

        // Extreme Sleep
        enter_deep_sleep();
    }
}
