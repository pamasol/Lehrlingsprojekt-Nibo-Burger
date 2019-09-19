# NIBO Burger – Pamasol Electronic project for apprentices

[Pamasol Willi Mäder AG](https://www.pamasol.com/) has specialised in the development and production of solutions for aerosols and spray systems since 1965. 

Its Automatiker Lehrlinge (German for “mechatronic apprentices”) go through a 4-year apprenticeship. They get an on the job training by qualified Pamasol employees and go to college one or two days per week.

At Pamasol they work on customer projects as well as on internal training projects. NIBO Burger represents the internal electronic training project. It pursues the following goals:

* Improving soldering skills
* Getting familiar with PCBs and electronic components (diode, LED, transistor, capacitor, amplifier, [microcontroller](https://en.wikipedia.org/wiki/Arduino_Uno))
* Acquiring knowledge of integrated circuits (motor bridges, [odometry](https://en.wikipedia.org/wiki/Odometry), USB programmer)
* Learning microcontroller programming with C and C++
* Becoming acquainted with control loops like [PID controller](https://en.wikipedia.org/wiki/PID_controller)
* Using Git as software version control system and GitHub for collaborative software development
* Enhancing documentation and presentation skills
* Getting comfortable with definition of project and interpreting datasheets in English (native language of most apprentices is Swiss-German)

**Table of contents:**
1. [Robot construction kit NIBO Burger](#robot-construction-kit-nibo-burger)
2. [Robot tasks](#robot-tasks)
3. [Assembling NIBO Burger](#assembling-nibo-burger)
4. [Programming with Arduino and Git](#programming-with-arduino-and-git)
5. [Documentation](#documentation)

In a first step the roboter kit is presented. The tasks which the robot has to solve are described in a second chapter, followed by robot assembling, programming and documentation.

# Robot construction kit NIBO Burger
The NIBO burger robot kit is a free programmable robot that can act autonomously with its environment. It is equipped with an Atmel ATmega16 AVR main controller, 9 sensors and a slot for ARDUINO shields where for example an 8x8 LED matrix display can be mounted. It comes as an assembly kit that is provided by [nicai-systems](http://www.nicai-systems.com/), Germany. 

NIBO Burger with 8x8 LED matrix display | Powered via 4x1.2V AAA battery
:-------------------------:|:-------------------------:
![NIBO Burger with 8x8 LED matrix display](https://github.com/pamasol/Lehrlingsprojekt-Nibo-Burger/blob/master/documentation/src/nibo-burger_01.png "NIBO Burger with 8x8 LED matrix display")  |  ![Powered via 4x1.2V AAA battery](https://github.com/pamasol/Lehrlingsprojekt-Nibo-Burger/blob/master/documentation/src/nibo-burger_02.png "Powered via 4 x 1.2V AAA battery")

IR and color sensor bricks | Sensor protection wings
:-------------------------:|:-------------------------:
![IR and Color sensor bricks ](https://github.com/pamasol/Lehrlingsprojekt-Nibo-Burger/blob/master/documentation/src/nibo-burger_03.png "IR and Color sensor bricks ")  |  ![Sensor protection wings](https://github.com/pamasol/Lehrlingsprojekt-Nibo-Burger/blob/master/documentation/src/nibo-burger_04.png "Sensor protection wings")

Features of NIBO Burger:
* Main processing unit Atmel ATmega16A, 16 kByte flash, 15 MHz
* 4 IR-Sensor-Bricks for contactless collision avoidance
* 3 Colour-Sensor-Bricks (R+G+B) for surface colour detection
* 10 Slots for variable sensor configuration
* 2 motors with 25:1 / 125:1 gear ratio
* 2 IR-sensors for rotation speed detection

Additional information can be found on the product website: [www.nicai-systems.com/en/nibo-burger](http://www.nicai-systems.com/en/nibo-burger)

> Hint: Best offer we could find for purchasing a NIBO Burger in Switzerland at time of writing is [Brack.ch](https://www.brack.ch/nicai-systems-bausatz-nibo-burger-379831) with CHF 77.00. Please let us know if there are better deals out there.


# Robot tasks

The following 5 tasks must be mastered, whereas the apprentice can choose the order by himself.

### 5m straightforward  
The robot should move exactly 5 meters without any longitudinal and lateral deviation.

### Fraidy cat  
The robot should avoid obstacles to the right or left as shown in [this video](https://youtu.be/c-cpGMVMM1k).

### Follow me  
The robot should follow a human hand as shown in [this video](https://youtu.be/KMAIyAhpvuM).

### Colour detection  
The robot should detect the colours blue, red, yellow and green. Colours should be indicated on the 8x8 pixel display as shown in [this video](https://youtu.be/4foVZdhxUQg).

### Rabbit warren  
The robot should follow a path from point A to B as shown in [this video](https://youtu.be/VxXwEKlsGJY).


# Assembling NIBO Burger

How to assemble the robot is described detailly in the manual that is provided with the robot.

* [English Manual NIBO Burger (PDF)](https://github.com/pamasol/Lehrlingsprojekt-Nibo-Burger/blob/master/documentation/src/Manual_NIBOburger_20150908.pdf)
* [German Manual NIBO Burger (PDF)](https://github.com/pamasol/Lehrlingsprojekt-Nibo-Burger/blob/master/documentation/src/Doku_NIBOburger_20150909.pdf)
* [Electrical Schematic NIBO Burger (PDF)](https://github.com/pamasol/Lehrlingsprojekt-Nibo-Burger/blob/master/documentation/src/nibo_burger_schematic_1_09.pdf)

It starts with interpreting and sorting the items followed by preparing the workplace and the soldering station. One should proceed as recommended in the instructions to avoid issues when putting the boards together to a burger.

Following points are not mentioned in the manual but recommended:
* The phototransistors of the IR sensors should be protected with shrinking tube to reduce interference from other sensors.

Since the microcontroller is preprogramed on can run the function tests as soon as the hardware work is done.

The maroon shield 8x8 matrix display is super easy to assemble. That is why its manual consists of 2 pages only.

* [English Manual maroon shield matrix display](https://github.com/pamasol/Lehrlingsprojekt-Nibo-Burger/blob/master/documentation/src/Anleitung-EN_maroon_SHIELD.pdf)
* [German Manual maroon shield matrix display](https://github.com/pamasol/Lehrlingsprojekt-Nibo-Burger/blob/master/documentation/src/Anleitung_maroon_Shield.pdf)
* [Electrical Schematic maroon shield matrix display](https://github.com/pamasol/Lehrlingsprojekt-Nibo-Burger/blob/master/documentation/src/maroon_shield_1_03_schematic.pdf)



# Programming with Arduino and Git

ToDo


# Documentation

ToDo
