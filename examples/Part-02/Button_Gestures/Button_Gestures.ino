/*
 * ATtiny85 Button Gestures (Short, Long, Double Press)
 * Demonstrates advanced user input handling with a single button.
 * 
 * Hardware:
 * - LED on PB0 (Pin 5)
 * - Button on PB3 (Pin 2) -> connects to GND
 * 
 * Logic:
 * - Single Click: Toggle LED
 * - Double Click: Flash LED twice quickly
 * - Long Press (>1s): LED stays on while held, then fades out
 */

#ifndef F_CPU
#define F_CPU 8000000UL
#endif

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/atomic.h>
#include <stdint.h>

/* ================= CONFIG ================= */

#define LED_PIN         PB0
#define BTN_PIN         PB3

#define DEBOUNCE_MS     50
#define DOUBLE_CLICK_MS 400
#define LONG_PRESS_MS   1000

/* ================= TIME BASE ================= */

volatile uint32_t g_tick = 0;

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

static void timer_init(void) {
    TCCR1  = (1 << CTC1) | (1 << CS11) | (1 << CS10); // clk/64
    OCR1C  = 124; // 1ms @ 8MHz
    TIMSK |= (1 << OCIE1A);
}

/* ================= GESTURE LOGIC ================= */

typedef enum {
    EVENT_NONE = 0,
    EVENT_SINGLE_CLICK,
    EVENT_DOUBLE_CLICK,
    EVENT_LONG_PRESS_START,
    EVENT_LONG_PRESS_RELEASE
} event_t;

static void handle_event(event_t e) {
    switch (e) {
        case EVENT_SINGLE_CLICK:
            PORTB ^= (1 << LED_PIN);
            break;

        case EVENT_DOUBLE_CLICK:
            // Quick double flash
            for (uint8_t i = 0; i < 2; i++) {
                PORTB |= (1 << LED_PIN);
                for (volatile uint32_t j = 0; j < 50000; j++);
                PORTB &= ~(1 << LED_PIN);
                for (volatile uint32_t j = 0; j < 50000; j++);
            }
            break;

        case EVENT_LONG_PRESS_START:
            PORTB |= (1 << LED_PIN);
            break;

        case EVENT_LONG_PRESS_RELEASE:
            PORTB &= ~(1 << LED_PIN);
            break;

        default:
            break;
    }
}

static void button_task(void) {
    static uint8_t  button_held = 0;
    static uint32_t last_press_time = 0;
    static uint32_t last_release_time = 0;
    static uint8_t  click_count = 0;
    static uint8_t  is_long_pressing = 0;

    uint8_t pressed = !(PINB & (1 << BTN_PIN));
    uint32_t now = millis();

    // 1. Detect State Change
    if (pressed && !button_held) {
        // Press Event
        if ((now - last_release_time) > DEBOUNCE_MS) {
            button_held = 1;
            last_press_time = now;
        }
    } 
    else if (!pressed && button_held) {
        // Release Event
        if ((now - last_press_time) > DEBOUNCE_MS) {
            button_held = 0;
            uint32_t press_duration = now - last_press_time;
            last_release_time = now;

            if (is_long_pressing) {
                is_long_pressing = 0;
                handle_event(EVENT_LONG_PRESS_RELEASE);
            } else {
                click_count++;
            }
        }
    }

    // 2. Process Timing
    if (button_held && !is_long_pressing) {
        if ((now - last_press_time) >= LONG_PRESS_MS) {
            is_long_pressing = 1;
            click_count = 0; // Cancel multi-click if held
            handle_event(EVENT_LONG_PRESS_START);
        }
    }

    if (!button_held && click_count > 0) {
        if ((now - last_release_time) >= DOUBLE_CLICK_MS) {
            if (click_count == 1) handle_event(EVENT_SINGLE_CLICK);
            else handle_event(EVENT_DOUBLE_CLICK);
            click_count = 0;
        }
    }
}

/* ================= MAIN ================= */

int main(void) {
    DDRB  |= (1 << LED_PIN);
    DDRB  &= ~(1 << BTN_PIN);
    PORTB |= (1 << BTN_PIN); // Pull-up

    timer_init();
    sei();

    while (1) {
        button_task();
    }
}
