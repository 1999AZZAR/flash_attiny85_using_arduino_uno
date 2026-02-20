# ATtiny85 Dynamic Clock Scaling & Deep Sleep

Part 3.1 of the Bare Metal ATtiny85 Series. 

## Overview
This example demonstrates how to control the ATtiny85's power consumption dynamically by changing its clock frequency and using advanced sleep modes. 

Current consumption targets:
- **8MHz Active:** ~3mA
- **125kHz Active:** ~300µA
- **Power-Down Mode:** < 1µA (with BOD disabled)

## Key Features
- **Clock Prescaler Register (`CLKPR`):** Manipulate the CPU frequency in real-time.
- **BOD Shutdown:** Disable Brown-Out Detection during sleep via `MCUCR` for maximum battery life.
- **Leakage Prevention:** Proper initialization of unused GPIOs.

## Build and Flash
```bash
make flash
```
