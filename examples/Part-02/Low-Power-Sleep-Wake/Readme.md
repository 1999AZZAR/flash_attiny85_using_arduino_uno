# ATtiny85 Low Power Sleep + Wake

Demonstrates deep sleep mode using **Power-Down** with wake-up via **Pin Change Interrupt**.

This is a foundation for ultra-low-power battery devices.

---

## Features

* Deepest sleep mode (Power-Down)
* Wake on button press
* Interrupt-driven wake
* No polling
* Minimal active time

---

## Hardware Connection

| ATtiny85 Pin | Component         |
| ------------ | ----------------- |
| Pin 5 (PB0)  | LED (via 220Ω)    |
| Pin 2 (PB3)  | Push Button → GND |
| Pin 8        | VCC               |
| Pin 4        | GND               |

Button uses internal pull-up.

---

## Behavior

1. MCU enters Power-Down sleep.
2. Button press triggers Pin Change Interrupt.
3. MCU wakes.
4. LED toggles.
5. MCU immediately returns to sleep.

---

## Why This Matters

In Power-Down mode:

* CPU halted
* Timers stopped
* Flash inactive
* Current consumption drops dramatically

With proper fuse + no peripherals active:

* Can reach single-digit µA range

---

## Power Optimization Tips

* Disable ADC before sleep (`ADCSRA &= ~(1 << ADEN);`)
* Disable analog comparator
* Use lower VCC (3.3V)
* Enable Brown-Out only if required

---

## What This Teaches

* Interrupt-based wake
* Sleep state management
* Ultra-low-power design pattern
* Event-driven firmware
