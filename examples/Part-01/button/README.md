# ATtiny85 Button Example

A digital input example demonstrating robust button handling with internal pull-up resistors and software debounce.

## Description
This program toggles an LED on **PB0** each time a button on **PB3** is pressed. It demonstrates how to read inputs without external resistors and how to filter out the mechanical "noise" (bouncing) inherent in physical switches.

## Hardware Connection

| ATtiny85 Pin | Component | Connection |
|---|---|---|
| **Pin 5 (PB0)** | LED | Anode to Pin 5, Cathode to GND (via 220Ω Resistor) |
| **Pin 2 (PB3)** | Push Button | One leg to Pin 2, other leg to GND |
| **Pin 8 (VCC)** | Power | 2.7V - 5.5V |
| **Pin 4 (GND)** | Ground | Common Ground |

**Important**: No external resistor is needed for the button. The code enables the ATtiny85's internal 20kΩ pull-up resistor.

## Technical Highlights

### 1. Internal Pull-up Resistors
*   **`DDRB &= ~(1 << PB3)`**: Sets PB3 as Input.
*   **`PORTB |= (1 << PB3)`**: Writing `1` to an Input Pin enables the internal pull-up resistor. This holds the pin HIGH (3.3V/5V) when the button is open.

### 2. Logic Inversion
Because of the pull-up, the logic is inverted:
*   **Released**: Pin reads HIGH (1)
*   **Pressed**: Pin reads LOW (0) (connected to GND)

### 3. Software Debounce
Mechanical buttons don't close perfectly; they "bounce" rapidly between open and closed for a few milliseconds.
*   The code detects a press, waits 50ms, and checks again. If the button is still pressed, it registers as a valid click. This prevents one press from triggering multiple toggles.

## Build and Flash

```bash
# Compile and Upload
make flash
```
