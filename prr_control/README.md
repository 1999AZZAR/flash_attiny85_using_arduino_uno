# ATtiny85 Power Reduction Register (PRR) Mastery

Part 3.2 of the Bare Metal ATtiny85 Series.

## Overview
Even in active mode, hardware modules like **Timers**, **ADC**, and **USI** consume significant current. This example demonstrates how to completely shutdown these modules using the **Power Reduction Register (PRR)**. 

## Key Modules Killed:
- **Timer0/Timer1:** PWM and Timing systems.
- **USI:** SPI, I2C, and UART hardware support.
- **ADC:** Analog-to-Digital conversion subsystem.

## Current Savings
By combining PRR mastery with Analog Comparator shutdown, active current can be dropped significantly even before entering sleep modes.

## Build and Flash
```bash
make flash
```
