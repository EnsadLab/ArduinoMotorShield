import processing.serial.*; // serial library to commincate with ardunino
import controlP5.*;  // GUI library
import themidibus.*; // MIDI library


ControlP5 cp5; // GUI library
Gui gui; // GUI
ArduinoSerial arduinoSerial; // Connection with arduino


// comment to delete... test
void setup()
{
  size(650,650);
  
  // initialise
  cp5 = new ControlP5(this);
  gui = new Gui();
  gui.setup();
  arduinoSerial = new ArduinoSerial(this); // check on the console if serial port has been detected correctly
  
  // show all midi ports
  listMidiDevices(); 
  openMidi("SLIDER/KNOB","CTRL"); // check on the console if midi port has been selected correctly
}


void draw() 
{
  background(255);
  arduinoSerial.listen();
  gui.drawSelectPort();
}


void exit()
{
  println("EXIT");  
  super.exit();  
}
