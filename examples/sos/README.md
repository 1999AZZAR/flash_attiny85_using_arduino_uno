# ATtiny85 SOS Example

A Morse code signaling example demonstrating precise timing control and modular code structure.

## Description
This program broadcasts the international distress signal "SOS" (... --- ...) on an LED connected to **PB0**. It demonstrates how to define timing constants and structure a C program with helper functions for readability and maintenance.

## Hardware Connection

| ATtiny85 Pin | Component |
|---|---|
| **Pin 5 (PB0)** | LED Anode (+) (via 220Ω Resistor) |
| **Pin 4 (GND)** | LED Cathode (-) |
| **Pin 8 (VCC)** | 2.7V - 5.5V Power Source |

## Technical Highlights

### 1. Timing Constants
Using `#define` for timing ensures the Morse code standard is maintained easily:
*   **Dot**: 1 unit (200ms)
*   **Dash**: 3 units (600ms)
*   **Element Gap**: 1 unit
*   **Letter Gap**: 3 units
*   **Word Gap**: 7 units

### 2. Modular Design
The logic is broken down into `signal_dot()` and `signal_dash()` functions. This makes the `main()` loop clean and easy to read, resembling a high-level script:
```c
signal_dot(); signal_dot(); signal_dot(); // S
signal_dash(); signal_dash(); signal_dash(); // O
signal_dot(); signal_dot(); signal_dot(); // S
```

## Build and Flash

```bash
# Compile and Upload
make flash
```
