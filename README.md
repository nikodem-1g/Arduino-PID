# PID Position Controller for a DC Gearmotor

Closed-loop angular position control of a 298:1 micro DC gearmotor, using an Arduino Nano ESP32, an L298N H-bridge and a Pololu magnetic encoder. Built for EE12001 Design Skills, University of Bath, 2026.

## Reflections

Tuning by trial and error got the hardware, firmware and measurement chain working end to end, and gave me a direct feel for what each parameter does. It also exposed the limits of the approach: I couldn't explain the overshoot or the dead-zone trade-off from the data alone. After reading further into control theory, I plan to return to the project with the knowledge I gained from the analysis and model these behaviours properly rather than tune around them.

## Repository contents

```
firmware/   Arduino sketch
matlab/     Analysis and plotting script
hardware/   Schematic and images
data/       Recorded step responses
docs/       Figures
```

## Tools

Arduino IDE (C++), MATLAB, KiCad, Git/GitHub.
