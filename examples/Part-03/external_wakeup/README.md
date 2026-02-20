# ATtiny85 Pin Change Interrupt (PCINT) Wakeup

Part 3.4 of the Bare Metal ATtiny85 Series.

## Overview
This is the holy grail of low-power consumption. Instead of polling a button or using a timer, the CPU is completely shut down (**Power-Down Mode**). It stays in this state indefinitely until an external event (button press) triggers a **Pin Change Interrupt (PCINT)**.

## Key Concepts
- **`GIMSK` (General Interrupt Mask Register):** Enable the Pin Change Interrupt system.
- **`PCMSK` (Pin Change Mask Register):** Select which specific pin should trigger the interrupt.
- **`MCUCR` (BODS):** Disabling Brown-Out Detection during sleep to drop below 1µA.

## Hardware Setup
1. Connect a momentary push-button between **PB3 (Pin 2)** and **GND**.
2. Connect an LED (with 220Ω resistor) to **PB0 (Pin 5)** to see the wakeup activity.

## Build and Flash
```bash
make flash
```
