#ifndef F_CPU
#define F_CPU 8000000UL
#endif

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/atomic.h>
#include <stdint.h>

/* ================= CONFIG ================= */

#define MAX_TASKS 4
#define LED_PIN   PB0
#define BTN_PIN   PB3

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

/* ================= KERNEL ================= */

typedef void (*task_func_t)(void);

typedef struct
{
    task_func_t func;
    uint32_t interval;
    uint32_t last_run;
    uint8_t enabled;
} task_t;

static task_t tasks[MAX_TASKS];

static void kernel_add_task(uint8_t id, task_func_t func, uint32_t interval)
{
    if (id >= MAX_TASKS)
        return;

    tasks[id].func = func;
    tasks[id].interval = interval;
    tasks[id].last_run = 0;
    tasks[id].enabled = 1;
}

static void kernel_run(void)
{
    uint32_t now = millis();

    for (uint8_t i = 0; i < MAX_TASKS; i++)
    {
        if (!tasks[i].enabled || tasks[i].func == 0)
            continue;

        if ((now - tasks[i].last_run) >= tasks[i].interval)
        {
            tasks[i].last_run = now;
            tasks[i].func();
        }
    }
}

/* ================= TASKS ================= */

static void task_blink(void)
{
    PORTB ^= (1 << LED_PIN);
}

static void task_button(void)
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
                PORTB ^= (1 << LED_PIN);
        }
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

    kernel_add_task(0, task_blink, 1000);
    kernel_add_task(1, task_button, 10);

    while (1)
    {
        kernel_run();
    }
}
