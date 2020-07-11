# Green House
A software to manage your personal greenhouse system with PIC32

## Table of contents
* [General Info](#generalinfo)
* [Technologies](#technologies)
* [Setup](#setup)
* [Features](#features)
* [Contact](#contact)

## General Info
System to automate the greenhouse and program irrigation. This system allows to manage the irrigation time making sure to read the user input from the terminal and check that it is valid. Thanks to the accelerometer you can manage the overturning of the microcontroller. This project is an integral part of the evaluation of the microcontroller programming course.

## Technologies
The technologies used for the development of this software are as follows:
- TeraTerm to manage user input
- I2C to communicate with the LCD
- UART to manage messages between the user and the system
- ADC to turn the potentiometer value into seconds
 
## Setup
To run the system it is necessary to connect the communication usb with the pc and configure teraterm to have a baud rate of 115200. Once the user has entered the programmed watering time and reached the watering time, a message will appear on the LCD screen with the words "Start watering at XX:YY". At the end of watering (lasts 1 minute with "beep" sound) the message "Stop watering, duration of XX:YY" will be displayed. 

## Features
#### List of features ready
- Microcontroller overturn management(return it to the correct state and press the safety button)
- Beep during irrigation

## Contact
Feel free to contact me at mohamedboutaleb@outlook.it
