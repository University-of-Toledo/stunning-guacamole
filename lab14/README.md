
# Lab 14 - Button with Debounce

Author: Ozzy Finnegan
Course: EET 3150 - C Programming

## Description
This program reads a momentary push-button connected to a
Raspberry Pi 5 and controls a red LED using software debouncing.

The LED turns on when the button is pressed and turns off
when the button is released.

## Hardware

- Raspberry Pi 5
- Momentary push-button
- Red LED
- 330 ohm resistor
- Breadboard and jumper wires

## GPIO Connections

- GPIO17 (physical pin 11): LED output
- GPIO23 (physical pin 16): Button input
- GND (physical pin 14): Shared ground

The button uses an internal pull-up resistor and is active-low.

## Debounce Algorithm

The program uses time-based software debouncing.

The button is sampled every 5 milliseconds. When the raw
button reading changes, the program records the time using
CLOCK_MONOTONIC.

The new state must remain stable for at least 30 milliseconds
before the LED is updated.

A 30 ms threshold was chosen to filter mechanical contact
bounce while keeping the button response quick.

CLOCK_MONOTONIC is used because it is not affected by
changes to the system's wall clock.

## Building

Run:

    make

## Running

Run:

    ./button_debounce

Press the button to turn on the LED.
Release the button to turn it off.
Press Ctrl+C to exit and turn off the LED.

## Cleaning

Run:

    make clean

## Testing

The program was tested using a Raspberry Pi 5 and libgpiod v2.

The button correctly controlled the LED when pressed and
released. The program also turned the LED off when exited
using Ctrl+C.
