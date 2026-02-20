# ATtiny85 Event Queue + Function Pointer FSM

This example demonstrates a scalable embedded firmware architecture combining:

* Buffered event queue
* Function pointer state table
* Deterministic dispatcher
* Non-blocking scheduling

---

## Architecture Overview

```
[ Interrupt Timer ]
        ↓
[ Event Producers ]
        ↓
[ Ring Buffer Queue ]
        ↓
[ Dispatcher ]
        ↓
[ State Handler (function pointer) ]
```

---

## Why This Is Important

Compared to simple switch-based FSM:

* No giant nested logic
* Clean separation of states
* Event buffering (no event loss under normal load)
* Easily extendable
* Testable structure

---

## Key Components

### Event Queue

* Circular buffer
* Fixed size
* Non-blocking
* Safe for small MCU

### State Table

```c
static const state_handler_t state_table[];
```

Each state is just a function.

No `switch(current_state)` required in main loop.

---

## What This Teaches

* Message-driven firmware
* Decoupled architecture
* Scalable event systems
* Production-grade structure
