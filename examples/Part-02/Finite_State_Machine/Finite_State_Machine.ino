#ifndef F_CPU
#define F_CPU 8000000UL
#endif

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/atomic.h>
#include <stdint.h>

#define LED_PIN PB0
#define BTN_PIN PB3

#define TICK_MS 1
#define BLINK_INTERVAL 500

volatile uint32_t g_tick = 0;

/* ===================== TIMER ===================== */

ISR(TIMER1_COMPA_vect)
{
    g_tick++;
}

static inline uint32_t millis(void)
{
    uint32_t t;
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
    {
        t = g_tick;
    }
    return t;
}

static void timer_init(void)
{
    TCCR1  = (1 << CTC1) | (1 << CS11) | (1 << CS10); // /64
    OCR1C  = 124;                                     // 1ms @ 8MHz
    TIMSK |= (1 << OCIE1A);
}

/* ===================== FSM CORE ===================== */

typedef enum {
    STATE_IDLE = 0,
    STATE_ON,
    STATE_BLINK
} state_t;

static state_t current_state;

static void state_enter(state_t s)
{
    switch (s)
    {
        case STATE_IDLE:
            PORTB &= ~(1 << LED_PIN);
            break;

        case STATE_ON:
            PORTB |= (1 << LED_PIN);
            break;

        case STATE_BLINK:
            break;

        default:
            break;
    }
}

static void state_update(void)
{
    static uint32_t last_toggle = 0;

    switch (current_state)
    {
        case STATE_IDLE:
            break;

        case STATE_ON:
            break;

        case STATE_BLINK:
        {
            uint32_t now = millis();
            if ((now - last_toggle) >= BLINK_INTERVAL)
            {
                last_toggle = now;
                PORTB ^= (1 << LED_PIN);
            }
            break;
        }

        default:
            break;
    }
}

static void state_transition(state_t next)
{
    if (next == current_state)
        return;

    current_state = next;
    state_enter(next);
}

/* ===================== BUTTON ===================== */

static void button_task(void)
{
    static uint8_t stable = 1;
    static uint8_t last_sample = 1;
    static uint32_t last_change = 0;

    uint32_t now = millis();
    uint8_t sample = (PINB & (1 << BTN_PIN)) ? 1 : 0;

    if (sample != last_sample)
    {
        last_change = now;
        last_sample = sample;
    }

    if ((now - last_change) >= 50)
    {
        if (sample != stable)
        {
            stable = sample;

            if (stable == 0)
            {
                // Cycle states
                if (current_state == STATE_IDLE)
                    state_transition(STATE_ON);
                else if (current_state == STATE_ON)
                    state_transition(STATE_BLINK);
                else
                    state_transition(STATE_IDLE);
            }
        }
    }
}

/* ===================== MAIN ===================== */

int main(void)
{
    DDRB  |= (1 << LED_PIN);
    DDRB  &= ~(1 << BTN_PIN);
    PORTB |= (1 << BTN_PIN);

    timer_init();
    sei();

    current_state = STATE_IDLE;
    state_enter(current_state);

    while (1)
    {
        button_task();
        state_update();
    }
}
