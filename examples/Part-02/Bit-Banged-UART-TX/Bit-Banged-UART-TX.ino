#ifndef F_CPU
#define F_CPU 8000000UL
#endif

#include <avr/io.h>
#include <util/delay.h>
#include <stdint.h>

#define TX_PIN PB1
#define BAUD_DELAY_US 104

static inline void uart_tx_bit(uint8_t bit)
{
    if (bit)
        PORTB |= (1 << TX_PIN);
    else
        PORTB &= ~(1 << TX_PIN);

    _delay_us(BAUD_DELAY_US);
}

void uart_tx_byte(uint8_t data)
{
    // Start bit (LOW)
    uart_tx_bit(0);

    // 8 data bits (LSB first)
    for (uint8_t i = 0; i < 8; i++) {
        uart_tx_bit(data & 0x01);
        data >>= 1;
    }

    // Stop bit (HIGH)
    uart_tx_bit(1);
}

void uart_tx_string(const char *s)
{
    while (*s) {
        uart_tx_byte(*s++);
    }
}

int main(void)
{
    DDRB |= (1 << TX_PIN);
    PORTB |= (1 << TX_PIN); // idle HIGH

    while (1) {
        uart_tx_string("Hello ATtiny85\r\n");
        _delay_ms(1000);
    }
}
