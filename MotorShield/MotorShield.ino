/*************************************************** 
  This is an example for our Adafruit 16-channel PWM & Servo driver
  Servo test - this will drive 16 servos, one after the other

  Pick one up today in the adafruit shop!
  ------> http://www.adafruit.com/products/815

  These displays use I2C to communicate, 2 pins are required to  
  interface. For Arduino UNOs, thats SCL -> Analog 5, SDA -> Analog 4

  Adafruit invests time and resources providing this open source code, 
  please support Adafruit and open-source hardware by purchasing 
  products from Adafruit!

  Written by Limor Fried/Ladyada for Adafruit Industries.  
  BSD license, all text above must be included in any redistribution
 ****************************************************/

#include <AccelStepper.h>
#include <Wire.h>
#include <Adafruit_MotorShield.h>
#include "utility/Adafruit_MS_PWMServoDriver.h"
#include <Adafruit_PWMServoDriver.h> // servos shield - not used for now
#include <Servo.h> 

// Adafruit Motor shield
Adafruit_MotorShield AFMShield(0x60); // Default address, no jumpers

// Adafruit 16-servos shield
// Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(); // not used for now


// ************************  servos  **************************
Servo servoID_0;
Servo servoID_1;


// ***********************  steppers  *************************
Adafruit_StepperMotor *stepperID_2 = AFMShield.getStepper(200, 1);
Adafruit_StepperMotor *stepperID_3 = AFMShield.getStepper(200, 2);
// you can change these to SINGLE, DOUBLE, INTERLEAVE or MICROSTEP!
void forwardstepID_2() {  
  stepperID_2->onestep(FORWARD, DOUBLE);
}
void backwardstepID_2() {  
  stepperID_2->onestep(BACKWARD, DOUBLE);
}
void forwardstepID_3() {  
  stepperID_3->onestep(FORWARD, DOUBLE);
}
void backwardstepID_3() {  
  stepperID_3->onestep(BACKWARD, DOUBLE);
}
// Now we'll wrap the 2 steppers in an AccelStepper object
AccelStepper acc_stepperID_2(forwardstepID_2, backwardstepID_2);
AccelStepper acc_stepperID_3(forwardstepID_3, backwardstepID_3);
int minValueStepper = -200;
int maxValueStepper = 200;
// ************************************************************

int ledPin = 13; // Set the pin to digital I/O 4


void setup() {

  pinMode(ledPin, OUTPUT); 
  Serial.begin(9600); // Start serial communication at 9600 bps
  Serial.println("Arduino is listening");

  AFMShield.begin();

  servoID_0.attach(9); // connection towards the center of the board
  servoID_1.attach(10); // connection on the board edge

  acc_stepperID_2.setMaxSpeed(500.0); // la vitesse max est sans doute moins...

  acc_stepperID_3.setMaxSpeed(500.0);

}


void processDatas(int data[], int dataLength){
  
  // according to the given ID, run the appropriate motor
  for(int i=0; i<dataLength; i += 2){
    int id = data[i];
    int value = 0;
    if( (i+1) < dataLength) value = data[i+1];
    //Serial.println(id);
    //Serial.println(value);
    if(id == 0 || id == 1){
      runServo(id,value);
    }else if(id == 2 || id == 3){
      runStepper(id,value);  
    }
  }
  
}


void runServo(int id,int value){
  String s = String("run servo ") + id + String(" with value ") + value;
  Serial.println(s);
  // servo library needs a number between 0 and 180
  // servoID_0.write(map(i, 0, 255, 0, 180));
  if(id == 0) servoID_0.write(value);
  else if(id == 1) servoID_1.write(value);
}


void runStepper(int id,int value){
  value = value + minValueStepper;
  String s = String("run stepper ") + id + String(" with value ") + value;
  Serial.println(s);
  if(id == 2) acc_stepperID_2.setSpeed(value);
  else if(id == 3) acc_stepperID_3.setSpeed(value);
}


// listens for messages coming from processing
void loop() {

  if (Serial.available() > 1) {
    //Serial.println("********** serial available **********");
    bool wait = true;
    int count = 0;
    int dataLength = 3;
    int dataIn[2];
    int value;
    while(wait) { // stay in this loop until newline is read
      if(Serial.available()) {
        //dataIn[count] = Serial.read();
        value = Serial.read();
        if (value == '\n') {
          wait = false; // exit while loop
        }else{
          //Serial.println(count);
          //Serial.println(dataIn[count]);
          if(count == 0) dataIn[0] = value;
          else if(count == 1) dataIn[1] = value<<8;
          else if(count == 2) dataIn[1] += value;
        }
        count++;
      }
    }  
    
    processDatas(dataIn,2);
    //Serial.println("**************************************");
  }

  acc_stepperID_2.runSpeed();
  acc_stepperID_3.runSpeed();
  

}
