import themidibus.*;
import processing.serial.*;
import controlP5.*;


PFont fontBig;
PFont fontMiddle;
PFont fontSmall;


class Gui implements ControlListener
{
  int nbMotors = 8;
  Slider[] sliders = new Slider[nbMotors];
  int minValueServo = 0;
  int maxValueServo = 180;
  int minValueStepper = -200;
  int maxValueStepper = 200;
  
  Gui()
  {
    fontBig = createFont("Verdana",16,true); 
    fontMiddle = createFont("Verdana",14,true); 
    fontSmall = createFont("Verdana",12,true); 
    
    cp5.setColorForeground(0xffaa0000);
    cp5.setColorBackground(0xff660000);
    cp5.setFont(fontBig);
    cp5.setColorActive(0xffff0000);
  }
  
  void setup(){
    
    int y = 40;
    cp5.addTextlabel("MOTORS_gui_main")
      .setText("MOTORS")
      .setPosition(50,40)
      .setFont(fontBig);
      
    y = 100;
    for(int i=0; i<8; i++){
      int minv = minValueServo;
      int maxv = maxValueServo;
      if(i == 2 || i == 3) { minv = minValueStepper; maxv = maxValueStepper; }
      // parameters : name, minimum, maximum, default value (float), x, y, width, height
      sliders[i] = cp5.addSlider("SLIDERMOTORVAL_" + i,minv,maxv,0.5*(maxv-minv),250,y,300,30);
      sliders[i].setSliderMode(Slider.FLEXIBLE);
      sliders[i].getValueLabel().align(ControlP5.RIGHT_OUTSIDE, ControlP5.CENTER).setFont(fontMiddle);
      sliders[i].getCaptionLabel().setVisible(false);
      sliders[i].addListener(this);
      
      Textlabel label = cp5.addTextlabel("LABELMOTORVAL_"+i).setText("motor"+i).setPosition(50,y+5).setFont(fontSmall).setColor(color(255,255,255));
      if(i== 0) label.setText("Servo motor(pin 9)");
      else if(i == 1) label.setText("Servo motor(pin 10)");
      else if(i == 2) label.setText("Stepper motor(M1-M2)");
      else if(i == 3) label.setText("Stepper motor(M3-M4)");
      else if(i == 4) label.setText("DC motor(M1)");
      else if(i == 5) label.setText("DC motor(M2)");
      else if(i == 6) label.setText("DC motor(M3)");
      else if(i == 7) label.setText("DC motor(M4)");
      
      y += 50;
    }
   
  }
  
  void setMotorValueFromMidi(int motorIndex, int midiValue)
  {
    int minv = minValueServo;
    int maxv = maxValueServo;
    if(motorIndex == 2 || motorIndex == 3) { minv = minValueStepper; maxv = maxValueStepper; }
    float value = map(midiValue,0,127,minv,maxv);
    if(motorIndex >= 0 && motorIndex < nbMotors)
    {
       sliders[motorIndex].setValue(value); 
    }
  }
  
  void reset(int motorIndex)
  {
    int minv = minValueServo;
    int maxv = maxValueServo;
    if(motorIndex == 2 || motorIndex == 3) { minv = minValueStepper; maxv = maxValueStepper; }
    if(motorIndex >= 0 && motorIndex < nbMotors)
    {
       sliders[motorIndex].setValue(0.5*(maxv-minv)); 
    } 
  }
  
  void resetAll()
  {
    for(int i=0; i<sliders.length; i++)
    {
       int minv = minValueServo;
       int maxv = maxValueServo;
       if(i == 2 || i == 3) { minv = minValueStepper; maxv = maxValueStepper; }
       sliders[i].setValue(0.5*(maxv-minv)); 
    }
  }
 
  void controlEvent(ControlEvent evt)
   {
     if(!evt.isController())
      return;
      
      Controller c = evt.getController();
      String addr = c.getAddress();
      float value = c.getValue();
      String[] params = split(addr,"_");
      
      //println("CONTROL EVENT",addr,value);
      //for(int i=0; i<params.length; i++){ println(params[i]); }
      
      if(addr.startsWith("/SLIDERMOTORVAL")){
        if(params.length >= 2){
          int motorIndex = int(params[1]);
          //println("SLIDER / motor index",motorIndex);
          if(motorIndex == 2 || motorIndex == 3){ value += abs(minValueStepper); }
          arduinoSerial.sendToArduino(motorIndex,int(value));
        }
      }
   }
  
};
