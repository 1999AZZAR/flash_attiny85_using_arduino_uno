# ATtiny85 Multi-Mode LED Controller (Refined)

Part 1.6 of the Bare Metal ATtiny85 Series. 

A deterministic, interrupt-driven firmware for the **ATtiny85 @ 8MHz** demonstrating deterministic timing, hardware PWM, and a cooperative task model with power management.

---

## Overview

A single push-button interface that cycles through four distinct LED modes. This refined version focuses on **State Isolation** and **Power Management**.

Design Goals:
*   **Zero Blocking Delays**: The main loop never stalls; all timing is delta-based.
*   **Deterministic 1ms Tick**: Derived from a hardware Timer1 CTC interrupt.
*   **Hardware PWM**: Brightness is handled entirely by the Timer0 hardware engine.
*   **Power Optimization**: CPU enters `IDLE` sleep mode between task iterations.

---

## LED Modes (Short Press to Cycle)

| Mode | Behavior                                |
| ---- | --------------------------------------- |
| 0    | **OFF** – LED fully disabled            |
| 1    | **25%** – Low brightness                |
| 2    | **50%** – Medium brightness             |
| 3    | **75%** – High brightness               |
| 4    | **100%** – Full brightness              |
| 5    | **FLASHING** – Tactical 20Hz toggle (25ms)|

Transitions are immediate; internal task states reset automatically upon mode change.

---

## Technical Highlights

### 1. Timer1 — The System Clock
Instead of using the heavy Arduino `millis()`, we use **Timer1** in **CTC Mode** (Clear Timer on Compare Match) to generate a precise 1ms interrupt.
*   **Prescaler**: /64
*   **Compare Match (`OCR1C`)**: 124 (125 counts including zero)
*   **Frequency**: 8,000,000 / 64 / 125 = 1,000 Hz

### 2. Timer0 — The Dimming Engine
We use **Fast PWM** mode on Pin 5 (**PB0/OC0A**) to handle LED brightness without CPU intervention.
*   **Resolution**: 8-bit (0-255)
*   **Frequency**: ~3.9kHz (8MHz / 8 / 256)

### 3. Idle Sleep Management
To save battery life, the `main` loop ends each cycle with `sleep_cpu()`.
*   The CPU sleeps in **Idle Mode**.
*   The Timer1 interrupt wakes the CPU every 1ms to process the tasks.
*   This significantly reduces power consumption compared to a "busy-wait" loop.

---

## Hardware Configuration

| ATtiny85 Pin | Signal | Description |
|---|---|---|
| **Pin 5 (PB0)** | LED | Anode (+) via 220Ω resistor to GND |
| **Pin 2 (PB3)** | Button | Momentary button to GND (Internal Pull-up) |
| **Pin 8 (VCC)** | Power | 2.7V - 5.5V |
| **Pin 4 (GND)** | Ground | Common Ground |

---

## Build and Flash

```bash
# Compile and Upload
make flash

# Set Fuses (8MHz Internal)
make fuses
```
