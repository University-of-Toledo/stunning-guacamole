# CSET 3150 — Lab 12: Blink

## Library & Environment
- **OS:** Raspberry Pi OS (Debian Trixie, arm64)
- **libgpiod Version:** `2.2.1` (`libgpiod` v2 API)
- **GPIO Device:** `/dev/gpiochip0` (Line Offset `17` / `GPIO17`)

## Hardware Wiring
All wiring was completed with the Raspberry Pi powered off:
- **GPIO17 (Physical Pin 11)** -> **330 Ohm Resistor** -> **LED Anode (longer lead)**
- **LED Cathode (shorter lead)** -> **GND (Physical Pin 6)**

## Build & Run
```bash
make
./blink
```

## Verification
- Compiled cleanly with `gcc -std=c11 -Wall -Wextra -Werror`.
- Verified zero memory leaks and zero invalid memory accesses via `valgrind` (see `valgrind.log`).
