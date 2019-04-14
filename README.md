## Versions
- Processing tested with version 3.5.3
- Arduino tested with software version 1.8.5 and the arduino Uno board

## libraries that need to be installed
### Arduino
- Adafruit motor shield V2 library
- AccelStepper library

### Processing
- ControlP5 library
- The MidiBus library

## Start
Once you have installed all necessary libraries and connect the motors to the board, upload the arduino code onto the arduino board and run the processing application.
CAREFUL: please check if the correct serial port has been selected in the processing application. Check the console for the listed available ports and go to the constructor of the "ArduinoSerial" class in order to change the port index. 

String portName = Serial.list()[3]; // 0,1,2 or 3 or ...

## hardware connections

Given the following direction of the arduino shield.

				  -----------------
arduino cable --- |				  |
				  |				  |
				  |				  |
				  |				  |
				  -----------------
	

### servos
From left to right: brown/red/orange
Top pin number(at the edge of the board): 10
bottom pin number: 9

### steppers (Nema steppers)

For M1-M2 connection -> From top to bottom: black, green, red and blue
For M3-M4 connection -> From top to bottom: blue, red, green and black

## power
The 5V powered from the arduino usb cable is enough for servos and the steppers(that I used).

## good to know
If you want to upload the arduino code again to the board, do not forget to stop running the processing application, because we first need to close the serial port.

## further documentation
### adafruit motor shield tutorial
https://learn.adafruit.com/adafruit-motor-shield-v2-for-arduino/overview

### Accel library used to control the steppers
https://www.airspayce.com/mikem/arduino/AccelStepper/index.html