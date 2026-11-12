# Lab 12: Blink

Author: Ozzy Finnegan

## Description
This program blinks a red LED using GPIO17 on a
Raspberry Pi 5 and the libgpiod C library.

The LED turns on for one second, then off for
one second. This sequence repeats five times.

## Hardware
- Raspberry Pi 5
- Breadboard
- Red LED
- 330-ohm resistor
- Jumper wires

## Wiring
- GPIO17 (physical pin 11) to 330-ohm resistor
- Resistor to LED anode (+)
- LED cathode (-) to GND (physical pin 6)

## Software
- Debian GNU/Linux 13 (Trixie)
- libgpiod version 2.2.1
- GPIO controller: /dev/gpiochip0
- GPIO line offset: 17

## Build
make

## Run
./blink

## Memory Testing
Valgrind 3.24.0

Results:
- 11 allocations
- 11 frees
- 0 bytes in use at exit
- 0 memory errors
- No memory leaks
