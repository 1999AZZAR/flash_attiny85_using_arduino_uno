#include <avr/io.h>
#include <avr/power.h>
#include <avr/sleep.h>

/*
 * PRR Mastery (Power Reduction Register)
 * 
 * Logic:
 * Total shutdown of individual hardware modules (Timer0, Timer1, USI, ADC).
 * This example is purely architectural to show how to kill all internal peripherals.
 */

int main() {
    // 1. Initial State: All Outputs Low
    DDRB = 0xFF;
    PORTB = 0x00;

    // 2. Kill all internal hardware modules to zero out their current draw
    // PRTIM0: Timer/Counter0
    // PRTIM1: Timer/Counter1
    // PRUSI:  Universal Serial Interface (SPI/I2C)
    // PRADC:  Analog to Digital Converter
    PRR |= (1 << PRTIM1) | (1 << PRTIM0) | (1 << PRUSI) | (1 << PRADC);

    // 3. Disable AC (Analog Comparator)
    ACSR |= (1 << ACD);

    while(1) {
        // Simple heartbeat flash before total sleep
        PORTB ^= (1 << PB0);
        for(volatile uint32_t i=0; i<10000; i++); 

        // Enter Deep Sleep with everything off
        set_sleep_mode(SLEEP_MODE_PWR_DOWN);
        sleep_mode();
    }
}
