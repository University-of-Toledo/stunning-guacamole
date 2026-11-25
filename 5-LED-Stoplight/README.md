# 5-LED Stoplight (Bitwise Operators & GPIO)

## Overview
Controls a 5-LED traffic light (`RED`, `YELLOW`, `GREEN`, `Y_ARROW`, `G_ARROW`) using C bitwise operators (`|`, `&`, `~`, `^`, `<<`, `>>`) on a single `unsigned int signal` variable and memory-mapped GPIO registers (`/dev/gpiomem`).

## Wiring & Bit Map

| Bit | Mask Name | LED Color | BCM GPIO | Physical Pin |
| :---: | :--- | :--- | :---: | :---: |
| 0 | `RED` (`1u << 0`) | Red | GPIO 17 | Pin 11 |
| 1 | `YELLOW` (`1u << 1`) | Yellow | GPIO 27 | Pin 13 |
| 2 | `GREEN` (`1u << 2`) | Green | GPIO 22 | Pin 15 |
| 3 | `Y_ARROW` (`1u << 3`) | Yellow Arrow | GPIO 23 | Pin 16 |
| 4 | `G_ARROW` (`1u << 4`) | Green Arrow | GPIO 24 | Pin 18 |
| — | `GND` | Shared Ground | GND | Pin 14 |

## Build & Run
```bash
make
./stoplight
```
