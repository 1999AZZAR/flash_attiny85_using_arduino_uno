# Watchdog Reset & Safety

This example demonstrates how to use the Watchdog Timer (WDT) as a fail-safe mechanism to prevent the system from hanging indefinitely.

## Concept
In embedded systems, external factors (like EMI or software bugs) can cause the CPU to get stuck in an infinite loop or a "hang" state. The Watchdog Timer is a dedicated piece of hardware that will automatically reset the microcontroller if the software fails to "kick" (reset) the timer within a certain period.

## Hardware
- **LED**: PB0 (Pin 5)
- **Button**: PB3 (Pin 2)

## How to Test
1. **Power-on**: The LED will flash slowly 3 times.
2. **Normal Operation**: The LED will blink steadily (0.5s on, 0.5s off).
3. **Trigger Hang**: Press and hold the button. The code will enter a `while(1)` loop that does *not* reset the watchdog.
4. **The Reset**: After ~2 seconds of the button being held (the WDTO_2S timeout), the chip will reset.
5. **Observation**: Upon reset, the code detects that the cause was a Watchdog Reset and flashes the LED rapidly 10 times as an error signal.

## Key Features
- **MCUSR Check**: Checking the `MCUSR` register to determine *why* the chip started (Power-on vs. Watchdog).
- **WDT Recovery**: Using `wdt_disable()` early in `main()` is critical for modern AVRs to prevent a "reset loop" if the watchdog was previously triggered.
