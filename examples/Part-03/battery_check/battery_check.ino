#include <avr/io.h>
#include <avr/sleep.h>

/*
 * Precision Battery/VCC Check via Internal Bandgap
 * 
 * Logic:
 * Instead of external resistor divider (which leaks current!),
 * we use the 1.1V internal reference as the ADC Input and
 * VCC as the ADC Reference.
 * 
 * VCC = (1.1V * 1024) / ADC_Reading
 */

uint16_t get_vcc_mv() {
    // 1. Select ADC Input to Internal 1.1V Bandgap
    // 2. Select VCC as Reference (default)
    ADMUX = (1 << MUX3) | (1 << MUX2); 

    // 3. Enable ADC and wait for reference to stabilize
    ADCSRA = (1 << ADEN) | (1 << ADPS1) | (1 << ADPS0); // Prescaler 8
    for(volatile uint16_t i=0; i<1000; i++); 

    // 4. Start conversion
    ADCSRA |= (1 << ADSC);
    while (ADCSRA & (1 << ADSC)); // Wait for it to finish

    uint16_t result = ADC;
    
    // 5. Calculate Millivolts: 1.1 * 1024 * 1000 = 1126400
    // VCC = 1126400 / result
    uint32_t vcc = 1126400L / (uint32_t)result;
    
    // 6. Disable ADC to save power
    ADCSRA &= ~(1 << ADEN);
    
    return (uint16_t)vcc;
}

int main() {
    DDRB |= (1 << PB0); // PB0 as "Low Battery" Indicator

    while(1) {
        uint16_t vcc = get_vcc_mv();

        if (vcc < 3000) { // If VCC drops below 3.0V
            PORTB |= (1 << PB0); // Warning Light
        } else {
            PORTB &= ~(1 << PB0);
        }

        // Deep Sleep for 8 seconds via Watchdog would go here
        // (For simplicity, we just loop with long wait)
        for(volatile uint32_t i=0; i<50000; i++); 
    }
}
