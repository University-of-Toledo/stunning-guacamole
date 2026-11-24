
# Lab 16 - Morse Code Blinker

## Description
This program converts a text message into Morse code and
transmits it using an LED.

## Hardware
- Raspberry Pi 5
- Red LED
- 330-ohm resistor
- GPIO17 (physical pin 11)
- GND (physical pin 6)

## Build
Compile the program using:

    make

## Usage
Run the program with:

    ./morse "MESSAGE" [unit_ms]

The optional unit_ms argument controls the Morse code speed.
The default is 200 milliseconds.

## Example
    ./morse "SOS"

Expected terminal output:

    S: ...
    O: ---
    S: ...

## Timing
- Dot: 1 unit
- Dash: 3 units
- Between symbols: 1 unit
- Between letters: 3 units
- Between words: 7 units

## Cleanup
Remove the compiled program using:

    make clean
