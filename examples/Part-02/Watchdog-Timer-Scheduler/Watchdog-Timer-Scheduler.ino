#ifndef F_CPU
#define F_CPU 8000000UL
#endif

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/wdt.h>
#include <stdint.h>

#define LED_PIN PB0

volatile uint8_t tick_16ms = 0;

// Watchdog interrupt (every ~16ms)
ISR(WDT_vect)
{
    tick_16ms = 1;
}

static void io_init(void)
{
    DDRB  |= (1 << LED_PIN);
    PORTB &= ~(1 << LED_PIN);
}

static void wdt_init(void)
{
    cli();

    // Clear reset flag
    MCUSR &= ~(1 << WDRF);

    // Enable change
    WDTCR |= (1 << WDCE) | (1 << WDE);

    // Interrupt mode only, ~16ms interval
    WDTCR = (1 << WDIE);

    sei();
}

int main(void)
{
    io_init();
    wdt_init();
    sei();

    uint16_t counter = 0;

    while (1)
    {
        if (tick_16ms)
        {
            tick_16ms = 0;

            counter++;

            // 16ms * 62 ≈ 1 second
            if (counter >= 62)
            {
                counter = 0;
                PORTB ^= (1 << LED_PIN);
            }
        }
    }
}
