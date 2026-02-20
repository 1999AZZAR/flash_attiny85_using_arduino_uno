# ATtiny85 Capacitive Touch Button

Implements capacitive touch sensing using a single GPIO pin.

No external components required (optional metal pad improves sensitivity).

---

## How It Works

1. Pin is driven LOW to discharge capacitance.
2. Pin switches to input (high impedance).
3. Internal pull-up slowly charges the pin.
4. We measure how long it takes to read HIGH.

Touching the pin increases capacitance → slower charge → higher count.

---

## Hardware Connection

| ATtiny85 Pin | Component                |
| ------------ | ------------------------ |
| Pin 7 (PB2)  | Touch pad / exposed wire |
| Pin 5 (PB0)  | LED (220Ω)               |
| Pin 8        | VCC                      |
| Pin 4        | GND                      |

Touch pad can be:

* Wire
* Copper tape
* PCB pad
* Metal screw head

---

## Key Parameters

### `THRESHOLD`

Adjust depending on environment.

Typical values:

* 20–60 depending on wiring and noise

---

## Behavior

* Touch pad → LED toggles
* Simple debounce via delay

---

## What This Teaches

* Capacitance measurement via GPIO
* Charge timing method
* Environmental noise impact
* Threshold calibration

---

## Limitations

* Sensitive to noise
* Temperature dependent
* Needs tuning per setup
* Not robust for production
