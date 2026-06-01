/*
 * ATtiny85 Watchdog Reset & Safety
 * Demonstrates using the Watchdog Timer (WDT) as a system supervisor.
 * 
 * Hardware:
 * - LED on PB0 (Pin 5)
 * - Button on PB3 (Pin 2)
 * 
 * Logic:
 * 1. On Power-up: LED flashes slowly 3 times.
 * 2. On WDT Reset: LED flashes rapidly 10 times (error signal).
 * 3. Normal Operation: LED blinks once per second.
 * 4. Hang Simulation: Hold button for >2 seconds to enter an infinite loop.
 * 5. WDT Action: After ~2 seconds of hanging, the WDT will reset the chip.
 */

#ifndef F_CPU
#define F_CPU 8000000UL
#endif

#include <avr/io.h>
#include <avr/wdt.h>
#include <util/delay.h>
#include <stdint.h>

#define LED_PIN PB0
#define BTN_PIN PB3

/* ================= UTILS ================= */

static void flash_led(uint8_t count, uint16_t speed_ms) {
    for (uint8_t i = 0; i < count; i++) {
        PORTB |= (1 << LED_PIN);
        _delay_ms(speed_ms);
        PORTB &= ~(1 << LED_PIN);
        _delay_ms(speed_ms);
    }
}

/* ================= MAIN ================= */

int main(void) {
    // 1. Check Reset Cause
    uint8_t reset_source = MCUSR;
    MCUSR = 0; // Clear flags for next time

    // Disable WDT immediately to prevent reset loop if WDT was enabled
    wdt_disable();

    // Setup IO
    DDRB |= (1 << LED_PIN);
    DDRB &= ~(1 << BTN_PIN);
    PORTB |= (1 << BTN_PIN); // Pull-up

    // 2. Visual Indication of Reset Type
    if (reset_source & (1 << WDRF)) {
        // Watchdog Reset occurred!
        flash_led(10, 50); // Rapid flash
    } else {
        // Normal Power-on or External Reset
        flash_led(3, 300); // Slow flash
    }

    // 3. Enable Watchdog (2 second timeout)
    wdt_enable(WDTO_2S);

    while (1) {
        // 4. "Kick the dog" (Reset Timer)
        // If this isn't called within 2 seconds, the chip resets.
        wdt_reset();

        // 5. Normal Operation (Blink)
        PORTB ^= (1 << LED_PIN);
        _delay_ms(500);

        // 6. Simulate System Hang
        // If button is pressed, we enter a "dead" loop.
        if (!(PINB & (1 << BTN_PIN))) {
            // Stay here until WDT rescues us
            while (1) {
                // We ARE NOT calling wdt_reset() here!
                // We can flash the LED to show we are stuck
                PORTB |= (1 << LED_PIN);
            }
        }
    }
}
