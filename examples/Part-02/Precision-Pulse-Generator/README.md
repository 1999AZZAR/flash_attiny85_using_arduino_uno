# ATtiny85 Precision Pulse Generator (Heartbeat)

Part 2.13 of the Bare Metal ATtiny85 Series.

## Overview
This example demonstrates how to use **Timer1** in **CTC (Clear Timer on Compare Match)** mode to generate a highly precise 1Hz pulse (heartbeat). This signal can be used as a master clock for external circuits or as a deterministic system heartbeat.

## Technical Highlights
- **Deterministic Timing**: Uses hardware interrupts to bypass main loop jitter.
- **Atomic Toggling**: Uses the `PINB` register to toggle the output pin in a single clock cycle.
- **Timer1 Specialization**: Leverages the ATtiny85's unique 8-bit high-speed timer.

## Implementation Details
- **Clock**: 8MHz Internal.
- **Prescaler**: /16384 (Timer1 specific).
- **Logic**: A software counter inside the ISR divides the slow timer ticks further to reach an exact 1Hz frequency.

## Hardware Connection
| ATtiny85 Pin | Signal | Notes |
|---|---|---|
| **Pin 5 (PB0)** | Pulse Output | 1Hz Heartbeat LED |
| **Pin 4 (GND)** | Ground | Common Ground |

## Build and Flash
```bash
make flash
```
