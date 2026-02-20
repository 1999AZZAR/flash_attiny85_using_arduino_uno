
# ATtiny85 EEPROM Persistent State

Demonstrates storing runtime state in internal EEPROM so behavior survives power loss.

---

## Features

* Reads EEPROM at startup
* Toggles LED on button press
* Writes updated state to EEPROM
* Uses `eeprom_update_byte()` (safe write)

---

## Hardware Connection

| ATtiny85 Pin | Component         |
| ------------ | ----------------- |
| Pin 5 (PB0)  | LED (220Ω)        |
| Pin 2 (PB3)  | Push Button → GND |
| Pin 8        | VCC               |
| Pin 4        | GND               |

---

## Behavior

1. On boot → read EEPROM
2. LED restores previous state
3. Button toggles LED
4. New state written to EEPROM
5. Power cycle → state persists

---

## Why `eeprom_update_byte()`?

Difference:

* `eeprom_write_byte()` → always writes (wears memory)
* `eeprom_update_byte()` → writes only if value changed

EEPROM endurance ≈ 100,000 write cycles per cell.

---

## What This Teaches

* Non-volatile memory usage
* Flash wear considerations
* Persistent configuration storage
* Safe write practices
