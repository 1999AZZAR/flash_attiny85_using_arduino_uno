# ATtiny85 Blink Example

A minimalist example demonstrating the most efficient way to toggle an I/O pin on the ATtiny85.

## Description
This program blinks an LED connected to **PB0** (Pin 5) at a 1Hz frequency (500ms ON, 500ms OFF). It bypasses standard Arduino functions (`digitalWrite`) in favor of direct register manipulation, resulting in a significantly smaller binary and faster execution.

## Hardware Connection

| ATtiny85 Pin | Component |
|---|---|
| **Pin 5 (PB0)** | LED Anode (+) (via 220Ω Resistor) |
| **Pin 4 (GND)** | LED Cathode (-) |
| **Pin 8 (VCC)** | 2.7V - 5.5V Power Source |

## Technical Highlights

### 1. Direct Register Access
Instead of the overhead of function calls, we write directly to the hardware ports:
*   **`DDRB |= (1 << PB0)`**: Sets the Data Direction Register to configure PB0 as an Output.
*   **`PORTB = 0`**: Initializes the port to a safe low state.

### 2. Atomic Toggling
*   **`PINB = (1 << PB0)`**: This is a special hardware feature of AVR microcontrollers. Writing a logic `1` to the Input Pin Address (`PINB`) toggles the corresponding bit in the Output Port (`PORTB`). This operation takes only **1 clock cycle**.

## Build and Flash

```bash
# Compile and Upload
make flash

# Set Fuses (8MHz Internal) - Run once
make fuses
```
