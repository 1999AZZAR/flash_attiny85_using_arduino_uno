# Makefile for ATtiny85 Button Gestures

MCU = attiny85
F_CPU = 8000000UL
FILENAME = Button_Gestures.ino

all: compile

compile:
	arduino-cli compile --fqbn ATTinyCore:avr:attinyx5:cpu=85,clock=8internal $(FILENAME)

upload:
	arduino-cli upload -p /dev/ttyACM0 --fqbn ATTinyCore:avr:attinyx5:cpu=85,clock=8internal $(FILENAME)

clean:
	rm -rf build
