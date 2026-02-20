#ifndef F_CPU
#define F_CPU 8000000UL
#endif

#include <avr/io.h>
#include <avr/eeprom.h>
#include <util/delay.h>
#include <stdint.h>

#define LED_PIN PB0
#define BTN_PIN PB3

#define EEPROM_ADDR 0x00

uint8_t EEMEM stored_state;   // EEPROM variable

static void io_init(void)
{
    DDRB  |= (1 << LED_PIN);
    DDRB  &= ~(1 << BTN_PIN);
    PORTB |= (1 << BTN_PIN); // pull-up
}

static void led_set(uint8_t state)
{
    if (state)
        PORTB |= (1 << LED_PIN);
    else
        PORTB &= ~(1 << LED_PIN);
}

int main(void)
{
    io_init();

    uint8_t led_state = eeprom_read_byte(&stored_state);

    if (led_state > 1)
        led_state = 0;

    led_set(led_state);

    while (1)
    {
        if (!(PINB & (1 << BTN_PIN)))  // pressed (active low)
        {
            _delay_ms(50);             // simple debounce
            if (!(PINB & (1 << BTN_PIN)))
            {
                led_state ^= 1;
                led_set(led_state);

                eeprom_update_byte(&stored_state, led_state);

                while (!(PINB & (1 << BTN_PIN))); // wait release
            }
        }
    }
}
