# RescueTech Gauntlet Prototype

Welcome to the RescueTech Gauntlet Prototype repository! This project aims to develop a gauntlet equipped with various sensors to aid rescuers during earthquake-stricken building missions. The gauntlet provides real-time data about environmental conditions, gas leaks, and distance to ensure the safety of the rescuer.

## Table of Contents
- [Introduction](#rescuetech-gauntlet-prototype)
- [Getting Started](#getting-started)
  - [Prerequisites](#prerequisites)
  - [Installation](#installation)
- [Project Overview](#project-overview)
- [Sensor Details](#sensor-details)
  - [Gas Sensor (MQ-135)](#gas-sensor-mq-135)
  - [Temperature and Humidity Sensor (Adafruit AM2320)](#temperature-and-humidity-sensor-adafruit-am2320)
  - [Ultrasonic Distance Sensor (HC-SR04)](#ultrasonic-distance-sensor-hc-sr04)
- [Code Implementation](#code-implementation)
- [Design Process](#design-process)
- [Real-Time Data](#real-time-data)
- [Contributing](#contributing)
- [License](#license)

## Getting Started

### Prerequisites

To run this project, you will need the following components:

- Arduino board (e.g., ADAfruit ESP32 Feather)
- Gas Sensor (MQ-135)
- Temperature and Humidity Sensor (Adafruit AM2320)
- Ultrasonic Distance Sensor (HC-SR04)
- GME 12864-13 OLED Display
- Neopixel Ring 12

### Installation

1. Clone this repository to your local machine using Git:
git clone https://github.com/cxelshal/AstroGears.git


2. Connect the sensors and other components to your Arduino board following the circuit diagram provided in the "Circuit" directory.

3. Upload the Arduino code located in the "Code" directory to your Arduino board using the Arduino IDE.

4. Open the "index.html" file in your web browser to explore detailed information about the gauntlet prototype, sensor details, code implementation, design process, and real-time data.

## Project Overview

The RescueTech Gauntlet Prototype is designed to enhance the safety of rescuers during earthquake missions. It incorporates various sensors and components to monitor environmental conditions, detect gas leaks, and navigate through obstacles. The gauntlet also features an OLED display to provide real-time sensor readings and a Neopixel Ring 12 to respond to temperature and humidity values.

## Sensor Details

### Gas Sensor (MQ-135)

The Gas Sensor (MQ-135) is a gas-sensitive semiconductor sensor that detects dangerous gas leaks and provides early warnings. It is capable of detecting a wide range of gases, including benzene, alcohol, smoke, and other harmful substances commonly found in earthquake-stricken buildings.


#### Key Features:
- Detects multiple types of harmful gases.
- Provides analog output for gas concentration.
- Used in conjunction with the Arduino board for real-time gas monitoring.

For more information, refer to the Gas Sensor (MQ-135) Datasheet.

### Temperature and Humidity Sensor (Adafruit AM2320)

The Temperature and Humidity Sensor (Adafruit AM2320) is a digital sensor that measures ambient temperature and humidity levels. It is based on the I2C communication protocol, allowing easy integration with the Arduino board. The sensor provides accurate and reliable temperature and humidity readings, making it suitable for environmental monitoring in rescue missions.


#### Key Features:
- Provides accurate temperature and humidity measurements.
- Digital output for easy integration with the Arduino board.
- Low power consumption for extended operation.

For more information, refer to the Temperature and Humidity Sensor (Adafruit AM2320) Datasheet.

### Ultrasonic Distance Sensor (HC-SR04)

The Ultrasonic Distance Sensor (HC-SR04) is a non-contact sensor that measures distances by sending and receiving ultrasonic waves. It consists of a transmitter and a receiver module and calculates distance based on the time taken for the sound waves to bounce back after hitting an obstacle. The sensor is widely used for navigation in dark rooms and obstacle detection.


#### Key Features:
- Non-contact distance measurement using ultrasonic waves.
- Easy to use and integrate with the Arduino board.
- Effective obstacle detection for safe navigation.

For more information, refer to the Ultrasonic Distance Sensor (HC-SR04) Datasheet.

## Code Implementation

The Arduino code for the RescueTech Gauntlet Prototype is available in the "Code" directory. The code is designed to read data from the sensors, display real-time sensor data on the OLED display, control the Neopixel Ring 12 based on temperature and humidity readings, and provide audio alerts using the buzzer for short distance readings.

## Design Process

The design process for the RescueTech Gauntlet Prototype involved multiple 3D printing iterations. We focused on enhancing the gauntlet's ergonomics, durability, and aesthetics to optimize it for rescue missions.


During the design process, we made several changes to the gauntlet to meet our design goals.


The final design reflects improvements made to ensure the gauntlet's effectiveness and reliability during rescue operations.

## Real-Time Data

The Real-Time Data page provides live sensor readings from the RescueTech Gauntlet Prototype. The gauntlet's sensors continuously monitor environmental conditions, gas levels, and distance, and the data is displayed on the web page.

## Contributing

We welcome contributions to the RescueTech Gauntlet Prototype project! If you have any suggestions, improvements, or bug fixes, please feel free to submit a pull request.

## License

This project is licensed under the UCLA HaCK license. Feel free to use and modify the code according to the terms of the license.

For any questions or inquiries, please contact Team AstroGears at team@astrogears.w3spaces.com or call us at +15108125869.

Happy rescuing!

