# 2WD-bluetooth-controlled-bot

A 2WD differential drive robot built using an Arduino Uno, L298N motor driver, HC-05 Bluetooth module and PlatformIO.

This is the first stage of a long-term robotics project that will eventually include:

- Wheel encoders
- PID motor speed control
- Distance and angle control
- Odometry
- ROS2 integration
- SLAM
- Autonomous Navigation

## Features

- Bluetooth control using the Arduino Bluetooth Controller app
- Differential drive
- PWM speed control
- Speed adjustment from the app
- Diagonal movement support
- Encoder interrupt support (ready for future phases)

## Hardware

- Arduino Uno
- L298N Motor Driver
- HC-05 Bluetooth Module
- 2 TT DC Motors
- 2 LM393 Encoder Modules
- 7.4V Li-ion Battery
- Buck Converter (5V)

## Wiring

| Device | Arduino |
|---------|----------|
| ENA | D5 |
| ENB | D6 |
| IN1 | D8 |
| IN2 | D9 |
| IN3 | D10 |
| IN4 | D11 |
| Left Encoder | D2 |
| Right Encoder | D3 |
| HC-05 TX | D4 |
| HC-05 RX | D7 |

## Software

- PlatformIO
- Arduino Framework
- C++

## Demo

See the video [here](https://www.youtube.com/shorts/QXy0v4SZiB4).

## Roadmap

- [x] Bluetooth Control
- [x] Encoder Integration
- [ ] RPM Calculation
- [ ] PID Speed Control
- [ ] Drive Exact Distance
- [ ] Turn Exact Angle
- [ ] Ultrasonic Obstacle Avoidance
- [ ] ROS2 Serial Bridge
- [ ] Odometry
- [ ] SLAM
- [ ] Navigation2

## Author

Shamith Nirmal
