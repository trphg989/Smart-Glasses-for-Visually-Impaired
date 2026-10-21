
# Smart Glasses for Visually Impaired People

**Project Status: Work in Progress (WIP)**

## 1. Overview

Smart Glasses for Visually Impaired People is an Arduino-based assistive technology project designed to help visually impaired individuals become more aware of their surroundings.

The project aims to develop a lightweight, affordable wearable device that can detect nearby obstacles and monitor ultraviolet (UV) radiation, providing users with different audible alerts.

By combining sensors, a microcontroller, and a simple warning system, this project explores how electrical and electronic engineering can address everyday accessibility challenges.

**Note:** This project is currently under development. The proposed functions have not yet been fully implemented, tested, or validated in real-world conditions.

## 2. Project Objectives

The main objectives of this project are:

- Develop a low-cost wearable obstacle detection system.
- Detect nearby objects using ultrasonic sensing.
- Monitor environmental UV radiation.
- Provide different buzzer alerts depending on detected conditions.
- Design a compact and lightweight electronic system suitable for wearable applications.
- Evaluate sensor performance and improve detection reliability through experimentation.

## 3. Planned Features

### 3.1. Obstacle Detection

An HC-SR04 ultrasonic sensor is used to measure the distance between the glasses and nearby objects.

The planned warning system uses different buzzer patterns depending on the measured distance:

| Distance | Warning |
|----------|---------|
| Above 100 cm | No warning |
| 50–100 cm | Slow beeping |
| 30–50 cm | Fast beeping |
| Below 30 cm | Very fast beeping |

The distance thresholds may be adjusted during testing.

### 3.2. UV Radiation Monitoring

A GUVA-S12SD ultraviolet sensor is used to detect UV radiation.

The Arduino reads the sensor's analog output and determines whether a warning should be activated.

Planned warning patterns include:

- Normal UV conditions: No warning.
- High UV levels: Two short beeps.
- Very high UV levels: Three short beeps.

UV thresholds will require calibration before reliable UV Index-based warnings can be implemented.

### 3.3. Warning Priority

The system is designed to prioritize obstacle detection over UV warnings.

When an obstacle is detected nearby, the buzzer will prioritize the distance-based warning.

This approach aims to reduce unnecessary alerts while emphasizing immediate hazards.

## 4. Hardware Components

| Component | Quantity | Purpose |
|-----------|----------|---------|
| Arduino Nano | 1 | Main microcontroller |
| HC-SR04 Ultrasonic Sensor | 1 | Obstacle detection |
| GUVA-S12SD UV Sensor | 1 | UV radiation sensing |
| Passive Buzzer | 1 | Audible alerts |
| Jumper Wires | As needed | Electrical connections |
| 5V Power Supply | 1 | System power |
| Prototype Board | 1 | Circuit assembly |

The component list may change during development.

## 5. Proposed Circuit Connections

### HC-SR04 Ultrasonic Sensor

| Sensor Pin | Arduino Nano |
|------------|--------------|
| VCC | 5V |
| GND | GND |
| TRIG | D2 |
| ECHO | D3 |

### GUVA-S12SD UV Sensor

| Sensor Pin | Arduino Nano |
|------------|--------------|
| VCC | Compatible supply voltage |
| GND | GND |
| OUT | A0 |

The sensor module's operating voltage must be verified before connection.

### Passive Buzzer

| Buzzer Pin | Arduino Nano |
|------------|--------------|
| Positive (+) | D4 |
| Negative (-) | GND |

Direct connection is intended only for a low-current buzzer compatible with the Arduino output pin. Otherwise, a transistor driver is required.

## 6. Software

The system is programmed using the Arduino IDE.

The current source code is available in:

`SmartGlasses.ino`

The program is intended to:

1. Trigger the ultrasonic sensor.
2. Calculate the distance to nearby obstacles.
3. Read analog values from the UV sensor.
4. Compare measurements with predefined thresholds.
5. Generate different buzzer warning patterns.
6. Prioritize obstacle warnings when necessary.

The software is an initial development version and may be revised following hardware testing.

## 7. Development Progress

| Development Task | Status |
|------------------|--------|
| Project concept and objectives | Completed |
| Initial component selection | Completed |
| Preliminary circuit design | Completed |
| Initial Arduino source code | Available |
| Hardware assembly | In progress / Pending verification |
| Obstacle detection testing | Pending |
| UV sensor calibration | Pending |
| Buzzer warning optimization | Pending |
| Wearable frame integration | Pending |
| Real-world evaluation | Pending |

The development status will be updated as the project progresses.

## 8. Planned Testing

Future experiments will focus on:

### Obstacle Detection
- Distance measurement accuracy.
- Detection performance at different distances.
- Sensor response time.
- Detection limitations for different object shapes and materials.

### UV Monitoring
- Sensor response under different outdoor conditions.
- Calibration of UV measurements.
- Comparison with reference UV measurements.
- Reliability of warning thresholds.

### Wearability
- Total device weight.
- Component placement.
- Power consumption.
- Comfort during extended use.

### Warning System
- Distinguishability of different buzzer patterns.
- Warning response time.
- Reduction of unnecessary repeated alerts.

## 9. Current Limitations

This project is still an early-stage prototype.

Several limitations must be considered:

- The ultrasonic sensor has a limited detection field.
- Certain obstacles may not be reliably detected.
- The system does not currently detect stairs or ground-level hazards.
- UV measurements require calibration.
- The buzzer system may not be suitable for noisy environments.
- Battery life and wearable comfort have not yet been evaluated.
- The device has not been validated as a reliable mobility aid.

This prototype is not intended to replace a white cane or other established mobility assistance methods.

## 10. Future Improvements

Potential future developments include:

- Smaller distance sensors to reduce device weight.
- Multiple sensors for improved obstacle coverage.
- Vibration-based warnings.
- Pre-recorded voice alerts.
- Improved UV Index estimation.
- Rechargeable battery integration.
- Optimized circuit design and custom PCB development.
- More advanced warning algorithms.

These features are proposed improvements and are not included in the current prototype.

## 11. Project Motivation

This project was inspired by the challenges visually impaired individuals may face when navigating everyday environments.

The goal is to explore how accessible electronic components and embedded programming can be combined to develop practical assistive technologies.

Beyond building a functional prototype, the project provides an opportunity to study sensor integration, circuit design, embedded systems, and human-centered engineering.

## 12. Project Status

**Work in Progress**

The project is currently undergoing development.

Source code, circuit documentation, experimental results, and hardware photographs will be updated as new development milestones are reached.

## 13. License

This project is released under the MIT License. See the LICENSE file for details.
