#ifndef F_CPU
#define F_CPU 8000000UL
#endif

#include <avr/io.h>
#include <util/delay.h>
#include <stdint.h>

#define TOUCH_PIN PB2
#define LED_PIN   PB0

#define THRESHOLD 40

static uint16_t measure_touch(void)
{
    uint16_t count = 0;

    // Discharge pin
    DDRB |= (1 << TOUCH_PIN);
    PORTB &= ~(1 << TOUCH_PIN);
    _delay_us(5);

    // Set as input (high impedance)
    DDRB &= ~(1 << TOUCH_PIN);
    PORTB &= ~(1 << TOUCH_PIN);

    // Count until pin reads HIGH
    while (!(PINB & (1 << TOUCH_PIN)) && count < 1000)
    {
        count++;
    }

    return count;
}

int main(void)
{
    DDRB |= (1 << LED_PIN);

    uint8_t led_state = 0;

    while (1)
    {
        uint16_t value = measure_touch();

        if (value > THRESHOLD)
        {
            led_state ^= 1;

            if (led_state)
                PORTB |= (1 << LED_PIN);
            else
                PORTB &= ~(1 << LED_PIN);

            _delay_ms(300); // simple lockout
        }

        _delay_ms(20);
    }
}
