# Makefile for ATtiny85 Fade Example

MCU = attiny85
F_CPU = 8000000L
PROGRAMMER = arduino
PORT = /dev/ttyACM0
BAUD = 19200
TARGET = fade
SRC = $(TARGET).ino

CC = avr-gcc
OBJCOPY = avr-objcopy
AVRDUDE = avrdude
CFLAGS = -mmcu=$(MCU) -Os -DF_CPU=$(F_CPU) -Wall -x c

all: $(TARGET).hex

$(TARGET).elf: $(SRC)
	$(CC) $(CFLAGS) -o $@ $<

$(TARGET).hex: $(TARGET).elf
	$(OBJCOPY) -O ihex -R .eeprom $< $@

flash: $(TARGET).hex
	$(AVRDUDE) -c $(PROGRAMMER) -p $(MCU) -P $(PORT) -b $(BAUD) -U flash:w:$<:i

fuses:
	$(AVRDUDE) -c $(PROGRAMMER) -p $(MCU) -P $(PORT) -b $(BAUD) -U lfuse:w:0xe2:m

clean:
	rm -f *.elf *.hex

.PHONY: all flash fuses clean
