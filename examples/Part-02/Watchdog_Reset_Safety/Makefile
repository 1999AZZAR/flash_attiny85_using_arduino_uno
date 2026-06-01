# Makefile for ATtiny85 Watchdog Reset Safety

MCU = attiny85
F_CPU = 8000000UL
FILENAME = Watchdog_Reset_Safety.ino

all: compile

compile:
	arduino-cli compile --fqbn ATTinyCore:avr:attinyx5:cpu=85,clock=8internal $(FILENAME)

upload:
	arduino-cli upload -p /dev/ttyACM0 --fqbn ATTinyCore:avr:attinyx5:cpu=85,clock=8internal $(FILENAME)

clean:
	rm -rf build
