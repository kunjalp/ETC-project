# APPS Plausibility Checker

Firmware for an electric race car's **Accelerator Pedal Position Sensor (APPS) plausibility check**, built for Formula Slug (UC Santa Cruz's FSAE electric vehicle team). Runs on an STM32 target under Mbed OS.

Race cars use two independent pedal sensors so a single sensor failure can't cause unintended acceleration. This firmware reads both, cross-checks them against each other and against their calibrated voltage ranges, and cuts power if they disagree for too long. That's the kind of redundant safety check EV rulesets require before a car is allowed on track.

## How it works

- **Dual-channel ADC sampling**: reads independent analog voltages from two pedal sensors on `PA_4` and `PA_3`
- **Signal normalization**: converts each raw voltage (against a 3.3V ADC reference) into a 0–100% pedal travel percentage
- **Range validation**: flags a sensor as "clamped" if its voltage falls outside its calibrated range (0.25V–2.25V for sensor 0, 0.30V–2.70V for sensor 1)
- **Deviation monitoring**: continuously compares the two sensors; if they diverge by more than 10%, the reading is flagged implausible
- **Grace period + cutoff**: tolerates transient noise for 100ms, but cuts power if the implausibility persists past that window

## Stack

C++ · Mbed OS · STM32 · CMake

## Build

This is an Mbed OS CMake project targeting an STM32 board.

```bash
mbed-tools deploy        # pulls in the mbed-os library
mbed-tools compile -m <YOUR_TARGET> -t GCC_ARM
```

Flash the resulting binary to the board; APPS readings and any implausibility events print over serial.
