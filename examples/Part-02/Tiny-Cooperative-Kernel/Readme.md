# ATtiny85 Tiny Cooperative Kernel

Implements a minimal cooperative task scheduler.

This is not an RTOS.
It is a deterministic time-sliced execution engine.

---

## Kernel Characteristics

* Static task table
* Function pointer execution
* Fixed interval scheduling
* No dynamic memory
* No stack switching
* No preemption

---

## Task Structure

```c
typedef struct
{
    task_func_t func;
    uint32_t interval;
    uint32_t last_run;
    uint8_t enabled;
} task_t;
```

Each task:

* Executes periodically
* Must return quickly
* Must never block

---

## Example Tasks

| Task        | Interval |
| ----------- | -------- |
| LED Blink   | 1000 ms  |
| Button Scan | 10 ms    |

---

## Why Cooperative?

Advantages:

* Extremely lightweight
* Predictable timing
* Tiny memory footprint
* Perfect for 8-bit MCUs

Trade-offs:

* Long tasks block others
* No priority
* No real-time guarantees

---

## What This Teaches

* Kernel abstraction
* Task scheduling
* Function pointer dispatch
* Embedded architectural thinking

---

## Next Advanced Direction

If you want real depth:

1. Add task priority
2. Add event queue per task
3. Add sleep between idle cycles
4. Convert into tickless kernel
5. Add software timers separate from tasks
