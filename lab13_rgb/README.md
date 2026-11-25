# Lab 13 - Multi-Color LED (RGB)

Author: Ozzy Finnegan

## System Information

- Raspberry Pi 5
- libgpiod API: v2.2.1
- Detected GPIO chip: /dev/gpiochip0
- Chip label: pinctrl-rp1

## Wiring

- GPIO17 (Pin 11): Red through 330 ohm resistor
- GPIO27 (Pin 13): Green through 330 ohm resistor
- GPIO22 (Pin 15): Blue through 330 ohm resistor
- GND (Pin 9): Common cathode

## Modifications to Original Code

- Updated the GPIO functions from libgpiod v1.6 to v2.2.1 for compatibility with the installed library.
- Replaced the original bulk GPIO functions with the v2 line request functions.
- Replaced hard-coded GPIO chip selection with automatic discovery using the chip label.
- Added the `-p` command-line option to allow manual GPIO chip selection.
- Added error checking for `sigemptyset()` to meet the lab requirements.

The replaced portions of the professor's original code were commented out and retained for reference. 
The modifications are labeled and explained within the source code.

## Testing

All eight color states were tested successfully. Automatic chip discovery, manual chip selection, input validation,
and clean shutdown were verified.

The program compiled with `-Wall -Wextra -Werror`. Valgrind reported zero memory leaks and zero errors.
