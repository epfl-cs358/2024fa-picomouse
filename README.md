# Picomouse

## Description
 
This project aims to create from scratch a micromouse and be able to run it on a 10x10 maze. The design of the mouse, the choice of components, the implementation of the code and the construction of the maze are parts of the project. Our team was responsible of the walls and pillars and another team had to construct the floor. The purpose of this project is to discover how to combine hardware and software and to learn how to work in a team of 6 persons.


## Table of Contents
1. [Description](#description)
2. [Mouse Components](#mouse-components)
3. [Mouse Preparation](#mouse-preparation)
4. [Mouse Assembly](#assembling-the-mouse)
5. [Maze](#maze)
6. [Software Overview](#software-overview)
7. [Software Installation](#software-installation)
8. [Sensors calibration](#sensors-calibration)
9. [Distance calculation](#distance-calculation)
10. [Computation distances with encoders](#computation-distances-with-encoders)
11. [What does the mouse?](#what-does-the-mouse)
12. [Improvements](#improvements)
    




## Mouse Components:
- 2 x N20 600rpm dc brushed motors
- MX1508 motor driver
- 9 volt battery
- ESP-32-vroom 32
- 5 x DFROBOT SEN0427 TOF
- DFRobot Gravity I2C BMI160 Gyroscope
- Small Bead
- M2 screws and bolts 

## Mouse Preparation:
 ### 3d Printing the components:
  The 3d files are in the CAD folder. \
  We used 10% infill for the printing. \
  1.Print the base plate: 
  
  ![Base Plate](images/3d_printing/base_plate.jpg)
  
  2.Print the motors holders: 
  
  ![Motor holder](images/3d_printing/motor_holders.jpg) 
  
  3.Print the bead holder (cap): 
  
  ![Bead cap](images/3d_printing/bead_cap.jpg) 
  
  4.Print the weels: 
  The weels are composed of 2 parts: 
  - The inner wheel support printed in hard plastic (PETG/PLA). 
  - Our rubber is printed with ninjaflex TPU but elastic rubber can also be used.

  ![wheels](images/assembly/wheels_rubber.jpg)
  

  

## Assembling the Mouse:
  The whole mouse can be build with bolts and screws but threaded insert can be used to disassemble the mouse easily.

  #### 1.Assemble the wheels:
  Glue the rubber on the wheel. \
  Place a M2 hexnut in the dedicated slot.
  
  ![wheels_rubber](images/assembly/wheel.jpg)


  #### 2.Place and screw the buck convertor: 
    
  ![Buck convertor](images/assembly/buck_convertor.jpg) 


  
 #### 3.Place the motors and fix them with the motors holder: 
   
  ![Motor holder](images/assembly/motor_holder.jpg) 

 #### 4.Assemble the weels and fix them to the motor axis:
 

 #### 5.Place the battery: 
 
  ![Batery](images/assembly/batery.jpg) 

 #### 6.Place and screw the TOF's sensors to their dedicated slot: 
   
  ![TOF](images/assembly/TOF.jpg) 

 #### 7.Place and screw the gyroscope: 
  
  ![Motor holder](images/assembly/gyroscope.jpg) 
  
  #### 8.Place and screw the ESP-32 to the back of the motor holder: 
  
  ![ESP-32](images/assembly/esp32_on_holder.jpg) 
  ![ESP-32](images/assembly/esp32_on_holder_2.jpg)

  ### Wiring:
  ![image](https://github.com/user-attachments/assets/c0026920-46d1-4338-90c0-385cb2a7d65f)



## Maze
The competition maze is made by 18cm x 18cm cells with 2cm width walls and 16 cm x 16 cm space inside the cells.
There is also an other maze design that uses 17cm x 17cm cells with 1cm width walls. \
We decided to make the first option to align with the competition design however the second option uses less platic and wood. \
We had the constraint to make a maze that is easily dissmountable thus we used Scratch to attach the walls and the pillars to a carpet. 

### Walls:
Option1. We used 6mm and 8mm thick mdf plates. \
    See the CAD for the dimension of the cut, we used a C02 laser cutter. \
    The wall is composed of 2 plates of 16cm x 5cm and 2 dowel of 2.5cm x 4.5cm that need to be glued together. 

    
![walls2cm](images/maze/2cm_wall_side.jpg) 

![walls2](images/maze/wall_assemble.jpg)     

    

Option2. Same as option 1 but with 3mm and 4mm thick mdf plates. \
We get the following wall:

![wall1cm](images/maze/1cm_wall_above.jpg) 
    
    

    
### Pillars:
Option1. 3d print the 2cm x 2cm x 5cm pillars \

![pillar_2](images/maze/pillar_2cm.jpg) 

Option2. 3d print the 1cm x 1cm x 5cm pillars \

![pillar_1](images/maze/pillar_1cm.jpg) 
    
Both of them have an option to add a M2 hexnut inside so the pillars can be screwed underneath.\
The bolt can easily be placed using a screwdriver by pushing the bolt until you hear a pop for the 2cm x 2cm pillar. \
For the 1cm x 1cm pillar the bolt has to be glued underneath. \
The bolt should be locked in place. 

![pillar_hexnut](images/maze/pillar_hexnut.jpg) 

![pillar_screw](images/maze/pillar_screw.jpg) 


    
    
## Software Overview
### Code interface
You can find a detailed scheme representing the relations of all components and pieces of code ![here](c4_model.png) (in .png) and ![here](c4_model.pdf) (in .pdf).
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
   ```


## Sensors calibration:
  ### TOF calibration:
  The Tof sensors need to be calibrated individually, to do that we measured different distances for each sensors and did a linear regression on the measurement     given the actual distance. \
  For our sensors we get the following graphs: \
  Where the x axis denotes the real distance and the y axis denotes the measured value. \
  The green line represent the expected slope, and the black line represent the measurements. 

  ![all TOFs](images/calibration/all_tofs_calibration.png)

  We get 5 slopes: \
  Left_y = 0.951x - 6.273 \
  Mid_Left_y = 0.998x - 0.189 \
  Mid_y = 1.000x - 0.000\
  Mid_Right_y = 0.995x - 4.407 \
  Right_y = 0.958x - 1.176 

  The values you will get need to be changed in sensors.h, the SLOPE and OFFSET of each captors. 

  ### Gyroscope calibration:
   The bmi60 chip is knowned for it's natural drift. To counter that the mouse, at initialization, \
   read's CALIBRATION_OFFSET_NB_ITERATIONS times the gyroscope and average the values to obtain the offset.\
   The offset itself is not sufficient, we also calibrated it with an affine function.\
   The gyroscope is also measured GYRO_NB_ITERATIONS times and average it out to mitigate the noise. \
   This parameter can also be changed in gyroscope.h. \
   If required, you can add and activation threshold.

### Distance Calculation:
Using the TOF sensors we can estimate the mouse position to a left or right wall.
We can get the mouse orientation and its distance to the wall (from the center of the mouse).
![trigo_1](images/calibration/dist_calc_2.jpg)

$$
    \begin{aligned}
    \text{Given: } \ & \alpha_1 , \ c_1 , \ \beta_1, \ r \\
    \text{To be found: } \ & d, \ \theta_1 \\
    \newline
    d &= \sqrt{r^2 + c_1^2 - 2 \cdot r \cdot c_1 \cdot \cos(\pi - \beta_1)} \\
    \theta_1 &= \alpha_1 + \arccos\left(\frac{d^2 + c_1^2 - r^2}{2 \cdot d \cdot c_1}\right) \\
    \end{aligned}
$$

![trigo_2](images/calibration/distance_calcul.jpg)

$$
    \begin{aligned}
    \text{Given: } \ & d_1, \ \theta_1, \ d_2, \ \theta_2 \\
    \text{To be found: } \ & d_3, \ d_5, \ d_6 \\
    \newline
    d_3 &= \sqrt{d_1^2 + d_2^2 - 2 \cdot d_1 \cdot d_2 \cdot \cos(\theta_1 - \theta_2)} \\
    d_5 &= \frac{d_2^2 + d_3^2 - d_1^2}{2 \cdot d_3} \\
    d_6 &= \sqrt{d_2^2 - d_5^2} \\
    \end{aligned}
$$

If you change the sensors disposition you need to changes those values in sensors.cpp.

## Computation distances with encoders
To compute the travel distance, one needs to know the real perimeter of the wheels and the number of pulsations transmitted by the encoder during one wheel revolution. To do that, you can modify in motion.h the constant WHEEL_DIAMETER and ENCODER_RESOLUTION.\
To compute the encoder's resolution, print get_steps_count() in a while loop and turn manually the wheel to measure the number of steps needed for one revolution.

## UI
The UI made for monitoring the mouse's position and state of the maze stored in the micro-controller is written in Python, using the [Pygame Community Edition library](https://github.com/pygame-community/pygame-ce). 
To use the provided code, you'll need to install pygame-ce (``pip install pygame-ce``) which should already be done if you installed the requirements.

### How to use
If you are simply trying to visualize the maze and play with it offline, you'll have to understand how it is interpreted by the code. Every cell is represented by a set of 4 bits. Each of these bits represents a wall. In order, the first bit represents the left wall, the second, the upper wall, the third, the right wall, the fourth, the bottom wall (i.e. 0b1010 represents a cell with a wall on the left and wall on the right).
#### Example of a complete maze
```
maze = [[0b0000, 0b0101, 0b0110, 0b1000, 0b0000, 0b0000, 0b0000, 0b0000],
        [0b1010, 0b1111, 0b1011, 0b1000, 0b0000, 0b0001, 0b0000, 0b0000],
        [0b1001, 0b0110, 0b1101, 0b0000, 0b0011, 0b1110, 0b1001, 0b0000],
        [0b0110, 0b1001, 0b0110, 0b1011, 0b1110, 0b1010, 0b1111, 0b1000],
        [0b0000, 0b0110, 0b1001, 0b0101, 0b0001, 0b0010, 0b1101, 0b0000],
        [0b0000, 0b0000, 0b0100, 0b0100, 0b0110, 0b1001, 0b0110, 0b0000],
        [0b0000, 0b0000, 0b0000, 0b0000, 0b0000, 0b0100, 0b0000, 0b0000],
        [0b0000, 0b0000, 0b0000, 0b0000, 0b0000, 0b0000, 0b0000, 0b0000]]
```
This represents the following maze :
![Example UI Maze](https://github.com/user-attachments/assets/cf55897d-5dbc-45f4-a3e9-48da468660a9)

## What does the mouse?:
The mouse navigate through the maze with the support of the gyroscope and the sensors. She does 3 runs : one to search the target cell (corner of the maze in diagonal of the start cell) by stopping in each cell, one to search a second path that could be faster than the first one when returning to the start cell, and one to run through the maze with the best path and without stopping at each cell. We use the gyroscope to keep track of and correct the current angle of the mouse in the global space. The sensors were used to detect the walls at the sides and in front of the mouse, and to recalibrate her when facing a wall.

## Improvements:
  1. Reduce the BLE (bluetooth) library so it can be compiled on the esp and used for the python script.
  2. Use a PCB as the mouse base plate to reduce the weight of the plastic plate and the cable.
  3. Make smooth turns when running the maze (after the exploration phase).
  4. Redesign of the mouse: bring closer the center of rotation to the sensors to make in place turns in less space.
  5. Better error handling for imperfect maze.
  6. Implementation of PID algorithm for sensors. The gyroscope has some limits in the precision of the turns and in straight lines, inclusion of the sensors in         the process would improve the general navigation.
  7. Ligthen weight of the mouse with a LiPo and a smaller buck converter.
  8. Usage of buck converter on motors to run at constant 6V.
  9. Redesign the floor of the maze to be more consistent (especially for the walls).
  10. Reduce the use of floating point calculations in the code using "scaled" fixed point values to improve calculation speed.
    
