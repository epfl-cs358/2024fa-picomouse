# Picomouse

![Project Logo](path/to/logo.png) <!-- Optional: Add a project logo if needed -->

## Description

A brief description of what this project does and its purpose. 



## Table of Contents
1. [Description](#description)
2. [Mouse Components](#mouse-components)
4. [Mouse Build](#mouse-build)
7. [Software Installation](#software-installation)
8. [Sensors calibration](#sensors-calibration)
9. [Contributing](#contributing)
10. [License](#license)
11. [Acknowledgements](#acknowledgements)






### Mouse Components:
- 2 x N20 600rpm dc brushed motors
- MX1508 motor driver
- 9 volt battery
- ESP-32-vroom 32
- 4 x DFROBOT SEN0427 TOF
- DFROBOT SEN0245 TOF
- DFRobot Gravity I2C BMI160 Gyroscope
- Small Bead size=??????
- M2 screws and bolts 

### Mouse Build:
 ### 3d Printing the components:
  The 3d files are in the CAD folder.
  1.Print the base plate:
  ![Base Plate](images/to/logo.png)
  2.Print the motors holders:
  ![Motor holder](images/to/logo.png) 
  3.Print the bead holder (cap):
  ![Bead cap](images/to/logo.png) 
  4.Print the weels:
  ![Wheels](images/to/logo.png) 

### Assembling the Mouse:
  The whole mouse can be build with bolts and screws but threaded insert can also be used. \
  
  1.Place the motors and fix them with the motors holder:
  ![Motor holder](images/to/logo.png) 

  2.Place and screw the TOF's sensors to theire dedicated slot:
  ![TOF](images/to/logo.png) 

  3.Place and screw the bead cap:
  ![Bead Cap](images/to/logo.png) 

  4.Place and screw the gyroscope:
  ![Motor holder](images/to/logo.png) 
  
  5.Place and screw the buck convertor: 
  ![Buck convertor](images/to/logo.png) 

  

  
  

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
  Mid_y = ???? \
  Mid_Right_y = 0.995x - 4.407 \
  Right_y = 0.958x - 1.176 

  The values you will get need to be changed in sensors.h, the SLOPE and OFFSET of each captors. 

  ### Gyroscope calibration:
   The bmi60 chip is knowned for it's natural drift. To counter that the mouse, at initialization, \
   read's GYRO_CALIBRATION_NB_ITERATIONS times the gyroscope and average the values to obtain the offset.\
   The offset itself is not sufficient, we also need an activation treshold to negate small angular speed variation. \
   This parameter needs to be adapted to your gyroscope in gyroscope.h. \
   The gyroscope is also measured ???? times and average it out to mitigate the noise.
   This parameter can also be changed in gyroscope.h.

  

## Improvements:
  1. Reduce the use of floating point calculations in the code using "scaled" fixed point values to improve calculation speed.
  2. Reduce the BLE (bluetooth) library so it can be compiled on the esp and used for the python script.
  3. Use a PCB as the mouse base plate to reduce the weight of the plastic plate and the cable.
  4. Make smooth turns when running the maze (after the exploration phase).
  5. Use a simpler/cheaper gyroscope since we use only the angular speed of the x to y axis.
    
