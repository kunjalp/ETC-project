# Accelerator Pedal Position Sensors Sensor Plausibility System

## Overview
Project implements a monitoring system for an electrical vehicle's Accelerator Pedal Position Sensors (APPS) using Mbed OS and C++.
System reads dual independent analog signals such that driver intent is captured correctly while protecting against sensor failure and signal deviation.

## Technical Features
- Dual-Channel ADC Sampling: Reads raw analog signals from pins PA_4 and PA_3
- Signal Normalization: Maps raw voltages (based on a 3.3V ADC reference) to a 0\% to 100\% pedal travel scale.
- Range Validation (Clamping): Detects if a sensor is out of its calibrated range (0.25V–2.25V for Sensor 0; 0.30V–2.70V for Sensor 1), flagging a "Clamped Voltage" error.
- Deviation Monitoring: Constantly compares both sensors; if they differ by more than 10%, an implausibility is detected.
- Safety Grace Period: Implements a 100ms timer to allow for transient noise. If the error persists beyond this window, the system enters a "Cutting Power" state to ensure vehicle safety.
