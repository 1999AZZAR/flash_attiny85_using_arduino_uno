# ATtiny85 Battery Monitor (No External Parts)

Part 3.5 of the Bare Metal ATtiny85 Series.

## Overview
Traditional battery monitoring uses a **Voltage Divider** (two resistors). This is bad for ultra-low power devices because current constantly leaks through the resistors.

This example uses a "reverse" logic to measure the supply voltage (**VCC**) without any external components:
1. Set the ADC input to the **Internal 1.1V Bandgap**.
2. Set the ADC reference to **VCC**.
3. Calculate: **`VCC = (1.1 * 1024) / ADC_Reading`**.

## Key Concepts
- **`ADMUX` (ADC Multiplexer Selection Register):** Setting `MUX3` and `MUX2` connects the 1.1V reference to the ADC input.
- **Power Efficiency:** Since no external resistors are used, there is **zero leakage current** when the ADC is disabled.

## Build and Flash
```bash
make flash
```
