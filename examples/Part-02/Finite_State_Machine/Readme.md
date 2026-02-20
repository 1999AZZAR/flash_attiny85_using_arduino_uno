# ATtiny85 Finite State Machine Framework

Reusable FSM structure for clean embedded architecture.

Separates:

* State definition
* Entry logic
* Update logic
* Transition control

---

## States

| State | Behavior                 |
| ----- | ------------------------ |
| IDLE  | LED OFF                  |
| ON    | LED ON                   |
| BLINK | LED toggles every 500 ms |

Button cycles through states.

---

## Architecture

Core FSM functions:

```
state_enter(state)
state_update()
state_transition(next)
```

Design pattern:

* `state_enter()` → runs once on transition
* `state_update()` → runs continuously
* `state_transition()` → controls switching

---

## Why This Matters

Without FSM:

* Logic becomes tangled
* Features become hard to expand
* Bugs multiply

With FSM:

* Deterministic behavior
* Scalable design
* Clear separation of concerns

---

## What This Teaches

* State-driven architecture
* Transition guarding
* Entry vs runtime logic separation
* Scalable firmware design
