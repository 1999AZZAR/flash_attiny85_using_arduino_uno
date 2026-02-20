# ATtiny85 ADC + Moving Average Filter

Reads analog input and controls LED brightness using filtered signal.

Demonstrates noise reduction and analog processing.

---

## Features

* 10-bit ADC sampling
* 16-sample moving average
* Hardware PWM output
* Analog → brightness mapping
* Simple ring buffer filter

---

## Hardware Connection

| ATtiny85 Pin       | Component           |
| ------------------ | ------------------- |
| Pin 5 (PB0)        | LED (220Ω)          |
| Pin 3 (PB4 / ADC2) | Potentiometer wiper |
| Pin 8              | VCC                 |
| Pin 4              | GND                 |

Potentiometer wiring:

* One side → VCC
* Other side → GND
* Middle → PB4

---

## Why Moving Average?

Raw ADC is noisy.

Moving average:

* Smooths fluctuations
* Reduces flicker
* Low-cost computation
* No floating point

---

## Filter Details

* 16 samples
* Uses rolling sum (efficient)
* O(1) update time

---

## What This Teaches

* ADC configuration
* Reference selection
* Fixed-point scaling
* Embedded filtering strategy
* Efficient circular buffer usage
