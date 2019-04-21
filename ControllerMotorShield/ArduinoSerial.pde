
Serial port;

class ArduinoSerial
{
    
   ControllerMotorShield controller;
   
   ArduinoSerial(ControllerMotorShield controller)
   {
     this.controller = controller;
     String portName = Serial.list()[3]; 
     String[] ports = Serial.list();
     println("Available SERIAL ports:");
     for(int i=0; i<ports.length; i++)
     {
       println(ports[i]);
     }
     
     println("----> Trying to open serial port " + portName);
     port = new Serial(controller, portName, 9600);
     // wait a little bit to be sure it is opened correctly
     delay(1000);
   }
   
   void sendToArduino(int id, int value)
   {
      //println("------> Sending to ARDUINO: Servo ID " + id + " WITH speed " + value);
      
      /*
      byte b[] = {64,32};
      b[0] = byte(id);
      b[1] = byte(value);//byte(value - 200);
      port.clear();
      port.write(b[0]);
      port.write(b[1]);
      */
      
      port.clear();
      port.write(id);
      // value can go above 256 -> we need two bytes to send the value
      int firstByte = value>>8;
      int secondByte = value&0xFF;
      //println("-----> " + firstByte + " " + secondByte);
      port.write(firstByte);
      port.write(secondByte);
      port.write('\n');
      
      //to be checked...
      //Probably better for arduino com but introduces too much latency in the GUI.
      //delay(30);
   }
   
   
   // prints out on the console DEBUGGING messages from Arduino (instead of using the arduino serial monitor)
   void listen()
   {
     
       if ( port.available() > 0) {  // If data is available,
          String msg = port.readStringUntil('\n');
          if(msg != null) 
            print("Arduino received:",msg);
       }
       
   }
    
};
