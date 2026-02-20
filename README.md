# ATtiny85 Examples

This repository contains C/C++ examples for the ATtiny85 microcontroller. The code emphasizes direct register manipulation for optimal performance, minimal binary size, and deterministic hardware control.

## Project Philosophy

*   **Security-First**: All GPIO pins are initialized to known states (`PORTB = 0`) to prevent floating inputs or undefined behavior.
*   **Performance**: Direct register access (`DDRB`, `PORTB`, `PINB`, `TCCR0A`) replaces abstract Arduino functions (`digitalWrite`, `analogWrite`) to eliminate overhead.
*   **Determinism**: Hardware peripherals (Timer0/Timer1) and cooperative scheduling are used for time-critical tasks.

## Repository Structure

The examples are organized into two parts:

*   **Part 1**: Essential techniques for GPIO, Timing, and PWM.
*   **Part 2**: Advanced system architecture, kernels, FSMs, and signal processing.

## Hardware Quick Reference

Common pinout used across most examples:

| Component | ATtiny85 Pin | Physical Pin | Notes |
|---|---|---|---|
| **LED** | PB0 | Pin 5 | Connected via 220Ω resistor to GND. |
| **Button** | PB3 | Pin 2 | Connected to GND (Internal Pull-up used). |
| **Serial TX** | PB1 | Pin 6 | For UART examples (Connect to USB-TTL RX). |
| **Power** | VCC | Pin 8 | 2.7V - 5.5V |
| **Ground** | GND | Pin 4 | Common Ground |

## Getting Started

### 1. Hardware Setup
Follow the [ISP Setup Guide](GUIDE.md) to configure your Arduino Uno as a programmer.
*   **Programmer**: Arduino Uno
*   **Target**: ATtiny85
*   **Clock**: 8MHz Internal (16MHz PLL for Motor Control)

### 2. Software Dependencies
Ensure your Linux environment has the necessary AVR toolchain:

```bash
sudo apt update
sudo apt install avrdude gcc-avr avr-libc make
```

### 3. Building and Flashing
Each example includes a `Makefile`. Navigate to the directory and run:

```bash
make flash
```

## Part 1: Essentials (`examples/Part-01`)

### Basic Output
*   **[Blink](examples/Part-01/blink/)**: The "Hello World" of microcontrollers. Demonstrates atomic port manipulation (`PINB`) for efficient toggling.
*   **[SOS](examples/Part-01/sos/)**: Implements Morse code signaling. Demonstrates modular function design and precise timing constants.

### Advanced Output
*   **[Fade](examples/Part-01/fade/)**: Creates a smooth breathing effect. Demonstrates **Hardware PWM (Timer0)** to control LED brightness without CPU intervention.
*   **[Candle](examples/Part-01/candle/)**: Simulates an organic flame flicker. Demonstrates **LFSR** (Linear Feedback Shift Register) for efficient pseudo-random number generation.

### Input & Logic
*   **[Button](examples/Part-01/button/)**: Toggles an LED on press. Demonstrates **Input Pull-ups** and **Software Debouncing** to handle physical switches reliably.
*   **[Multimode](examples/Part-01/multimode/)**: A sophisticated non-blocking controller. Cycles through multiple modes (Off, On, Blink, Breathe). Demonstrates **Timer1 Interrupts** for system timing and multitasking.

## Part 2: Advanced System Architecture (`examples/Part-02`)

### Real-Time Control
*   **[Closed Loop Motor Control](examples/Part-02/Closed-Loop-Motor-Control/)**: A high-performance PI controller maintaining exactly **1440 RPM** via Hall Sensor feedback. Demonstrates **Adaptive Soft-Start**, **16MHz PLL Clock**, and **Interrupt-based Precision Timing**.

### System Architecture & Power
*   **[Tiny Cooperative Kernel](examples/Part-02/Tiny-Cooperative-Kernel/)**: A lightweight, non-preemptive task scheduler. Executes functions at fixed intervals without an RTOS.
*   **[Low Power Sleep Wake](examples/Part-02/Low-Power-Sleep-Wake/)**: Demonstrates Deep Sleep (Power-Down) with Pin Change Interrupt wakeup for battery-powered devices.
*   **[Watchdog Timer Scheduler](examples/Part-02/Watchdog-Timer-Scheduler/)**: Uses the Watchdog Timer as a low-power system tick for tasks that don't require high precision.

### Communication & Storage
*   **[Bit Banged UART TX](examples/Part-02/Bit-Banged-UART-TX/)**: Software-based Serial Transmit (9600 baud) for debugging without hardware UART.
*   **[EEPROM Persistent State](examples/Part-02/EEPROM_Persistent_State/)**: Saves state across power cycles using the internal EEPROM memory.

### Finite State Machines (FSM)
*   **[Finite State Machine](examples/Part-02/Finite_State_Machine/)**: Basic Moore/Mealy machine implementation for structured logic.
*   **[Event Driven FSM](examples/Part-02/Event-Driven_FSM/)**: State transitions triggered by asynchronous events rather than polling.
*   **[Event Queue Function Pointer FSM](examples/Part-02/Event-Queue_Function-Pointer-FSM/)**: Advanced architecture using function pointers and an event queue for complex, decoupled systems.

### Signal Processing
*   **[Moving Average Filter](examples/Part-02/Moving_Average_Filter/)**: smooths noisy sensor data using a windowed average algorithm.
*   **[Capacitive Touch Button](examples/Part-02/Capacitive_Touch_Button/)**: Implements touch sensing using internal ADC/GPIO physics without specialized hardware.

## Technical Notes

*   **Portability**: The code is written in standard C (avr-libc) but uses `.ino` extensions for compatibility with the Arduino IDE if preferred.
*   **Safety**: Ensure the 10µF capacitor is present on the Arduino Uno's RESET pin to prevent auto-reset during upload.
