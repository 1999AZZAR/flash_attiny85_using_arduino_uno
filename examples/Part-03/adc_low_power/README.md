# ATtiny85 ADC Noise Reduction Mode

Part 3.3 of the Bare Metal ATtiny85 Series.

## Overview
Taking accurate ADC readings is difficult in noisy digital environments. This example demonstrates how to use **ADC Noise Reduction Mode** to stop the CPU during conversion, providing a cleaner power rail and more stable readings. 

## Technical Details:
- **`SLEEP_MODE_ADC`:** Disables I/O and CPU clocking to eliminate switching noise.
- **`ADC_vect`:** The interrupt vector that triggers once the conversion is complete, automatically waking the CPU.
- **Current Savings:** Significantly less current than a continuous polling ADC loop.

## Build and Flash
```bash
make flash
```
