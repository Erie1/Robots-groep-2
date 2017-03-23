import processing.serial.*;
Serial myPort;  // Create object from Serial class
String val;     // Data received from the serial port

boolean[] keyList = {false, false, false, false};
int dataToSend = 0;

void setup(){

  size(400, 275);
  
  String portName = Serial.list()[0]; //change the 0 to a 1 or 2 etc. to match your port
  myPort = new Serial(this, portName, 9600);
  println(Serial.list()[0]);
  delay(5000);
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
  
  dataToSend = 0;
  if(keyList[0]){
    dataToSend += 8;
    fill(0,255,0);
    
  }else{
    fill(255,0,0);
  }
  rect(150, 25, 100, 100);
  if(keyList[1]){
    dataToSend += 4;
    fill(0,255,0);
  }else{
    fill(255,0,0);
  }
  rect(25, 150, 100, 100);
  
  if(keyList[2]){
    dataToSend += 2;
    fill(0,255,0);
  }else{
    fill(255,0,0);
  }
  rect(150, 150, 100, 100);
  
  if(keyList[3]){
    dataToSend += 1;
    fill(0,255,0);
  }else{
    fill(255,0,0);
  }
  rect(275, 150, 100, 100);

  
  
   //println(dataToSend); //<>// //<>//
  myPort.write(0x91);
  myPort.write(dataToSend);

 
 
  //delay(2500);
  
}

void serialEvent(Serial test){
  char temp = (char)test.read();
  if(temp == 0x06) println("ACK received");
  if(temp == 0x15) println("NACK received");
  print(temp);
}