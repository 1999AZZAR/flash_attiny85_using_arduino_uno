# ATtiny85 Software UART (TX Only)

Minimal bit-banged UART transmitter running at **9600 baud** on **ATtiny85 @ 8 MHz internal oscillator**.

This example demonstrates manual serial framing without hardware UART.

---

## Features

* 9600 baud
* 8N1 format
* LSB-first transmission
* No timers used
* Deterministic timing via `_delay_us()`

---

## Hardware Connection

| ATtiny85 Pin | Connect To  |
| ------------ | ----------- |
| Pin 6 (PB1)  | USB-TTL RX  |
| Pin 4 (GND)  | USB-TTL GND |

⚠️ Ensure voltage compatibility (3.3V or 5V).

---

## Serial Settings

* Baud: 9600
* Data bits: 8
* Parity: None
* Stop bits: 1

---

## How It Works

UART frame structure:

```
START (0)
8 DATA BITS (LSB first)
STOP (1)
```

Each bit duration:

```
1 / 9600 ≈ 104 µs
```

The transmitter:

1. Pulls line LOW (start)
2. Sends 8 bits
3. Returns line HIGH (stop)

Idle state is HIGH.

---

## Expected Output

```
Hello ATtiny85
Hello ATtiny85
...
```

Printed once per second.

---

## Limitations

* Blocking implementation
* Timing sensitive to clock accuracy
* No RX support
* No buffering

---

## Next Upgrade Path

* Timer-driven UART (more precise)
* Interrupt-based buffered TX
* Full duplex software UART
* Ring buffer implementation

---

Test it.
If stable, next example will remove `_delay_us()` and use timer scheduling instead.
