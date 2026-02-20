#include <avr/io.h>
#include <avr/sleep.h>
#include <avr/interrupt.h>

/*
 * ADC Noise Reduction & Sleep mode
 * 
 * Logic:
 * Taking ADC samples is power-intensive and noisy.
 * Using SLEEP_MODE_ADC, the CPU stops during the conversion,
 * reducing digital switching noise and saving power.
 */

volatile uint16_t adc_result;

ISR(ADC_vect) {
    // Conversion complete. Wakes up the CPU automatically.
    adc_result = ADC;
}

void setup_adc() {
    ADMUX = (1 << MUX0);         // Select ADC1 (PB2/Pin 7)
    ADCSRA = (1 << ADEN) |       // Enable ADC
             (1 << ADIE) |       // Enable ADC Interrupt
             (1 << ADPS1) | (1 << ADPS0); // Prescaler 8 (for 8MHz clock)
    sei();                       // Global interrupts
}

int main() {
    setup_adc();
    DDRB = (1 << PB0);           // PB0 as indicator LED

    while(1) {
        // Start ADC conversion and enter sleep mode immediately
        set_sleep_mode(SLEEP_MODE_ADC);
        sleep_mode();

        // --- CPU Wakes up here after conversion completes ---
        
        // Use result for simple logic:
        if (adc_result > 512) {
            PORTB |= (1 << PB0);
        } else {
            PORTB &= ~(1 << PB0);
        }
        
        for(volatile uint32_t i=0; i<10000; i++); 
    }
}
