/*
 * ATtiny85 Multi-Mode LED Controller (8MHz Internal)
 * - Timer1: 1ms system tick (CTC)
 * - Timer0: Fast PWM (OC0A → PB0)
 * - Non-blocking state machine
 */

#ifndef F_CPU
#define F_CPU 8000000UL
#endif

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/atomic.h>
#include <stdint.h>

// ======================== CONFIG ========================

#define LED_PIN         PB0
#define BTN_PIN         PB3

#define BLINK_INTERVAL  250U    // ms
#define DEBOUNCE_MS     50U
#define FADE_STEP_MS    5U

// ======================== GLOBAL ========================

static volatile uint32_t g_tick = 0;

typedef enum {
    MODE_OFF = 0,
    MODE_ON,
    MODE_BLINK,
    MODE_BREATHE,
    MODE_MAX
} mode_t;

static mode_t g_mode = MODE_OFF;

// ======================== TIME ========================

ISR(TIMER1_COMPA_vect)
{
    g_tick++;
}

static inline uint32_t millis(void)
{
    uint32_t t;
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
        t = g_tick;
    }
    return t;
}

// ======================== HARDWARE ========================

static void timer_init(void)
{
    // --- Timer0: Fast PWM on OC0A (PB0) ---
    TCCR0A = (1 << WGM00) | (1 << WGM01) | (1 << COM0A1);
    TCCR0B = (1 << CS01);                 // clk/8 → ~3.9kHz PWM
    OCR0A  = 0;

    // --- Timer1: 1ms tick ---
    // 8MHz / 64 = 125kHz → 125 counts = 1ms
    TCCR1  = (1 << CTC1) | (1 << CS11) | (1 << CS10); // clk/64
    OCR1C  = 124;
    TIMSK |= (1 << OCIE1A);
}

static void io_init(void)
{
    DDRB  |= (1 << LED_PIN);
    DDRB  &= ~(1 << BTN_PIN);
    PORTB |= (1 << BTN_PIN); // pull-up
}

// ======================== BUTTON ========================

static void button_task(void)
{
    static uint8_t stable = 1;
    static uint8_t last_sample = 1;
    static uint32_t last_change = 0;

    uint32_t now = millis();
    uint8_t sample = (PINB & (1 << BTN_PIN)) ? 1 : 0;

    if (sample != last_sample) {
        last_change = now;
        last_sample = sample;
    }

    if ((now - last_change) >= DEBOUNCE_MS) {
        if (sample != stable) {
            stable = sample;

            if (stable == 0) {   // falling edge (pressed)
                g_mode = static_cast<mode_t>((g_mode + 1) % MODE_MAX);
            }
        }
    }
}

// ======================== LED ========================

static void led_task(void)
{
    static uint32_t last_update = 0;
    static uint8_t  brightness = 0;
    static int8_t   dir = 1;
    static uint8_t  blink = 0;

    uint32_t now = millis();

    switch (g_mode)
    {
        case MODE_OFF:
            OCR0A = 0;
            break;

        case MODE_ON:
            OCR0A = 255;
            break;

        case MODE_BLINK:
            if ((now - last_update) >= BLINK_INTERVAL) {
                last_update = now;
                blink ^= 1;
                OCR0A = blink ? 255 : 0;
            }
            break;

        case MODE_BREATHE:
            if ((now - last_update) >= FADE_STEP_MS) {
                last_update = now;

                brightness += dir;
                if (brightness == 0 || brightness == 255)
                    dir = -dir;

                OCR0A = brightness;
            }
            break;

        default:
            g_mode = MODE_OFF;
            break;
    }
}

// ======================== MAIN ========================

int main(void)
{
    io_init();
    timer_init();
    sei();

    while (1) {
        button_task();
        led_task();
        // Idle space → can add sleep_cpu() for ultra-low power
    }
}