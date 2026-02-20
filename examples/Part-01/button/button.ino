/*
 * ATtiny85 Production Button Toggle - 8MHz Internal
 * Security: Uses input pull-up resistors to prevent floating pins
 * Performance: Direct PIN reading with software debounce
 */

#ifndef F_CPU
#define F_CPU 8000000UL
#endif

#include <avr/io.h>
#include <util/delay.h>

// Define Input Pin (PB3 / Pin 2)
#define BUTTON_PIN PB3

int main(void) {
    // Set PB0 (LED) as Output
    DDRB |= (1 << PB0);

    // Set PB3 (Button) as Input
    DDRB &= ~(1 << BUTTON_PIN);

    // Enable Internal Pull-up Resistor on PB3
    // This eliminates the need for an external resistor.
    // Button should connect PB3 to GND.
    PORTB |= (1 << BUTTON_PIN);

    // Initial state: LED Off
    PORTB &= ~(1 << PB0);

    uint8_t button_state = 0;
    uint8_t last_button_state = 0;

    while (1) {
        // Read input (Active Low due to pull-up)
        // If bit is 0, button is pressed (GND)
        if (!(PINB & (1 << BUTTON_PIN))) {
            button_state = 1; // Pressed
        } else {
            button_state = 0; // Released
        }

        // Detect Edge (Press event)
        if (button_state == 1 && last_button_state == 0) {
            // Software Debounce
            _delay_ms(50);
            
            // Re-check if still pressed
            if (!(PINB & (1 << BUTTON_PIN))) {
                // Toggle LED
                PINB = (1 << PB0); // Atomic toggle
            }
        }

        last_button_state = button_state;
        
        // Small delay to prevent rapid bouncing issues
        _delay_ms(10);
    }

    return 0;
}
