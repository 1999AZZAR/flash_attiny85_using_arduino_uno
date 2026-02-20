/*
 * ATtiny85 Multi-Mode LED Controller (Refined)
 * Part 1.6 of the Bare Metal ATtiny85 Series
 * 
 * Logic:
 * - Timer1: 1ms high-precision system tick (CTC mode)
 * - Timer0: Fast PWM (OC0A -> PB0) for smooth hardware dimming
 * - Non-blocking state machine with cooperative multitasking
 * - Low-power Idle sleep when the CPU is not processing tasks
 * 
 * Target: ATtiny85 @ 8MHz Internal Oscillator
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

#define LED_PIN         PB0   // OC0A (Pin 5)
#define BTN_PIN         PB3   // Input (Pin 2)

#define BLINK_INTERVAL  250U  // 250ms (2Hz)
#define DEBOUNCE_MS     50U   // 50ms stable window
#define FADE_STEP_MS    6U    // Smoothness of the breathing effect

// ======================== GLOBAL STATE ========================

typedef enum {
    MODE_OFF = 0,
    MODE_ON,
    MODE_BLINK,
    MODE_BREATHE,
    MODE_MAX
} mode_t;

static volatile uint32_t g_tick = 0;
static mode_t g_mode = MODE_OFF;
static uint8_t g_mode_changed = 0; // Flag to reset task internals

// ======================== TIMING & INTERRUPTS ========================

// Timer1 CTC Interrupt: Fires every 1ms
ISR(TIMER1_COMPA_vect) {
    g_tick++;
}

// Thread-safe millisecond counter
static uint32_t millis(void) {
    uint32_t t;
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
        t = g_tick;
    }
    return t;
}

// ======================== INITIALIZATION ========================

static void hardware_init(void) {
    // 1. GPIO Setup
    DDRB  |= (1 << LED_PIN);       // LED as output
    DDRB  &= ~(1 << BTN_PIN);      // Button as input
    PORTB |= (1 << BTN_PIN);       // Enable internal pull-up

    // 2. Timer0: Hardware Fast PWM (8-bit)
    // WGM00|WGM01: Fast PWM mode
    // COM0A1: Non-inverting PWM on OC0A (PB0)
    TCCR0A = (1 << WGM00) | (1 << WGM01); // Don't connect COM0A1 yet
    // CS01: Prescaler clk/8 -> ~3.9kHz PWM frequency
    TCCR0B = (1 << CS01); 
    OCR0A  = 0;                    // Start at 0% duty cycle

    // 3. Timer1: 1ms System Tick Generator (CTC Mode)
    // 8MHz / 64 = 125,000Hz -> 125 counts = 1ms
    TCCR1  = (1 << CTC1) | (1 << CS11) | (1 << CS10); // clk/64
    OCR1C  = 124;                  // 0-indexed count to 125
    TIMSK |= (1 << OCIE1A);        // Enable Compare Match A interrupt

    // 4. Power Management
    set_sleep_mode(SLEEP_MODE_IDLE); // Stay in Idle to keep Timers running
}

// ======================== TASKS ========================

// Handle button debouncing and mode switching
static void task_button(void) {
    static uint8_t button_state = 1; // High (pull-up active)
    static uint32_t last_debounce_time = 0;
    
    uint8_t reading = (PINB & (1 << BTN_PIN)) ? 1 : 0;
    uint32_t now = millis();

    // Reset debounce timer if state changes
    static uint8_t last_reading = 1;
    if (reading != last_reading) {
        last_debounce_time = now;
        last_reading = reading;
    }

    // Process only if signal is stable for the debounce window
    if ((now - last_debounce_time) > DEBOUNCE_MS) {
        if (reading != button_state) {
            button_state = reading;
            
            // On Button Press (Falling Edge)
            if (button_state == 0) {
                g_mode = (mode_t)((g_mode + 1) % MODE_MAX);
                g_mode_changed = 1; // Signal tasks to reset their state
            }
        }
    }
}

// Handle LED behavior based on current mode
static void task_led(void) {
    static uint32_t last_update = 0;
    static uint8_t brightness = 0;
    static int8_t fade_dir = 1;
    static uint8_t blink_state = 0;

    uint32_t now = millis();

    // Reset local state variables on mode transition for immediate effect
    if (g_mode_changed) {
        last_update = now;
        brightness = 0;
        fade_dir = 1;
        blink_state = 0;
        g_mode_changed = 0;
        
        // Ensure LED starts from a known state when mode changes
        if (g_mode == MODE_OFF) {
            TCCR0A &= ~(1 << COM0A1); // Disconnect PWM hardware
            PORTB &= ~(1 << LED_PIN); // Force pin LOW
        } else {
            TCCR0A |= (1 << COM0A1);  // Reconnect PWM hardware
        }
    }

    switch (g_mode) {
        case MODE_OFF:
            // PWM disconnected in the change detection block above
            break;

        case MODE_ON:
            OCR0A = 255;
            break;

        case MODE_BLINK:
            if ((now - last_update) >= BLINK_INTERVAL) {
                last_update = now;
                blink_state ^= 1;
                OCR0A = blink_state ? 255 : 0;
            }
            break;

        case MODE_BREATHE:
            if ((now - last_update) >= FADE_STEP_MS) {
                last_update = now;
                
                // Linear fade logic
                brightness += fade_dir;
                if (brightness == 0 || brightness == 255) {
                    fade_dir = -fade_dir;
                }
                OCR0A = brightness;
            }
            break;

        default:
            g_mode = MODE_OFF;
            break;
    }
}

// ======================== MAIN LOOP ========================

int main(void) {
    hardware_init();
    sei(); // Global interrupts enabled

    while (1) {
        task_button();
        task_led();
        
        // Sleep in Idle mode between tasks to save power
        // The Timer1 interrupt will wake us up every 1ms
        sleep_enable();
        sleep_cpu();
        sleep_disable();
    }
}
