# Button Gestures (Short, Long, Double Press)

This example demonstrates how to implement multiple functions using a single push-button on the ATtiny85.

## Hardware
- **LED**: PB0 (Pin 5)
- **Button**: PB3 (Pin 2), connected to GND.

## Functionality
1. **Single Click**: Toggles the LED state.
2. **Double Click**: Triggers a rapid double flash of the LED.
3. **Long Press (>1s)**: Turns the LED ON while the button is held, then turns it OFF when released.

## Logic
The implementation uses a simple state machine and timing logic based on a 1ms system tick (Timer1). It handles debouncing, press duration, and inter-click windows to distinguish between different gestures.
