# ATtiny85 Multi-Mode LED Controller

Interrupt-driven, non-blocking firmware for **ATtiny85 @ 8 MHz (internal oscillator)** demonstrating deterministic timing, hardware PWM, and a cooperative task model.

---

## Overview

Single push-button interface controlling four LED modes.

Design goals:

* Zero blocking delays
* Deterministic 1 ms system tick
* Hardware-driven PWM (no software bit-banging)
* Clean state isolation

The main loop never stalls. All timing is derived from a hardware interrupt.

---

## Modes (Short Press to Cycle)

| Mode | Behavior                                |
| ---- | --------------------------------------- |
| 0    | **OFF** – LED disabled                  |
| 1    | **ON** – 100% duty cycle                |
| 2    | **BLINK** – 2 Hz (250 ms toggle)        |
| 3    | **BREATHE** – Smooth linear fade in/out |

Mode wraps automatically after the last state.

---

## Hardware Connections

| ATtiny85 Pin       | Signal | Description                         |
| ------------------ | ------ | ----------------------------------- |
| Pin 5 (PB0 / OC0A) | LED    | Anode → PB0, Cathode → GND via 220Ω |
| Pin 2 (PB3)        | Button | One side → PB3, other → GND         |
| Pin 8              | VCC    | 2.7V – 5.5V                         |
| Pin 4              | GND    | Common ground                       |

Button uses internal pull-up (active LOW).

---

## Architecture

### Timer0 — PWM Engine

* Mode: Fast PWM
* Output: OC0A (PB0)
* Prescaler: /8
* Frequency ≈ 3.9 kHz

Handles brightness fully in hardware.

---

### Timer1 — System Tick

* Mode: CTC
* Prescaler: /64
* Compare: 124
* Interrupt rate: 1 ms

Generates a global millisecond counter used for:

* Debounce logic
* Blink scheduling
* Fade stepping

All time-based logic derives from this single source of truth.

---

## Task Model

Main loop:

```
while (1) {
    button_task();
    led_task();
}
```

No delays. No blocking calls.

### Button Handling

* Edge-triggered (falling edge)
* 50 ms software debounce
* Mode increment with wrap

### LED Scheduling

Uses delta-time checks:

```
if (now - last_update >= interval)
```

Ensures cooperative multitasking behavior.

---

## Technical Characteristics

* Fully interrupt-driven timing
* Atomic multi-byte access protection
* Deterministic scheduling
* Minimal RAM footprint
* No dynamic memory
* Compatible with ATtiny25 / ATtiny45 (memory permitting)

---

## Fuse Configuration (Recommended)

For 8 MHz internal oscillator:

* CKDIV8 disabled
* Internal 8 MHz clock
* Brown-out detection optional (recommended for stability)

---

## Build & Flash

Using Makefile + avrdude:

```bash
make flash
```

Or Arduino core (ATtinyCore / similar) with:

* Clock: 8 MHz internal
* Programmer: ISP

---

## Extension Ideas

* Gamma-corrected breathing curve
* Long-press detection
* Double-click mode switching
* Sleep mode between ticks (power optimization)
* Interrupt-based button instead of polling

---

## Resource Usage

Designed to run comfortably on:

* ATtiny85 (8 KB flash)
* ATtiny45 (4 KB flash)
* ATtiny25 (optimized build required)