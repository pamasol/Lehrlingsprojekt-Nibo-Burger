# NIBO Burger – Pamasol electronic project for apprentices

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
* Getting comfortable with project definition and datasheets in English (native language of most apprentices is Swiss-German)

**Table of contents:**
1. [Robot construction kit NIBO Burger](#1-robot-construction-kit-nibo-burgerr)
2. [Robot tasks](#2-robot-tasks)
3. [Assembling NIBO Burger](#3-assembling-nibo-burger)
4. [Programming with AVR Studio and Git](#4-programming-with-avr-studio-and-git)
5. [Documentation](#5-documentation)

In a first step the roboter kit is presented. The tasks which the robot has to solve are described in a second chapter, followed by robot assembling, programming and documentation.

# 1. Robot construction kit NIBO Burger
The NIBO burger robot kit is a free programmable robot that can act autonomously with its environment. It is equipped with an Atmel ATmega16 AVR main controller, 9 sensors and a slot for ARDUINO shields where for example an 8x8 LED matrix display can be mounted. It comes as an assembly kit that is provided by [nicai-systems](http://www.nicai-systems.com/), Germany. 

NIBO Burger with 8x8 LED matrix display | Powered via 4x1.2V AAA battery
:-------------------------:|:-------------------------:
![NIBO Burger with 8x8 LED matrix display](https://user-images.githubusercontent.com/7021907/65602250-dd7b0100-dfa3-11e9-96bf-c2065803712c.png "NIBO Burger with 8x8 LED matrix display")  |  ![Powered via 4x1.2V AAA battery](https://user-images.githubusercontent.com/7021907/65602251-dd7b0100-dfa3-11e9-83b5-0e59b2f1853e.png "Powered via 4 x 1.2V AAA battery")

IR and color sensor bricks | Sensor protection wings
:-------------------------:|:-------------------------:
![IR and Color sensor bricks ](https://user-images.githubusercontent.com/7021907/65602252-dd7b0100-dfa3-11e9-93f1-be680089db00.png "IR and Color sensor bricks ")  |  ![Sensor protection wings](https://user-images.githubusercontent.com/7021907/65602253-de139780-dfa3-11e9-80d9-09017d9aeeda.png "Sensor protection wings")

Features of NIBO Burger:
* Main processing unit Atmel ATmega16A, 16 kByte flash, 15 MHz
* 4 IR-Sensor-Bricks for contactless collision avoidance
* 3 Colour-Sensor-Bricks (R+G+B) for surface colour detection
* 10 Slots for variable sensor configuration
* 2 motors with 25:1 / 125:1 gear ratio
* 2 IR-sensors for rotation speed detection

Additional information can be found on the product website: [www.nicai-systems.com/en/nibo-burger](http://www.nicai-systems.com/en/nibo-burger)

> Hint: Best offer we could find for purchasing a NIBO Burger in Switzerland at time of writing is [Brack.ch](https://www.brack.ch/nicai-systems-bausatz-nibo-burger-379831) with CHF 77.00. Please let us know if there are better deals out there.


# 2. Robot tasks

The roboter tasks are divided into two parts, the excercise tasks and the master tasks. The exercise tasks can be done with help of [NIBO burger Coding Tutorial](https://www.roboter.cc/index.php?option=com_wrapper&view=wrapper&Itemid=63). Do not create a new project in Atmel Studio for every task, use Git instead efficiently.

## Excercise tasks

### A) Flashing LED1
LED 1 should flash for 0.5sec and then pause for 0.5sec. This process should be repeated infinitely (loop).

[Solution](https://github.com/pamasol/Lehrlingsprojekt-Nibo-Burger/blob/master/PhilippBruhin/Excercise_tasks/a_flashing_led_1/a_flashing_led_1/main.c)

### B) For loop
The C for loop statement is used to execute a block of code repeatedly. It is often used when the number of iterations is predetermined. Use a for loop for switching on LED1 up to LED4.

[Solution](https://github.com/pamasol/Lehrlingsprojekt-Nibo-Burger/blob/master/PhilippBruhin/Excercise_tasks/b_for_loop/b_for_loop/main.c)

### C) Switch on LED2-LED4
Update program B) from above with the result that only LED2 up to LED4 are on.

[Solution](https://github.com/pamasol/Lehrlingsprojekt-Nibo-Burger/blob/master/PhilippBruhin/Excercise_tasks/c_switch_on_led2-led4/c_switch_on_led2-led4/main.c)

### D) Flashing LEDs 1
Create a program where LED 1 is flashing for 0.5sec and then pausing for 0.5sec, then LED 2 is flashing for 0.5sec and then pausing for 0.5sec and so on up to LED4. Don't use a for loop.

[Solution](https://github.com/pamasol/Lehrlingsprojekt-Nibo-Burger/blob/master/PhilippBruhin/Excercise_tasks/d_flashing_leds_1/d_flashing_leds_1/main.c)

### E) Flashing LEDs 2
Write same program as above in D) but work with a for loop. What is the advantage of a for loop in this case?

[Solution](https://github.com/pamasol/Lehrlingsprojekt-Nibo-Burger/blob/master/PhilippBruhin/Excercise_tasks/e_flashing_leds_2/e_flashing_leds_2/main.c)

### F) For in for
Nest program E) from above in a loop that increases the time from 50 to 100 and 200 milliseconds (each iteration double of time).

[Solution](https://github.com/pamasol/Lehrlingsprojekt-Nibo-Burger/blob/master/PhilippBruhin/Excercise_tasks/f_for_in_for/f_for_in_for/main.c)

### G) Using buttons
Analyze [this program](https://github.com/pamasol/Lehrlingsprojekt-Nibo-Burger/blob/eaf6fde06b96f30fbcf23a8362a2644a54070c86/PhilippBruhin/Excercise_tasks/g_using_buttons/g_using_buttons/main.c) which uses buttons and a switch case statement. Now add an event for button 3. If this button is pressed, LED3 should flash and as soon as the button is released, LED3 should switch off.

[Solution](https://github.com/pamasol/Lehrlingsprojekt-Nibo-Burger/blob/master/PhilippBruhin/Excercise_tasks/g_using_buttons/g_using_buttons/main.c)

### H) Light switch
Write a program based on [this initial situation](https://github.com/pamasol/Lehrlingsprojekt-Nibo-Burger/blob/5efa40aacb6b36359cd94a89daca1e62d4111b8e/PhilippBruhin/Excercise_tasks/h_light_switch/h_light_switch/main.c) that switches on LED1 and LED4 when button 1 is clicked. LED2 and LED3 should switch on when button 2 is clicked. All LEDs should switch off when button 3 is clicked.

[Solution](https://github.com/pamasol/Lehrlingsprojekt-Nibo-Burger/blob/master/PhilippBruhin/Excercise_tasks/h_light_switch/h_light_switch/main.c)

### I) Odometry sensors
Check [this program](https://github.com/pamasol/Lehrlingsprojekt-Nibo-Burger/blob/fd1ff6b52d8f29d7a4f62cfac1a31953eee3e15d/PhilippBruhin/Excercise_tasks/i_odometry_sensors/i_odometry_sensors/main.c). The photoelectric sensors that are placed in the transmission are called odometry sensors. The program measures the readings of the left odometry sensor and switches on LED1 as soon as it counts more than 10 and LED2 when it is more than 20. The counter of left odometry sensor can be reset (counter back to 0) with clicking button 1 or button 3. Implement the same behavior for the right odometry sensor. The program can be tested by turning the red gear.

[Solution](https://github.com/pamasol/Lehrlingsprojekt-Nibo-Burger/blob/master/PhilippBruhin/Excercise_tasks/i_odometry_sensors/i_odometry_sensors/main.c)

### J) Playing with motors
The motors have to be initialized via `motpwm_init();` in the setup function. Check setup function in [this program](https://github.com/pamasol/Lehrlingsprojekt-Nibo-Burger/blob/aa2d69d422beaac5624e22e60bb058737606038a/PhilippBruhin/Excercise_tasks/j_playing_with_motors/j_playing_with_motors/main.c). The program furthermore explaines local and global variables and the motor control. Extend it as follows: When button 3 is released motor power variable should be decreased by 200.

[Solution](https://github.com/pamasol/Lehrlingsprojekt-Nibo-Burger/blob/master/PhilippBruhin/Excercise_tasks/j_playing_with_motors/j_playing_with_motors/main.c)

### K) Sensor calibration
In this excercise one will calibrate the color sensors of the roboter. Therefore create a program with [this content](https://github.com/pamasol/Lehrlingsprojekt-Nibo-Burger/blob/master/PhilippBruhin/Excercise_tasks/k_sensor_calibration/k_sensor_calibration/main.c), compile it and download it to the robot. Put robot on a **black surface** and click **button 1**. In a second step, put the robot on a **white surface** and click **button 2**.

Since the [RGB values](https://en.wikipedia.org/wiki/RGB_color_model) will be stored in the [EEPROM](https://en.wikipedia.org/wiki/EEPROM), they will not get lost anymore, even when turning off or reprogramming the robot. Furthermore a new variable type is used: `unsigned long int` what is a [2 byte variable](https://gcc.gnu.org/wiki/avr-gcc#Deviations_from_the_Standard) that can be positive only (no - in front).

If everything went well, LED1 is flashing when sensors detect a black surface. LED2 is flashing when sensors detect a white surface.

### L) Infrared sensors
Take a look at [this program](). It switches off all LEDs when IR-Sensor (respectively phototransistor) in slot FL does not receive reflected [infrared light](https://en.wikipedia.org/wiki/Infrared) from the IR diode. The better the reflection get, the more LEDs are flashing. Update the code that when there is no reflection, all LEDs are switched off. Is ther a bit reflection, LED1 swichtes on. Is there more reflection, LED2 switches on additionally and so until LED4 and high reflection. 

[Solution]()

### M) Visualize obstacle


## Master tasks

The following 5 tasks must be mastered, whereas the apprentice can choose the order by himself.

### A) Round trip  
The robot should move 1.5m or more in one direction, turn 180° and move back to its origin.

[Layout drawing round trip (PDF)](https://github.com/pamasol/Lehrlingsprojekt-Nibo-Burger/files/3652583/Nibo01_V1.0.pdf)

### B) Fraidy cat  
The robot should avoid obstacles to the right or left as shown in [this video](https://youtu.be/c-cpGMVMM1k).

[Layout drawing fraidy cat (PDF)](https://github.com/pamasol/Lehrlingsprojekt-Nibo-Burger/files/3652584/Nibo02_V1.0.pdf)

### C) Follow me  
The robot should follow a human hand as shown in [this video](https://youtu.be/KMAIyAhpvuM).

[Layout drawing follow me (PDF)](https://github.com/pamasol/Lehrlingsprojekt-Nibo-Burger/files/3652585/Nibo03_V1.0.pdf)

### D) Colour detection  
The robot should detect the colours blue, red, yellow and green. Colours should be indicated on the 8x8 pixel display as shown in [this video](https://youtu.be/4foVZdhxUQg).

[Layout drawing colour detection (PDF)](https://github.com/pamasol/Lehrlingsprojekt-Nibo-Burger/files/3652586/Nibo04_V1.0.pdf)

### E) Rabbit warren  
The robot should follow a path from point A to B as shown in [this video](https://youtu.be/VxXwEKlsGJY).

[Layout drawing rabbit warren (PDF)](https://github.com/pamasol/Lehrlingsprojekt-Nibo-Burger/files/3652587/Nibo05_V1.0.pdf)


# 3. Assembling NIBO Burger

How to assemble the robot is described detailly in the manual that is provided with the robot.

* [English Manual NIBO Burger (PDF)](https://github.com/pamasol/Lehrlingsprojekt-Nibo-Burger/files/3652507/Manual_NIBOburger_20150908.pdf)
* [German Manual NIBO Burger (PDF)](https://github.com/pamasol/Lehrlingsprojekt-Nibo-Burger/files/3652506/Doku_NIBOburger_20150909.pdf)
* [Electrical Schematic NIBO Burger (PDF)](https://github.com/pamasol/Lehrlingsprojekt-Nibo-Burger/files/3652510/nibo_burger_schematic_1_09.pdf)

It starts with interpreting and sorting the items followed by preparing the workplace and the soldering station. One should proceed as recommended in the instructions to avoid issues when putting the boards together to a burger.

Following points are not mentioned in the manual but recommended:
* The phototransistors of the IR sensors should be protected with shrinking tube to reduce interference from other sensors.
 
Since the microcontroller is preprogramed on can run the function tests as soon as the hardware work is done.

## 8x8 pixel matrix display

The maroon shield 8x8 matrix display is super easy to assemble. That is why its manual consists of 2 pages only.

* [English Manual maroon shield matrix display (PDF)](https://github.com/pamasol/Lehrlingsprojekt-Nibo-Burger/files/3652517/Anleitung-EN_maroon_SHIELD.pdf)
* [German Manual maroon shield matrix display (PDF)](https://github.com/pamasol/Lehrlingsprojekt-Nibo-Burger/files/3652516/Anleitung_maroon_Shield.pdf)
* [Electrical Schematic maroon shield matrix display (PDF)](https://github.com/pamasol/Lehrlingsprojekt-Nibo-Burger/files/3652520/maroon_shield_1_03_schematic.pdf)


# 4. Programming with AVR Studio and Git

There are several ways to program the NIBO Burger. The easiest way is via online compiler of [roboter.cc](https://www.roboter.cc/), another way is via Arduino IDE or via Atmel Studio. We use AtmelStudio 7.0 in this project.

## Installing software

1. Install latest **NIBO Burger library** from [SourceForge]( https://sourceforge.net/projects/nibo/).
2. Install **AtmelStudio 7.0** from [microchip.com](https://www.microchip.com/mplab/avr-support/atmel-studio-7) website.
3. Install **[WinAVR](https://sourceforge.net/projects/winavr/)** which is a distribution of the avr-gcc compiler that is used on Windows operating systems.
4. Install **Git** from [git-scm.com](https://git-scm.com/downloads).

As soon as the three programmes above update AtmelStudio Settings as described in [this PDF (German only)](https://github.com/pamasol/Lehrlingsprojekt-Nibo-Burger/files/3652530/ProgrammierungdesNIBOBurgersmitAtmelStudio7.0.pdf). The template that has to be imported can be [downloaded here](https://github.com/pamasol/Lehrlingsprojekt-Nibo-Burger/files/3652529/NIBOBurger_Vorlage.zip).

## Using Git and GitHub

Git is the most widely used modern version control system in the world. It tracks changes in source code during software development. It is designed for coordinating work among programmers, but it can be used to track changes in any set of files. Its setup has to be performed as follows.

1. Generate an SSH key and connect your Git with GitHub as [described here]( https://help.github.com/en/articles/generating-a-new-ssh-key-and-adding-it-to-the-ssh-agent).
2. Ask your Pamasol instructor for adding you to this repository.
3. Clone the project as follows
```
git clone https://github.com/pamasol/Lehrlingsprojekt-Nibo-Burger.git
```
4. Add your changes with
```
git add –all
```
5. Commit changes with
```
git commit -m “here what I did and want to commit”
```
6. Push commits to GitHub
```
git push
```
7. Load latest commits
```
git pull
```


# 5. Documentation

ToDo
