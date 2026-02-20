#ifndef F_CPU
#define F_CPU 8000000UL
#endif

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <stdint.h>

#define LED_PIN PB0
#define BTN_PIN PB3

volatile uint8_t wake_flag = 0;

// Pin Change Interrupt
ISR(PCINT0_vect)
{
    wake_flag = 1;
}

static void io_init(void)
{
    // LED output
    DDRB  |= (1 << LED_PIN);
    PORTB &= ~(1 << LED_PIN);

    // Button input with pull-up
    DDRB  &= ~(1 << BTN_PIN);
    PORTB |= (1 << BTN_PIN);
}

static void pcint_init(void)
{
    // Enable pin change interrupt for PB3
    GIMSK |= (1 << PCIE);        // Enable PCINT
    PCMSK |= (1 << PCINT3);      // Enable interrupt on PB3
}

static void enter_sleep(void)
{
    set_sleep_mode(SLEEP_MODE_PWR_DOWN);
    sleep_enable();

    sei();
    sleep_cpu();

    sleep_disable();
}

int main(void)
{
    io_init();
    pcint_init();
    sei();

    while (1)
    {
        // Toggle LED when woken
        if (wake_flag)
        {
            wake_flag = 0;
            PORTB ^= (1 << LED_PIN);
        }

        // Enter deepest sleep
        enter_sleep();
    }
}
