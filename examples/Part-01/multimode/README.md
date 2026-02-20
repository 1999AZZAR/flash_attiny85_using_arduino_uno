# ATtiny85 Multi-Mode LED Controller (The Final Stand - V2)

Part 1.6 of the Bare Metal ATtiny85 Series. 

## Overview
A single push-button interface that cycles through six distinct LED modes. This refinement uses hardware PWM for both static dimming and high-speed pulsing.

---

## LED Modes (Short Press to Cycle)

| Mode | Behavior                                |
| ---- | --------------------------------------- |
| 0    | **OFF** – LED fully disabled            |
| 1    | **25%** – Low brightness                |
| 2    | **50%** – Medium brightness             |
| 3    | **75%** – High brightness               |
| 4    | **100%** – Full brightness              |
| 5    | **FLASHING** – Pulsing (6ms step)          |

---

## Key Refinements
- **Pulsing Mode**: The old "Breathe" mode is tuned and renamed to "Flashing" to create a more dynamic, pulsing visual effect.
- **Hardware PWM Consistency**: Uses Timer0 hardware PWM for all active modes to ensure flicker-free dimming.
- **Zero-Latency Transition**: Internal task variables reset immediately upon mode change for instant tactile feedback.

---

## Build and Flash
```bash
make flash
```
