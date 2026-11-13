
# Lab 13: Traffic Light

Author: Ozzy Finnegan
Course: EET 3150 - C Programming

## Description

This program controls three LEDs using the Raspberry Pi 5
and the libgpiod library.

The traffic light cycles through three states:
- Red: 5 seconds
- Green: 5 seconds
- Yellow: 2 seconds

The program uses a struct and an array to store each
traffic light state and its duration.

## Hardware

- Raspberry Pi 5
- Breadboard
- 3 LEDs (red, yellow, green)
- 3 resistors (330 ohms each)
- Jumper wires

## Wiring

Red LED: GPIO17, physical pin 11
Yellow LED: GPIO27, physical pin 13
Green LED: GPIO22, physical pin 15

Each GPIO connects through a 330-ohm resistor to
the LED anode (+).

All LED cathodes (-) connect to GND, physical pin 6.

## Software

Operating System: Debian 13 (Trixie)
GPIO Library: libgpiod 2.2.1
GPIO Controller: /dev/gpiochip0

## Build and Run

Compile:
    make

Run:
    ./traffic_light

Clean:
    make clean

## Program Operation

The program requests three GPIO lines as outputs,
initially OFF.

It cycles through the traffic light states using
an array of structs.

Only one LED is activated at a time.

Press Ctrl+C to stop the program. The program turns
all LEDs off and releases the GPIO resources.

## Testing

The program compiled successfully using:
-Wall -Wextra -Werror

The traffic light sequence was tested successfully
on the Raspberry Pi 5.
