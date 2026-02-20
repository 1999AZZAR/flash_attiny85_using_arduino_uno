#ifndef F_CPU
#define F_CPU 8000000UL
#endif

#include <avr/io.h>
#include <util/delay.h>
#include <stdint.h>

#define LED_PIN     PB0      // OC0A
#define ADC_CHANNEL 2        // PB4 (ADC2)

#define FILTER_SIZE 16

static uint16_t adc_read(void)
{
    ADCSRA |= (1 << ADSC);
    while (ADCSRA & (1 << ADSC));
    return ADC;
}

static void adc_init(void)
{
    ADMUX =
        (1 << REFS0) |           // VCC reference
        (ADC_CHANNEL & 0x0F);    // Select channel

    ADCSRA =
        (1 << ADEN)  |           // Enable ADC
        (1 << ADPS2) | (1 << ADPS1);  // Prescaler /64 (125kHz ADC clock)
}

static void pwm_init(void)
{
    DDRB |= (1 << LED_PIN);

    TCCR0A =
        (1 << WGM00) |
        (1 << WGM01) |
        (1 << COM0A1);

    TCCR0B = (1 << CS01);  // /8
}

int main(void)
{
    adc_init();
    pwm_init();

    uint16_t buffer[FILTER_SIZE] = {0};
    uint32_t sum = 0;
    uint8_t index = 0;

    while (1)
    {
        sum -= buffer[index];

        buffer[index] = adc_read();
        sum += buffer[index];

        index++;
        if (index >= FILTER_SIZE)
            index = 0;

        uint16_t average = sum / FILTER_SIZE;

        // 10-bit ADC (0-1023) → 8-bit PWM (0-255)
        OCR0A = average >> 2;

        _delay_ms(5);
    }
}
