/*
 * ATtiny85 Multi-Mode Controller - 8MHz Internal
 * Security: Uses hardware timers for non-blocking operation
 * Performance: Interrupt-driven system tick and hardware PWM
 *
 * Modes:
 * 0: OFF
 * 1: ON (100% Brightness)
 * 2: BLINK (2Hz)
 * 3: BREATHE (Smooth Fade)
 */

#ifndef F_CPU
#define F_CPU 8000000UL
#endif

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

// Pins
#define LED_PIN PB0
#define BTN_PIN PB3

// Timing Constants
#define BLINK_INTERVAL 250 // ms
#define DEBOUNCE_DELAY 50  // ms
#define FADE_DELAY 5       // ms

// Global System Tick (incremented every 1ms)
volatile uint32_t sys_tick = 0;

// Button State
uint8_t current_mode = 0;
uint8_t last_btn_state = 1; // 1 = Released (Input Pull-up)
uint32_t last_debounce_time = 0;

// Timer1 Compare Match A Interrupt Service Routine
// Configured to run every 1ms
ISR(TIMER1_COMPA_vect) {
    sys_tick++;
}

// Simple millis() implementation
uint32_t millis(void) {
    uint32_t m;
    // Atomic read of multi-byte variable
    uint8_t oldSREG = SREG;
    cli();
    m = sys_tick;
    SREG = oldSREG;
    return m;
}

void setup_timers(void) {
    // Timer0: Fast PWM for LED Brightness (PB0/OC0A)
    // WGM00|WGM01 = Fast PWM, COM0A1 = Clear on Match
    TCCR0A = (1 << WGM00) | (1 << WGM01) | (1 << COM0A1);
    TCCR0B = (1 << CS01); // Prescaler /8 (~3.9kHz PWM)

    // Timer1: CTC Mode for 1ms System Tick
    // Clear Timer on Compare Match (CTC)
    TCCR1 = (1 << CTC1) | (1 << CS12) | (1 << CS11) | (1 << CS10); // Prescaler /64
    // 8MHz / 64 = 125kHz -> 125 ticks = 1ms
    OCR1C = 124; // OCR1C determines the TOP value for Timer1 in CTC mode
    TIMSK |= (1 << OCIE1A); // Enable Compare Match A Interrupt
}

void handle_button(void) {
    // Read button state (0 = Pressed, 1 = Released)
    uint8_t reading = (PINB & (1 << BTN_PIN)) ? 1 : 0;

    // Check if state changed
    if (reading != last_btn_state) {
        last_debounce_time = millis();
    }

    // Check if stable state is different from logic state
    // We only care about the PRESS action (High -> Low transition logic)
    // But since we track "last_btn_state" which is the raw reading...
    // Let's simplify: detect transition from 1 to 0 stable.
    
    // Actually, simpler logic:
    // If reading is different from stored "stable" state after debounce delay
    static uint8_t stable_state = 1;
    
    if ((millis() - last_debounce_time) > DEBOUNCE_DELAY) {
        if (reading != stable_state) {
            stable_state = reading;

            // Action on Press (Active Low)
            if (stable_state == 0) {
                current_mode++;
                if (current_mode > 3) current_mode = 0;
            }
        }
    }

    last_btn_state = reading;
}

void handle_led(void) {
    static uint32_t last_led_update = 0;
    static uint8_t brightness = 0;
    static int8_t fade_dir = 1;
    static uint8_t blink_state = 0;

    switch (current_mode) {
        case 0: // OFF
            OCR0A = 0; // 0% Duty Cycle
            break;

        case 1: // ON
            OCR0A = 255; // 100% Duty Cycle
            break;

        case 2: // BLINK
            if (millis() - last_led_update >= BLINK_INTERVAL) {
                last_led_update = millis();
                blink_state = !blink_state;
                OCR0A = blink_state ? 255 : 0;
            }
            break;

        case 3: // BREATHE
            if (millis() - last_led_update >= FADE_DELAY) {
                last_led_update = millis();
                
                OCR0A = brightness;
                brightness += fade_dir;

                if (brightness == 0 || brightness == 255) {
                    fade_dir = -fade_dir;
                }
            }
            break;
    }
}

int main(void) {
    // Setup Hardware
    DDRB |= (1 << LED_PIN);   // LED Output
    DDRB &= ~(1 << BTN_PIN);  // Button Input
    PORTB |= (1 << BTN_PIN);  // Enable Pull-up on Button

    setup_timers();
    sei(); // Enable Global Interrupts

    while (1) {
        handle_button();
        handle_led();
        // The loop is non-blocking! CPU can sleep or do other tasks here.
    }

    return 0;
}
