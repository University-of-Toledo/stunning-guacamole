# Lab 13: Traffic Light

## Overview
This program implements a data-driven traffic light controller on the Raspberry Pi using `libgpiod` v2. Three LEDs (Red on GPIO17, Yellow on GPIO27, Green on GPIO22) are requested together in a single bulk line request and driven by an array of `TrafficState` structs rather than hardcoded `if/else` chains.

## State Table & Timing Choices

| Order | State | Red (GPIO17) | Yellow (GPIO27) | Green (GPIO22) | Duration |
| :--- | :--- | :---: | :---: | :---: | :---: |
| 1 | **RED (Stop)** | `1` (ON) | `0` (OFF) | `0` (OFF) | 5000 ms (5.0 s) |
| 2 | **GREEN (Go)** | `0` (OFF) | `0` (OFF) | `1` (ON) | 5000 ms (5.0 s) |
| 3 | **YELLOW (Caution)** | `0` (OFF) | `1` (ON) | `0` (OFF) | 2000 ms (2.0 s) |

### Design & Safety Rationale
- **Timing:** A 5-second Red and 5-second Green phase provide clear visual confirmation of steady-state operation during hardware testing, while a shorter 2-second Yellow phase reflects real-world caution transitions without making full cycle tests overly slow.
- **Safety Constraint:** Because each table entry explicitly defines all three GPIO output values simultaneously (`gpiod_line_request_set_values`), mutually exclusive states are enforced in data—ensuring Red and Green are never active at the same time.
- **Clean Shutdown:** Pressing `Ctrl+C` (`SIGINT`) breaks the main loop, sets all three lines back to `0` (OFF), releases the line request, and closes the GPIO chip cleanly.
