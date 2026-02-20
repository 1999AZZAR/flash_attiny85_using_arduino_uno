# ATtiny85 Candle Example

A simulation of a flickering candle flame using pseudo-random number generation and hardware PWM.

## Description
This program mimics the organic flicker of a candle flame on an LED connected to **PB0**. It combines hardware PWM for brightness control with a software Linear Feedback Shift Register (LFSR) to generate random flicker patterns.

## Hardware Connection

| ATtiny85 Pin | Component |
|---|---|
| **Pin 5 (PB0)** | LED Anode (+) (via 220Ω Resistor) |
| **Pin 4 (GND)** | LED Cathode (-) |
| **Pin 8 (VCC)** | 2.7V - 5.5V Power Source |

*Note: A yellow or warm-white LED provides the best effect.*

## Technical Highlights

### 1. Linear Feedback Shift Register (LFSR)
Standard `rand()` functions are heavy on memory and cycles. An LFSR is a shift register whose input bit is a linear function of its previous state.
*   **Performance**: Extremely fast generation of pseudo-random numbers using bitwise XOR and shifts.
*   **Memory**: Uses only a few bytes of flash and a single 16-bit variable.

### 2. Random Flicker Logic
The brightness `OCR0A` is updated to a random value (constrained between 50-255 to ensure the "flame" never goes out completely), and the delay between updates is also randomized for a natural feel.

## Build and Flash

```bash
# Compile and Upload
make flash
```
