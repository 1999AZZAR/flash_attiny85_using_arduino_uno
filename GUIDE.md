# ATtiny85 In-System Programming (ISP) Guide

This document outlines the procedure for using an Arduino Uno as an ISP programmer to flash an ATtiny85 microcontroller, including hardware pinouts, environment configuration, and optimized test code.

## 1. Hardware Requirements

### Complete Development BOM (Bill of Materials)

To build the full development circuit capable of running all examples in this repository, you will need:

| Component | Quantity | Purpose | Placement |
|---|---|---|---|
| **ATtiny85** | 1 | Microcontroller | Breadboard center |
| **Arduino Uno** | 1 | ISP Programmer | Connected via jumper wires |
| **Capacitor (10µF - 100µF)** | 2 | 1x Programmer Stability<br>1x Target Power Stability | **Cap 1:** Uno `RESET` to `GND`<br>**Cap 2:** ATtiny `VCC` to `GND` |
| **Push Button** | 2 | 1x User Input<br>1x System Reset | **Btn 1:** `PB3` (Pin 2) to `GND`<br>**Btn 2:** `RESET` (Pin 1) to `GND` |
| **LED** | 1 | Output Indicator | `PB0` (Pin 5) to `GND` |
| **Resistor (220Ω)** | 1 | Current Limiting | In series with LED |

### Capacitor Placement Detail

1.  **Programmer Anti-Reset (Crucial):**
    *   Place a **10µF - 100µF Capacitor** between Arduino Uno `RESET` and `GND`.
    *   **Polarity:** Positive (+) to `RESET`, Negative (-) to `GND`.
    *   **Why:** Prevents the Uno from resetting itself when avrdude tries to talk to it.

2.  **Target Power Smoothing:**
    *   Place a **100µF Capacitor** on the breadboard power rails (or close to ATtiny `VCC`/`GND`).
    *   **Why:** Smooths out voltage dips when the LED flashes or the MCU wakes from sleep.

### Complete Pinout & Connections

| Physical Pin | Function (Port) | ISP Connection (Uno) | Development Connection |
| :---: | :--- | :--- | :--- |
| **1** | **RESET** / PB5 | Digital 10 | **Button 2** (Reset) → GND |
| **2** | PB3 / ADC3 | *Unconnected* | **Button 1** (Input) → GND |
| **3** | PB4 / ADC2 | *Unconnected* | *(Unused / Analog In)* |
| **4** | **GND** | GND | **GND Rail** / Cap (-) |
| **5** | PB0 / MOSI | Digital 11 | **LED** (Anode) |
| **6** | PB1 / MISO | Digital 12 | **Serial TX** (Optional) |
| **7** | PB2 / SCK | Digital 13 | *(Unused)* |
| **8** | **VCC** | 5V | **VCC Rail** / Cap (+) |

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

*   **Signature 0x000000:** Check Pin 11 (MOSI) and Pin 13 (SCK). Ensure the Reset capacitor on the Uno is correctly seated and polarized.
*   **Verification Mismatch:** Indicates power instability. Ensure the **Target Power Capacitor** is installed.
*   **Device Not Found:** Ensure your user is in the `dialout` group: `sudo usermod -a -G dialout $USER`.
