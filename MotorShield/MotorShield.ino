/*************************************************** 
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
  stepperID_2->onestep(FORWARD, MICROSTEP);
}
void backwardstepID_2() {  
  stepperID_2->onestep(BACKWARD, MICROSTEP);
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

bool velocity_mode = false;


// *************************  DEBUG  **************************
// PAS ENCORE FAIT...
// set the value to 1 if you really want to see debug messages in processing 
// #define USE_DEBUG 1

int ledPin = 13; // Set the pin to digital I/O 4


// *************************  ANIMS  **************************
typedef enum Anim_type:byte { RANDOM,SINUS,COM } Anim_type;
Anim_type anim_type = COM; // by default, we listen to the serial port for processing messages
//int fakeValues[] = {0,70,40,90,100,20,170,50,70,20,180}; // tests servos
//int fakeValues[] = {40,140}; // tests servos
int fakeValues[] = {-200,200}; // tests steppers
int nbFakeValues =  (sizeof(fakeValues) / sizeof(fakeValues[0]));
int fakeValueIndex = 0;
int fakeValueIndexStepper = 0;

unsigned long DELAY_TIME = 2000; // 2 seconds
unsigned long delayStart = 0;
bool delayRunning = false;

void setup() {

  pinMode(ledPin, OUTPUT); 
  Serial.begin(9600); // Start serial communication at 9600 bps
  //Serial.println("Arduino is listening");

  AFMShield.begin();

  servoID_0.attach(9); // connection towards the center of the board
  servoID_1.attach(10); // connection on the board edge

  acc_stepperID_2.setMaxSpeed(2000.0); // vitesse ?
  acc_stepperID_2.setAcceleration(1000);
  acc_stepperID_3.setMaxSpeed(2000.0);
  acc_stepperID_3.setAcceleration(1000);

  DELAY_TIME = 1000;//random(1000,3000); // check if not better to use rand(10) * 100 or something like that...
  delayStart = millis();
  delayRunning = true;

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
  // Serial.println(s);
  // servo library needs a number between 0 and 180
  // servoID_0.write(map(i, 0, 255, 0, 180));
  if(id == 0) servoID_0.write(value);
  else if(id == 1) servoID_1.write(value);
}


void runStepper(int id,int value){
  value = value + minValueStepper;
  String s = String("run stepper ") + id + String(" with value ") + value;
  //value = map(value,-200,200,-500,500); // valeur, min, max, minNew, maxNew => pour tester rapidement
  //Serial.println(s);
  if(id == 2){
    if(velocity_mode) acc_stepperID_2.setSpeed(value);
    else acc_stepperID_2.moveTo(value);
  }
  else if(id == 3){
    if(velocity_mode) acc_stepperID_3.setSpeed(value);
    else acc_stepperID_3.moveTo(value);
  }
}

void listenToCom(){

  if (Serial.available() > 1) {
    //Serial.println("********** serial available **********"); // seulement si on arrive vraiment a rien lire dans la console
    //Serial.println('*');
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
    //Serial.println("*");
    //Serial.println("**************************************"); // seulement si on arrive vraiment a rien lire dans la console
  }

}



void updateRandom(){
  if (delayRunning && ((millis() - delayStart) >= DELAY_TIME)) {
    DELAY_TIME = 3000;//random(1000,3000); // check if not better to use rand(10) * 100 or something like that...
    delayStart = millis();
    triggerRandomPos();
    //delayRunning = false; // keep running... uncomment if you want to stop running
  }
}

void triggerRandomPos(){
  
  triggerServos();
  triggerSteppers();

  // ou si on veut updater qu'un moteur à chaque trigger...
  //int motorId = random(4);
  int motorId = 5; // la suite va etre ignoré
  if(motorId == 0) {int value = random(0,180); servoID_0.write(value);}
  else if(motorId == 1) {int value = random(0,180); servoID_1.write(value);}
  if(motorId == 2) {int value = random(400) - 200; acc_stepperID_2.setSpeed(value);}
  else if(motorId == 3) {int value = random(400) - 200; acc_stepperID_3.setSpeed(value);}
  
}


void triggerServos(){
  int value =  fakeValues[fakeValueIndex];
  fakeValueIndex++;
  if(fakeValueIndex >= nbFakeValues){
    fakeValueIndex = 0;  
  }
  servoID_0.write(value);
  servoID_1.write(value);
}


void triggerSteppers(){
  //int value = random(400) - 200;
  int value =  fakeValues[fakeValueIndexStepper];
  fakeValueIndexStepper++;
  if(fakeValueIndexStepper >= nbFakeValues){
    fakeValueIndexStepper = 0;  
  }
  if(velocity_mode){
    acc_stepperID_2.setSpeed(value);
    acc_stepperID_3.setSpeed(value);
  }else{
    acc_stepperID_2.setSpeed(value);
    acc_stepperID_3.setSpeed(value);
    acc_stepperID_2.moveTo(value);
    acc_stepperID_3.moveTo(value);
  }


}


void loop() {

  if(anim_type == COM){
    listenToCom();
  }else if(anim_type == RANDOM){
    updateRandom();
  }

  if(velocity_mode){
    acc_stepperID_2.runSpeed();
    acc_stepperID_3.runSpeed(); 
  }else{
    acc_stepperID_2.run();
    acc_stepperID_3.run();
  }

}


