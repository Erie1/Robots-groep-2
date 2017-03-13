import processing.serial.*;
Serial myPort;  // Create object from Serial class
String val;     // Data received from the serial port

boolean[] keyList = {false, false, false, false};
int dataToSend = 0;

void setup(){

  size(200, 200);
  
  String portName = "COM5"; //change the 0 to a 1 or 2 etc. to match your port
  myPort = new Serial(this, portName, 9600);
}

void keyPressed(){
  if(key == 'w'){
    keyList[0] = true;
  }
  if(key == 'a'){
    keyList[1] = true;
  }
  if(key == 's'){
    keyList[2] = true;
  }
  if(key == 'd'){
    keyList[3] = true;
  }
}

void keyReleased()
{
  if(key == 'w'){
    keyList[0] = false;
  }
  if(key == 'a'){
    keyList[1] = false;
  }
  if(key == 's'){
    keyList[2] = false;
  }
  if(key == 'd'){
    keyList[3] = false;
  }
} 




void draw(){
  dataToSend = 127;
  if(keyList[0]){
    dataToSend += 8;
  }
  if(keyList[1]){
    dataToSend += 4;
  }
  if(keyList[2]){
    dataToSend += 2;
  }
  if(keyList[3]){
    dataToSend += 1;
  }
  
  println(dataToSend);
  
  myPort.write(dataToSend);
  
}