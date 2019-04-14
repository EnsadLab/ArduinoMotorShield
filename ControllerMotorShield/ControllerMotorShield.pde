import processing.serial.*; // serial library to commincate with ardunino
import controlP5.*;  // GUI library
import themidibus.*; // MIDI library


ControlP5 cp5; // GUI library
Gui gui; // GUI
ArduinoSerial arduinoSerial; // Connection with arduino


void setup()
{
  size(700,550);
  
  // initialise
  cp5 = new ControlP5(this);
  arduinoSerial = new ArduinoSerial(this); // check on the console if serial port has been detected correctly
  gui = new Gui();
  gui.setup();
  
  // show all midi ports
  listMidiDevices(); 
  openMidi("SLIDER/KNOB","CTRL"); // check on the console if midi port has been selected correctly
}


void draw() 
{
  background(0);
  arduinoSerial.listen();
}


void exit()
{
  println("EXIT");  
  super.exit();  
}
