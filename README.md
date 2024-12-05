# Picomouse

![Project Logo](path/to/logo.png) <!-- Optional: Add a project logo if needed -->

## Description

A brief description of what this project does and its purpose. 

## Features

- Feature 1:
- Feature 2: 
- Feature 3: 
- Feature 4:

## Table of Contents
1. [Description](#description)
2. [Features](#features) 4 files changed, 0 insertions(+), 0 deletions(-)
 create mode 100644 images/left_captor_calibration.png
 create mode 100644 images/mid_left_captor_calibration.png
 create mode 100644 images/mid_right_captor_calibration.png
 create mode 100644 images/right_captor_calibration.png

3. [Getting Started](#getting-started)
4. [Usage](#usage)
5. [Software Installation](#software-installation)
6. [Sensors calibration](#sensors_calibration)
7. [Contributing](#contributing)
8. [License](#license)
9. [Acknowledgements](#acknowledgements)



## Getting Started



### Mouse Components:
- 2 x N20 600rpm dc brushed motors
- MX1508 motor driver
- 9 volt battery
- ESP-32-vroom 32
- 4 x DFROBOT SEN0427 TOF
- DFROBOT SEN0245 TOF
- DFRobot Gravity I2C BMI160 Gyroscope
  





## Software Installation
  ### Software Prerequisites:
  
  - Arduino IDE
  - Python 3.10
  - Python libraries: (bleak, pygame)

### Installation:

1. Clone the repository to your local machine:
   ```bash
   git clone https://github.com/epfl-cs358/2024fa-picomouse.git
   cd 2024fa-picomouse
2. Install python dependecies:
   ```bash
   pip install -r requirements.txt

## Sensors calibration:
  ### TOF calibration:
  The Tof sensors need to be calibrated individualy, take measure of the sensors at different distances \
  and do a linear regression on the measure given the actual distance. \
  For our sensors we get the following graphs: \
  Where the x axis denotes the real distance and the y axis denotes the measured value. \
  The green line represent the expected slope, and the black line represent the measurements. \

  Left TOF: 
  
  ![Left TOF](images/left_captor_calibration.png)

  Mid Left TOF:

  ![Mid-Left TOF](images/mid_left_captor_calibration.png)

  Mid Right TOF:
  
  ![Mid-Right TOF](images/mid_right_captor_calibration.png)

  Right TOF

  ![Right TOF](images/right_captor_calibration.png)

  We get 5 slopes: \
  Left_y = 0.951x - 6.273 \
  Mid_Left_y = 0.998x - 0.189 \
  Mid_Right_y = 0.995x - 4.407 \
  Right_y = 0.958x - 1.176 \

  The vallues you will get need to be changed in sensors.h. \

  ### Gy


## Improvements:
  1. Reduce the use of floating point calculations in the code using "scaled" fixed point values to improve calculation speed.
  2. Reduce the BLE (bluetooth) library so it can be compiled on the esp and used for the python script.
  3. Use a PCB as the mouse base plate to reduce the weight of the plastic plate and the cable.
  4. Make smooth turns when running the maze (after the exploration phase).
    
    
