#ifndef F_CPU
#define F_CPU 8000000UL
#endif

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/atomic.h>
#include <stdint.h>

/* ================= CONFIG ================= */

#define LED_PIN PB0
#define BTN_PIN PB3
#define BLINK_INTERVAL 500

/* ================= TIME BASE ================= */

volatile uint32_t g_tick = 0;

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

/* ================= EVENT SYSTEM ================= */

typedef enum {
    EVENT_NONE = 0,
    EVENT_BUTTON_PRESS,
    EVENT_TICK_500MS
} event_t;

volatile event_t pending_event = EVENT_NONE;

static void post_event(event_t e)
{
    pending_event = e;
}

/* ================= STATES ================= */

typedef enum {
    STATE_IDLE = 0,
    STATE_ON,
    STATE_BLINK
} state_t;

static state_t current_state;

/* ================= STATE HANDLERS ================= */

static void state_idle(event_t e)
{
    if (e == EVENT_BUTTON_PRESS)
    {
        PORTB |= (1 << LED_PIN);
        current_state = STATE_ON;
    }
}

static void state_on(event_t e)
{
    if (e == EVENT_BUTTON_PRESS)
    {
        current_state = STATE_BLINK;
    }
}

static void state_blink(event_t e)
{
    if (e == EVENT_BUTTON_PRESS)
    {
        PORTB &= ~(1 << LED_PIN);
        current_state = STATE_IDLE;
    }
    else if (e == EVENT_TICK_500MS)
    {
        PORTB ^= (1 << LED_PIN);
    }
}

/* ================= DISPATCHER ================= */

static void dispatch(event_t e)
{
    switch (current_state)
    {
        case STATE_IDLE:
            state_idle(e);
            break;

        case STATE_ON:
            state_on(e);
            break;

        case STATE_BLINK:
            state_blink(e);
            break;

        default:
            break;
    }
}

/* ================= BUTTON TASK ================= */

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
                post_event(EVENT_BUTTON_PRESS);
        }
    }
}

/* ================= PERIODIC EVENTS ================= */

static void periodic_task(void)
{
    static uint32_t last = 0;
    uint32_t now = millis();

    if ((now - last) >= BLINK_INTERVAL)
    {
        last = now;
        post_event(EVENT_TICK_500MS);
    }
}

/* ================= MAIN ================= */

int main(void)
{
    DDRB  |= (1 << LED_PIN);
    DDRB  &= ~(1 << BTN_PIN);
    PORTB |= (1 << BTN_PIN);

    timer_init();
    sei();

    current_state = STATE_IDLE;

    while (1)
    {
        button_task();
        periodic_task();

        if (pending_event != EVENT_NONE)
        {
            event_t e = pending_event;
            pending_event = EVENT_NONE;
            dispatch(e);
        }
    }
}
