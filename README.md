# Smart Accident Detection and Emergency Alert System

## Overview
This project is an Arduino-based accident detection and emergency alert system designed to improve road safety. The system detects accidents using sensors and sends an emergency alert when a collision is detected.

## Features
- Accident detection using sensors
- Automatic emergency alert generation
- Real-time monitoring
- Low-cost embedded solution
 
## Components Used

- Arduino Uno
- MPU6050 Accelerometer and Gyroscope Sensor
- GSM Module
- GPS Module
- LCD Display
- Buzzer
- Push Button
- PCB
- Power Supply

## Working Principle

1. The MPU6050 continuously monitors acceleration and tilt values.
2. When the measured values exceed predefined thresholds, an accident is assumed.
3. The buzzer is activated and a countdown begins.
4. The user can cancel the alert using the push button if it is a false alarm.
5. If no response is received, the GPS module acquires the current location.
6. The GSM module sends an emergency SMS containing the accident notification and GPS coordinates.
7. System status is displayed on the LCD screen.
 
## Technologies Used

- Arduino IDE
- Embedded C
- Serial Communication
- GPS Tracking
- GSM Communication
  
## Applications

- Vehicle Safety Systems
- Emergency Response Systems
- Fleet Monitoring
- Smart Transportation

## Future Improvements

- Mobile Application Integration
- Cloud-Based Monitoring
- Real-Time Accident Analytics
- IoT Dashboard for Emergency Services

  
