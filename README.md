# IntroductionToRobotics

This is my repository that will contain all the assignments from the **"Introduction to Robotics"** laboratory at the **Faculty of Mathematics and Computer Science**, **University of Bucharest**. Each assignment will include detailed documentation, with images and videos.

# Homeworks

<details>
<summary><h2> Homework 2</summary>
<br>
This assignment focuses on controlling an RGB LED using three potentiometers (one for each color).



For the implementation, I used:

* An RGB LED
* 3 potentiometers
* 6 resistors of 100 ohms (I connected three in series for each because I only had one 330-ohm resistor)
* 1 resistor of 330 ohms

Photo with first homework:

![Tema 1](./Imagini/tema1.jpg)



This is the video with first homework: https://www.youtube.com/watch?v=EDOw4xzkUVw&ab_channel=IulianMarin
</details>



<details>
<summary><h2> Homework 3</summary>
<br>
This assignment involves simulating a 3-floor elevator control system using
LEDs, buttons, and a buzzer with Arduino.

Components
* LEDs (At least 4: 3 for the floors and 1 for the elevator’s operational
state)
* Buttons
* Buzzer 
* Resistors and wires as needed

This code has functionalities only for the 1st and 3rd floor and has some bugs.The project's functionality is the one shown in the following video.: https://www.youtube.com/shorts/kLPn3MB_2u4

Below is a photo with the assembled homework.

![Tema 2](./Imagini/tema2.jpg)


</details>

<details>
<summary><h2> Homework 4</summary>
<br>
This project involves implementing a 7-segment display using Arduino, where you can 'draw' by lighting up segments one by one.


**General description:** We will use the joystick to control the position of the segment and 'draw' on the display. The movement between segments should be natural, meaning they should transition from the current position only to neighboring segments, without passing through 'walls'.

**Components**:
* 7 segment display (1)
* 1 joystick
* resistors and wires

**Requirement**: The initial position should be on the DP. The current
position always blinks (irrespective of the fact that the segment is on or
off). Use the joystick to move from one position to neighbors (see table for
corresponding movement). Short pressing the button toggles the segment
state from ON to OFF or from OFF to ON. Long pressing the button
resets the entire display by turning all the segments OFF and moving the
current position to the decimal point.
![Tema 4](./Imagini/tema3.jpg)

Link with the video: [here](https://youtube.com/shorts/8ki4eayzJKo)

</details>



<details>
<summary><h2> Homework 5</summary>
<br>
In this project, I created a timer using a 4 digit 7-segment display, 3 buttons (one for start/stop, one for saving laps, and one for reset). To save Arduino pins, I used a 74HC595 shift register.  <br>
<br>

**General description**: Using the 4 digit 7 segment display and 3 buttons,
we should implement a stopwatch timer that counts in 10ths of a second
and has a save lap functionality (similar to most basic stopwatch functions
on most phones).

The **components** I used are:
* 4-digit 7-segment display
* 3 buttons
* 74HC595 shift register
* Resistors and wires


**Requirement**:  The starting value of the 4 digit 7 segment display should
be ”000.0”. The buttons should have the following functionalities:
* Button 1: Start / pause.
* Button 2: Reset (if in pause mode). Reset saved laps (if in lap
viewing mode).
* Button 3: Save lap (if in counting mode), cycle through last saved
laps (up to 4 laps).


**Workflow**:
1. Display shows ”000.0”. When pressing the Start button, the timer
should start.
2. During timer counter, each time you press the lap button, you should
save that timer’s value in memory (not persistent, it is OK to be
deleted upon reset), up to 4 laps (or more if you want); pressing the
5th time should override the 1st saved one. If you press the reset
button while timer works, nothing happens. If you press the pause
button, the timer stops.
3. In Pause Mode, the lap flag button doesn’t work anymore. Pressing
the reset button resets you to 000.0.
4. After reset, you can now press the flag buttons to cycle through the
lap times. Each time you press the flag button, it takes you to the
next saved lap. Pressing it continuously should cycle you through it
continuously. Pressing the reset button while in this state resets all
your flags and takes the timer back to ”000.0”.

![Tema 5](./Imagini/tema5.jpg)


Link with the video: [here](https://www.youtube.com/shorts/0NJ0U_hqhgI)
</details>



<details>
<summary><h2> Homework 6</summary>
<br>
The purpose of this assignment was to implement a menu through Arduino's serial communication to control various parameters of sensors. Specifically, we had to develop a '<i>Smart Environment Monitor and Logger</i>' using Arduino

To accomplish this assignment, I needed the following:
* Ultrasonic Sensor (HC-SR04)
* LDR (Light-Dependent Resistor) aka Photocell aka Photoresistor aka Light
Sensor
* RGB Led

The menu structure is as follows (text taken from the assignment):

<b>1.Sensor Settings:</b>

* 1.1 Adjust Sensors Sampling Interval.
* 1.2 Set Ultrasonic Alert Threshold.
* 1.3 Set LDR (Light Dependent Resistor) Alert Threshold.
* 1.4 Return to the main menu.

<b>2.Reset Logger Data:</b>
* 2.1 Confirm Data Reset (Yes).
* 2.2 Confirm Data Reset (No).

**3.System Status:**
* 3.1 Display Current Sensor Readings.
* 3.2 Display Current Sensor Settings.
        
* 3.3 Display Logged Data.
* 3.4 Return to the main menu.

**4.RGB LED Control:**
* 4.1 Manual Color Control.
* 4.2 Toggle Automatic Mode for LED.
* 4.3 Return to the main menu.


Link with the video: [here](https://www.youtube.com/watch?v=kUFAfjDHYBo)

Photos:

![Tema 6](./Imagini/tema6_1.jpg)
![Tema 6_2](./Imagini/tema6_2.jpg)


</details>


<details>
<summary><h2> Homework 7</summary>
<br>
For this homework, I implemented a <i>mini matrix game</i> using Arduino. The game must have at least 3 types
of elements: <b>player</b> (blinks slowly), <b>bombs</b> (blinks fast), <b>wall</b> (doesn’t
blink).

The created mini-game resembles Bomberman. A player moves on the map (using a joystick), trying to break all the walls using a bomb. The time during which the player can move away from the bomb from the moment of placement is 3 seconds.If the player succeeds to destroy all the walls, he will win. If the player is within the bomb's blast radius during detonation, he will lose the game.

**Components Required:**

* Arduino Uno
* Matrix 8x8
* Joystick
* Driver Max7219
* Resistors and capacitors as needed

Link with the video: [here](https://www.youtube.com/watch?v=4rCxRH2-8Ak)


![Tema 7](./Imagini/tema7.jpg)

</details>


<details>
<summary><h2> Homework 8</summary>
<br>
For this homework, I implement a "mini" LCD Menu for the final project with matrix. This menu contain:

**Introduction**
* Initiate the game and begin at the initial level.

**Menu Options**
* **Start Game** - Initiate the game and begin at the initial level.
*  **Settings** 
1. LCD Brightness Control - Adjust LCD brightness, save the preference for future use.
2. Matrix Brightness Control - Control matrix brightness with a visual display, saving the chosen setting for consistent use.

Link with the video: [here](https://www.youtube.com/watch?v=s7_bhGq-qnM)


![Tema 7](./Imagini/tema8.jpg)

</details>