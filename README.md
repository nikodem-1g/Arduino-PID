# PID Position Controller for a DC Gearmotor

Closed-loop angular position control of a 298:1 micro DC gearmotor, using an Arduino Nano ESP32, an L298N H-bridge and a Pololu magnetic encoder. Built for EE12001 Design Skills, University of Bath, 2026.

## Reflections

Tuning by trial and error got the hardware, firmware and measurement chain working end to end, and gave me a direct feel for what each parameter does. It also exposed the limits of the approach: I couldn't explain the overshoot or the dead-zone trade-off from the data alone. After reading further into control theory, I plan to return to the project with the knowledge I gained from the analysis and model these behaviours properly rather than tune around them.


## System overview

Every 10 ms the Arduino Nano ESP32 takes a target position (from a potentiometer or a predefined function), reads the shaft position from the encoder and computes a PID command. The command drives the motor through the L298N, and the encoder closes the loop.

**Hardware**

| Part | Details |
|---|---|
| Controller | Arduino Nano ESP32 |
| Motor driver | L298N H-bridge module |
| Motor | Micro DC gearmotor, 298:1 |
| Encoder | Pololu magnetic encoder (item #4761) |
| Input | 10 kΩ trimmer potentiometer |

The encoder and potentiometer run from 3.3 V. The motor runs from the 5 V USB rail through the L298N. All grounds are common. The schematic (KiCad) is in [`hardware/`](hardware/schematic).

**Firmware**

The control loop runs non-blocking at 10 ms, using the measured interval rather than the nominal one in all calculations. Position feedback comes from [ESP32Encoder](https://github.com/madhephaestus/ESP32Encoder) with 4× quadrature decoding in the hardware pulse counter, giving 3576 counts per output revolution (12 counts per motor revolution × 298), or 0.10° resolution. The position error is wrapped to ±180° so the shaft always takes the shorter path, and a parallel-form PID controller with a clamped integral term converts it into a command in PWM units. That command is conditioned with a minimum PWM to overcome stiction and a ±1.0° dead zone that zeroes the output near the target. Finally, the sign of the command determines which L298N input receives the PWM signal, setting the motor direction.


## Results 

3.3% overshoot, within the ≤ 5% target. A steady-state error of 0.7° and settling to ±0.7° in 0.9 s missed the ±0.5° targets. The steady-state error is set by a ±1.0° dead zone (see [Analysis](#analysis)).

| Metric | Target | Measured | Met? |
|---|---|---|---|
| Overshoot | ≤ 5% of step | 3.3% (6.0°) | Yes |
| Settling time | ≤ 1 s to ±0.5° | 0.9 s to ±0.7° | No |
| Steady-state error | ≤ 0.5° | 0.7° | No |

The response settled to ±0.7° within 0.9 s but never entered the ±0.5° band. Results are from a single 180° step. The full report will be pushed after academic submission.

## Analysis

Inside the ±1.0° dead zone the output is zero, so the shaft rests wherever it stops, and because the dead zone is wider than the ±0.5° target, it sets the error floor. Overshoot is hypothesised to come mainly from integral windup, since the output is saturated for most of the step: proportional action alone saturates the output until the error falls to 3.4°, but with the accumulated integral term it desaturates only within 1.3° of the target, too late to brake the motor before the target is crossed. This is inferred from the trace and has not been measured. The maximum slew rate of about 240°/s (output saturated) is set by the motor supply rather than the gains. Finally, a 180° step lies on the error-wrapping boundary, so the direction of rotation depends on where the shaft rested before the step.

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
