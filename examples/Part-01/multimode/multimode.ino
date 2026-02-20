/*
 * ATtiny85 Multi-Mode LED Controller (Rock Solid Tactical)
 * Part 1.6 of the Bare Metal ATtiny85 Series
 * 
 * Logic:
 * - Timer1: 1ms high-precision system tick (CTC mode)
 * - Timer0: Fast PWM (OC0A -> PB0) for dimming
 * - Sequence: OFF > 25% > 50% > 75% > 100% > Quick Flashing (50ms)
 */

#ifndef F_CPU
#define F_CPU 8000000UL
#endif

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <util/atomic.h>
#include <stdint.h>

// ======================== CONFIGURATION ========================

#define LED_PIN         PB0   // Pin 5
#define BTN_PIN         PB3   // Pin 2

#define FLASH_INTERVAL  50U   // 50ms (10Hz) - More visible than 25ms
#define DEBOUNCE_MS     50U   // 50ms stable window

// ======================== GLOBAL STATE ========================

typedef enum {
    MODE_OFF = 0,
    MODE_DIM_25,
    MODE_DIM_50,
    MODE_DIM_75,
    MODE_ON_100,
    MODE_FLASHING,
    MODE_MAX
} mode_t;

static volatile uint32_t g_tick = 0;
static mode_t g_mode = MODE_OFF;
static uint8_t g_mode_changed = 0; 

// ======================== TIMING & INTERRUPTS ========================

ISR(TIMER1_COMPA_vect) {
    g_tick++;
}

static uint32_t millis(void) {
    uint32_t t;
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
        t = g_tick;
    }
    return t;
}

// ======================== INITIALIZATION ========================

static void hardware_init(void) {
    DDRB  |= (1 << LED_PIN);
    DDRB  &= ~(1 << BTN_PIN);
    PORTB |= (1 << BTN_PIN);

    // Timer0: Fast PWM
    TCCR0A = (1 << WGM00) | (1 << WGM01); 
    TCCR0B = (1 << CS01); // clk/8
    OCR0A  = 0;

    // Timer1: 1ms Tick
    TCCR1  = (1 << CTC1) | (1 << CS11) | (1 << CS10); // clk/64
    OCR1C  = 124;
    TIMSK |= (1 << OCIE1A);

    set_sleep_mode(SLEEP_MODE_IDLE);
}

// ======================== TASKS ========================

static void task_button(void) {
    static uint8_t button_state = 1;
    static uint32_t last_debounce_time = 0;
    static uint8_t last_reading = 1;

    uint8_t reading = (PINB & (1 << BTN_PIN)) ? 1 : 0;
    uint32_t now = millis();

    if (reading != last_reading) {
        last_debounce_time = now;
        last_reading = reading;
    }

    if ((now - last_debounce_time) > DEBOUNCE_MS) {
        if (reading != button_state) {
            button_state = reading;
            if (button_state == 0) {
                g_mode = (mode_t)((g_mode + 1) % MODE_MAX);
                g_mode_changed = 1; 
            }
        }
    }
}

static void task_led(void) {
    static uint32_t last_update = 0;
    static uint8_t flash_val = 0;
    uint32_t now = millis();

    if (g_mode_changed) {
        last_update = now;
        g_mode_changed = 0;
        flash_val = 0;
        
        // Ensure PWM hardware is connected only when needed
        if (g_mode >= MODE_DIM_25 && g_mode <= MODE_ON_100) {
            TCCR0A |= (1 << COM0A1);
        } else {
            TCCR0A &= ~(1 << COM0A1);
            PORTB &= ~(1 << LED_PIN); // Physical OFF
        }
    }

    switch (g_mode) {
        case MODE_OFF:
            break;

        case MODE_DIM_25:
            OCR0A = 64;
            break;

        case MODE_DIM_50:
            OCR0A = 127;
            break;

        case MODE_DIM_75:
            OCR0A = 191;
            break;

        case MODE_ON_100:
            OCR0A = 255;
            break;

        case MODE_FLASHING:
            if ((now - last_update) >= FLASH_INTERVAL) {
                last_update = now;
                flash_val ^= 1;
                if (flash_val) {
                    PORTB |= (1 << LED_PIN);
                } else {
                    PORTB &= ~(1 << LED_PIN);
                }
            }
            break;

        default:
            break;
    }
}

int main(void) {
    hardware_init();
    sei();

    while (1) {
        task_button();
        task_led();
        
        sleep_enable();
        sleep_cpu();
        sleep_disable();
    }
}
