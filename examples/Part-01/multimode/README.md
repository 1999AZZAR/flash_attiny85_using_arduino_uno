# ATtiny85 Multi-Mode LED Controller (The Final Stand)

Part 1.6 of the Bare Metal ATtiny85 Series. 

## Overview
A single push-button interface that cycles through seven distinct LED modes. This final refinement focuses on **Strict Hardware Isolation** between PWM (dimming) and Software Toggling (flashing).

---

## LED Modes (Short Press to Cycle)

| Mode | Behavior                                |
| ---- | --------------------------------------- |
| 0    | **OFF** – LED fully disabled (PWM cut)  |
| 1    | **25%** – Low brightness                |
| 2    | **50%** – Medium brightness             |
| 3    | **75%** – High brightness               |
| 4    | **100%** – Full brightness              |
| 5    | **BREATHE** – Smooth 5s cycle fade      |
| 6    | **FLASHING** – Tactical 20Hz toggle     |

---

## Key Refinements
- **PWM Isolation**: The hardware PWM (`TCCR0A`) is physically disconnected when entering **OFF** or **FLASHING** modes to prevent Timer/PWM conflicts.
- **Manual Overdrive**: Flashing is now handled by direct `PORTB` manipulation once the PWM hardware is released.
- **Zero-Latency Transistion**: Internal task variables (timers, brightness) reset immediately upon mode change.

---

## Build and Flash
```bash
make flash
```
