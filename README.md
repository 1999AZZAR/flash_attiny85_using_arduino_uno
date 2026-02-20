# ATtiny85 Production Examples

This repository contains production-ready, security-focused C/C++ examples for the ATtiny85 microcontroller. The code emphasizes direct register manipulation for optimal performance, minimal binary size, and deterministic hardware control.

## Project Philosophy

*   **Security-First**: All GPIO pins are initialized to known states (`PORTB = 0`) to prevent floating inputs or undefined behavior.
*   **Performance**: Direct register access (`DDRB`, `PORTB`, `PINB`, `TCCR0A`) replaces abstract Arduino functions (`digitalWrite`, `analogWrite`) to eliminate overhead.
*   **Determinism**: Hardware peripherals (Timer0 PWM) are used for time-critical tasks to free the CPU for other operations.

## Repository Structure

```
.
├── GUIDE.md                # Comprehensive ISP Hardware & Setup Guide
├── README.md               # Project Documentation
└── examples/
    ├── blink/              # Minimalist GPIO Toggling
    │   ├── blink.ino
    │   └── Makefile
    ├── sos/                # Precise Timing & Logic Control
    │   ├── sos.ino
    │   └── Makefile
    ├── fade/               # Hardware PWM (CPU-Independent)
    │   ├── fade.ino
    │   └── Makefile
    ├── candle/             # Pseudo-Random PWM Flicker
    │   ├── candle.ino
    │   └── Makefile
    ├── button/             # Debounced Input Handling
    │   ├── button.ino
    │   └── Makefile
    └── multimode/          # Sophisticated State Machine
        ├── multimode.ino
        └── Makefile
```

## Getting Started

### 1. Hardware Setup
Follow the [ISP Setup Guide](GUIDE.md) to configure your Arduino Uno as a programmer.
*   **Programmer**: Arduino Uno
*   **Target**: ATtiny85
*   **Clock**: 8MHz Internal

### 2. Software Dependencies
Ensure your Linux environment has the necessary AVR toolchain:

```bash
sudo apt update
sudo apt install avrdude gcc-avr avr-libc make
```

### 3. Building and Flashing
Each example includes a `Makefile` for automated build and deployment.

**Workflow:**

1.  Navigate to the desired example directory:
    ```bash
    cd examples/fade
    ```

2.  (First Time Only) Set the fuses for 8MHz internal clock:
    ```bash
    make fuses
    ```

3.  Compile and Flash:
    ```bash
    make flash
    ```

## Example Details

### Blink (`examples/blink`)
Demonstrates the most efficient way to toggle an I/O pin.
*   **Technique**: Atomic writes to `PINB` register.
*   **Benefit**: Toggles a pin in 1 clock cycle vs ~50 cycles for `digitalWrite`.

### SOS (`examples/sos`)
Implements standard Morse code signaling with precise timing.
*   **Technique**: Modular function design with strict timing constants.
*   **Benefit**: Shows how to structure logic for readability without sacrificing performance.

### Fade (`examples/fade`)
Demonstrates "breathing" LED effect using hardware Pulse Width Modulation (PWM).
*   **Technique**: Configures Timer0 for Fast PWM mode.
*   **Benefit**: The LED fading happens entirely in hardware. The CPU only updates the duty cycle occasionally, leaving it 99% free for other tasks.

### Candle (`examples/candle`)
Simulates a flickering candle flame using a pseudo-random number generator.
*   **Technique**: Implements a Linear Feedback Shift Register (LFSR) algorithm and maps the output to PWM duty cycle.
*   **Benefit**: Achieves a realistic organic effect using minimal memory and CPU cycles compared to standard `rand()` functions.

### Button (`examples/button`)
Demonstrates robust digital input handling with debounce logic.
*   **Technique**: Uses internal pull-up resistors on PB3 and software debounce to reliably toggle an LED on PB0.
*   **Benefit**: Eliminates the need for external resistors and handles noisy switch contacts purely in software.

### Multimode (`examples/multimode`)
A sophisticated multi-tasking controller that cycles through Off, On, Blink, and Breathe modes.
*   **Technique**: Combines **Timer0** (Hardware PWM) for LED control with **Timer1** (Interrupt) for system timing.
*   **Benefit**: The code is completely **non-blocking**. The button remains responsive at all times because the blinking and fading logic does not use `delay()`.

## Technical Notes

*   **Portability**: The code is written in standard C (avr-libc) but uses `.ino` extensions for compatibility with the Arduino IDE if preferred.
*   **Safety**: Ensure the 10µF capacitor is present on the Arduino Uno's RESET pin to prevent auto-reset during upload.
