# ATtiny85 In-System Programming (ISP) Guide

This document outlines the procedure for using an Arduino Uno as an ISP programmer to flash an ATtiny85 microcontroller, including hardware pinouts, environment configuration, and optimized test code.

## 1. Hardware Requirements

### Core Components

* **Programmer:** Arduino Uno (or compatible)
* **Target:** ATtiny85 Microcontroller
* **Reset Capacitor:** 10µF - 100µF Electrolytic Capacitor (see below)
* **Status LED (Optional):** LED + 220Ω Resistor for PB0 (Pin 5)

### Capacitor Placement

**Arduino Uno (Programmer):**
*   Place the **10µF - 100µF Capacitor** between `RESET` and `GND`.
*   **Polarity:** Positive (+) leg to `RESET`, Negative (-) leg to `GND`.
*   **Purpose:** Prevents the Arduino Uno from auto-resetting during the upload process.

### Signal Routing

| ATtiny85 Physical Pin | Function | Arduino Uno Pin |
| --- | --- | --- |
| Pin 1 | RESET | Digital 10 |
| Pin 4 | GND | GND |
| Pin 5 | PB0 (MOSI) | Digital 11 |
| Pin 6 | PB1 (MISO) | Digital 12 |
| Pin 7 | PB2 (SCK) | Digital 13 |
| Pin 8 | VCC | 5V |

---

## 2. Environment Configuration

### Software Dependencies (Debian/Ubuntu)

```bash
sudo apt update
sudo apt install avrdude gcc-avr avr-libc make
```

### Arduino IDE Setup (Optional)

1. Add the Board Manager URL: `https://raw.githubusercontent.com/damellis/attiny/ide-1.6.x-package/package_damellis_attiny_index.json`
2. Install **attiny** by David A. Mellis via Boards Manager.
3. Upload `File > Examples > 11.ArduinoISP` to the Uno **before** connecting the Reset capacitor.

---

## 3. Implementation (Production-Ready)

### Register-Level Blink Code

This implementation bypasses the overhead of `digitalWrite()` for performance optimization and reduced binary size.

```c
/**
 * @file main.c
 * @brief Minimalist LED Blink for ATtiny85 @ 8MHz
 * @target PB0 (Physical Pin 5)
 */

#ifndef F_CPU
#define F_CPU 8000000UL
#endif

#include <avr/io.h>
#include <util/delay.h>

int main(void) {
    /* Set PB0 as output in Data Direction Register B */
    DDRB |= (1 << PB0);

    /* Security: Ensure all PORTB pins are initialized to a known state (Low/Hi-Z) */
    PORTB = 0;

    while (1) {
        /* Toggle PB0 using the PINB register for efficiency */
        /* Atomic write to PINB toggles the corresponding PORTB bit */
        PINB = (1 << PB0);
        _delay_ms(500);
    }

    return 0;
}
```

---

## 4. Deployment via Terminal

### Verification (Ping)

Verify the hardware signature before attempting to flash:

```bash
avrdude -c arduino -p t85 -P /dev/ttyACM0 -b 19200 -v
```

### Clock Configuration (Fuse Setting)

To set the internal oscillator to **8MHz**, run the following terminal command to set the Low Fuse:

```bash
avrdude -c arduino -p t85 -P /dev/ttyACM0 -b 19200 -U lfuse:w:0xe2:m
```

### Flashing Binary

```bash
avrdude -c arduino -p t85 -P /dev/ttyACM0 -b 19200 -U flash:w:firmware.hex:i
```

---

## 5. Troubleshooting Hardware Logic

* **Signature 0x000000:** Check Pin 11 (MOSI) and Pin 13 (SCK). Ensure the Reset capacitor on the Uno is correctly seated and polarized.
* **Verification Mismatch:** Indicates power instability or floating pins. Check all jumper wire connections.
* **Device Not Found:** Ensure your user is in the `dialout` group: `sudo usermod -a -G dialout $USER`.
