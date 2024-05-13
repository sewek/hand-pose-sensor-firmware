# Hand pose sensor firmware
This repository contains the firmware for the hand pose sensor. The firmware is written in C++ and is based on the [NRF Connect SDK](https://www.nordicsemi.com/Software-and-Tools/Software/nRF-Connect-SDK). The firmware is designed to run on the [Nordic Semiconductor nRF52840](https://www.nordicsemi.com/Products/Low-power-short-range-wireless/nRF52840) microcontroller.

## Description
The firmware is designed to run on the hand pose sensor. The hand pose sensor is a wearable device that can be worn on the hand. The sensor is equipped with multiple IMU sensors that can be used to track the orientation of the hand and fingers. The sensor can be used to track hand gestures and finger movements. The sensor can be used for a variety of applications, such as virtual reality, augmented reality, and human-computer interaction.

## Features
The firmware has the following features:
- IMU sensor analysis: The firmware can read data from multiple IMU sensors and perform sensor fusion to estimate the orientation of the hand and fingers.
- Gesture recognition: The firmware can recognize a variety of hand gestures and finger movements.
- Wireless communication: The firmware can communicate wirelessly with a host device, such as a smartphone or computer, using Bluetooth Low Energy (BLE).
- Data logging: The firmware can log sensor data to an SD card for offline analysis.

## Getting started

### Easy way
Easy way to get started is to use devcontainer. This will setup the development environment for you.
Requirements:
- Docker
- Visual Studio Code
- Remote - Containers extension for Visual Studio Code

To get started:
1. Clone the repository
2. Open the repository in Visual Studio Code
3. Click on the "Reopen in Container" button in the bottom right corner of Visual Studio Code
4. Wait for the container to build
5. Open a terminal in Visual Studio Code and run the following commands:
```bash
west init -l .
west update
west build -b hand_pose_sensor
```
6. Connect the nRF52840 DK to your computer using a USB cable
7. Run the following command to flash the firmware to the nRF52840 DK:
```bash
west flash
```

### Manual way
Requirements:
- Python 3
- pip
- west
- nRF Connect SDK

To get started:
1. Clone the repository
2. Install the nRF Connect SDK by following the instructions [here](https://developer.nordicsemi.com/nRF_Connect_SDK/doc/latest/nrf/gs_installing.html)
3. Open a terminal and navigate to the root of the repository
4. Run the following commands to initialize the nRF Connect SDK:
```bash
west init -l .
west update
```
5. Run the following command to build the firmware:
```bash
west build -b hand_pose_sensor
```
6. Connect the nRF52840 DK to your computer using a USB cable
7. Run the following command to flash the firmware to the nRF52840 DK:
```bash
west flash
```

## License
This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.