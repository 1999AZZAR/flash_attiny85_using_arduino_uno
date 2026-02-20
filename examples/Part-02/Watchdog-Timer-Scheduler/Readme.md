# ATtiny85 Watchdog Timer Scheduler

Uses the internal **Watchdog Timer (WDT)** as a periodic interrupt source instead of a hardware timer.

This demonstrates how to build lightweight cooperative scheduling without Timer0/Timer1.

---

## Features

* WDT interrupt mode (no reset)
* ~16ms periodic tick
* Simple software scheduler
* No blocking delays
* Timer1 free for other use

---

## Behavior

* WDT fires every ~16 ms
* Software counter accumulates ticks
* LED toggles every ~1 second

---

## Why Use Watchdog as Scheduler?

Advantages:

* Extremely low overhead
* Works in sleep modes
* Frees hardware timers
* Ideal for battery-powered systems

Trade-offs:

* Limited timing precision
* Limited interval options
* Coarser granularity than hardware timers

---

## WDT Timing Options (ATtiny85)

| Prescaler | Interval |
| --------- | -------- |
| 16 ms     | Fastest  |
| 32 ms     |          |
| 64 ms     |          |
| 125 ms    |          |
| 250 ms    |          |
| 500 ms    |          |
| 1 s       |          |
| 2 s       |          |
| 4 s       |          |
| 8 s       | Slowest  |

You can change interval by setting `WDP` bits in `WDTCR`.

---

## What This Teaches

* Watchdog interrupt mode
* Cooperative scheduling pattern
* Time slicing without delay()
* Embedded reliability patterns
