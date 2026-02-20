# ATtiny85 Event-Driven FSM

Implements a dispatcher-based event architecture.

All behavior is driven by events, not direct state manipulation.

---

## Architecture Layers

1. **Time Base** → generates periodic events
2. **Input Layer** → converts hardware signals into events
3. **Event Dispatcher** → routes events
4. **State Handlers** → contain state-specific behavior

---

## States

| State | Behavior                 |
| ----- | ------------------------ |
| IDLE  | LED OFF                  |
| ON    | LED ON                   |
| BLINK | LED toggles every 500 ms |

---

## Events

| Event        | Source          |
| ------------ | --------------- |
| BUTTON_PRESS | Debounced input |
| TICK_500MS   | Software timer  |

---

## Why This Architecture Is Important

Advantages:

* Decoupled logic
* Scalable
* Testable
* Easy to extend
* Clear flow of control

Adding a new feature = add event + extend state handlers.

---

## What This Teaches

* Event abstraction
* State isolation
* Dispatcher pattern
* Cooperative embedded scheduling
