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

#define QUEUE_SIZE 8
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
    TCCR1  = (1 << CTC1) | (1 << CS11) | (1 << CS10);
    OCR1C  = 124; // 1ms
    TIMSK |= (1 << OCIE1A);
}

/* ================= EVENT SYSTEM ================= */

typedef enum {
    EVENT_NONE = 0,
    EVENT_BUTTON_PRESS,
    EVENT_TICK_500MS
} event_t;

typedef struct {
    event_t buffer[QUEUE_SIZE];
    uint8_t head;
    uint8_t tail;
} event_queue_t;

static event_queue_t queue;

static uint8_t queue_is_empty(void)
{
    return queue.head == queue.tail;
}

static uint8_t queue_is_full(void)
{
    return ((queue.head + 1) % QUEUE_SIZE) == queue.tail;
}

static void post_event(event_t e)
{
    if (queue_is_full())
        return; // drop if full (safe for tiny systems)

    queue.buffer[queue.head] = e;
    queue.head = (queue.head + 1) % QUEUE_SIZE;
}

static event_t get_event(void)
{
    if (queue_is_empty())
        return EVENT_NONE;

    event_t e = queue.buffer[queue.tail];
    queue.tail = (queue.tail + 1) % QUEUE_SIZE;
    return e;
}

/* ================= FSM ================= */

typedef enum {
    STATE_IDLE = 0,
    STATE_ON,
    STATE_BLINK,
    STATE_COUNT
} state_t;

typedef void (*state_handler_t)(event_t);

static state_t current_state;

/* -------- State Implementations -------- */

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

/* -------- State Table -------- */

static const state_handler_t state_table[STATE_COUNT] = {
    state_idle,
    state_on,
    state_blink
};

/* ================= TASKS ================= */

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

        event_t e = get_event();
        if (e != EVENT_NONE)
        {
            state_table[current_state](e);
        }
    }
}
