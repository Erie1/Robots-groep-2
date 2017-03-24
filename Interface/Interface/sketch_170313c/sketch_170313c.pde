//COPYRIGHT GROEP 2

import processing.serial.*;
import controlP5.*;


int ACK  = 0x06;
int NACK = 0x15;

boolean ACK_received = false;
boolean NACK_received = false;
boolean headerSend = false;


Serial myPort;  // Create object from Serial class
String val;     // Data received from the serial port

boolean[] keyList = {false, false, false, false};
int xSize = 800;
int ySize = 875;

int newSpeed = 0;
int drawSpeed = 0;

int inpOffsetX = 0;
int inpOffsetY = 600;
int speedOffsetX = 0;
int speedOffsetY = 0;
int directionOffsetX = 0;
int directionOffsetY = 0;

int endArrowX = 200+directionOffsetX;
int endArrowY = 200+directionOffsetY;

int staticArrowX = 200+directionOffsetX;
int staticArrowY = 200+directionOffsetY;

boolean clickedOnDirection = false;
boolean movedInsideDirection = false;

boolean newCommand = true;
int afstand = 0;

int startTimer = 0;

ControlP5 distanceBox;




void setup(){
  //Setup the canvas
  size(400,875);
  
  //Draw first assets on the canvas
  drawBackground();
  drawEmptyKeys();
  drawSpeedMeter(6);
  drawDirectionBox();
  
  //Font
  PFont font1 = createFont("arial", 30);
  
  //Initialize inputBox 
  distanceBox = new ControlP5(this);
  distanceBox.addTextfield(" ").setPosition(50, 480).setSize(300, 30).setFont(font1).setValue(100);
  
  
  textSize(20);
  
  //Open serial communcation on first open port
  
  String portName = "COM8";
  //String portName = Serial.list()[(Serial.list().length-1)]; 
  
  if(Serial.list().length > 0){
    myPort = new Serial(this, portName, 9600);
  }else{
    println("\n\n Er zijn geen poorten om mee te verbinden");
    exit();
  }
  
  //Print the port to make sure it's the right one
  println(portName);
  //Wait 4 seconds to make sure it's all ready.
  delay(4000);
}




void draw(){
   //
  //
  
  
  
  if((startTimer+1000) < millis()){ //<>//
    println("retrying connection");
      headerSend = false;
      startTimer = millis(); //<>//
  }  
     //<>//
   //<>//
  afstand = int(distanceBox.get(Textfield.class, " ").getText());
  distanceBox.get(Textfield.class, " ").isFocus();
   //<>//
  //Krijg de afstand. //<>//
  //afstand = distance; //<>//
  //clear(); //<>//
  
  drawBackground();
  drawKeys();
  drawSpeedMeter(343);
  checkMovedAndClicked();
  drawValues(); //<>//
 
  
}

 //<>//
void serialEvent(Serial test){ //<>//
   int receivedData = test.read();
   if(receivedData == ACK){
     ACK_received = true;
     startTimer = millis();
   }else if(receivedData == NACK){
     NACK_received = true;
     startTimer = millis();
   }else{
     print((char)receivedData);
   }
   
   
   
  if(!headerSend){
    myPort.write(0x21); //<>//
    headerSend = true; //<>//
  }else if(ACK_received) {
    println("ack received");
    myPort.write(keysToNumber()); //<>// //<>//
    headerSend = false;
    ACK_received = NACK_received = false;
  } else if(NACK_received) {
    println("nack received");
    headerSend = false;
    ACK_received = NACK_received = false;      
  } else{
    println("waiting");
  }
  //while(!newCommand){}
}

void checkMovedAndClicked(){
  if(clickedOnDirection){
    drawArrow(directionOffsetX+200, directionOffsetY + 200, staticArrowX, staticArrowY, 255,0,0);
  }
  if(movedInsideDirection){
    drawArrow(directionOffsetX+200, directionOffsetY + 200, endArrowX, endArrowY, 0,0,0);
    drawSpeed = (int)ceil(0.6666666666666666666666666*distance(endArrowX - (directionOffsetX+200), endArrowY-(directionOffsetY + 200)));
  }
}

void drawValues(){
  text("Direction: "+directionToDegrees(staticArrowX, staticArrowY)+"°   ("+directionToDegrees(endArrowX, endArrowY)+"°) ", directionOffsetX + 50, directionOffsetY + 400);  
  text("Snelheid:  "+newSpeed + "%  ("+drawSpeed+"%)", directionOffsetX + 50, 430); 
  text("Afstand: " + afstand + " cm", 50, 460);
  //text(millis(), 400, 400);
}
void drawDirectionBox(){
  strokeWeight(0);
  translate(directionOffsetX, directionOffsetY);
  
  fill(255,255,255);
  
  ellipse(200, 200, 300, 300);
  arc(200,200,300, 300, 0, TWO_PI,CHORD);

  translate(-directionOffsetX, -directionOffsetY);
}



void drawSpeedMeter(int speed){
  strokeWeight(0);
  fill(255,255,255);
  ellipse(speedOffsetX+200, speedOffsetY+200, 300,300);
  
}


void drawBackground(){
  fill(13,36,73);
  rect(0,0,xSize, ySize); 
  
  
}

void drawEmptyKeys(){
  strokeWeight(0);
  fill(100,100,100);
  rect(inpOffsetX+150, inpOffsetY+25, 100, 100);
  rect(inpOffsetX+25, inpOffsetY+150, 100, 100);
  rect(inpOffsetX+150, inpOffsetY+150, 100, 100);
  rect(inpOffsetX+275, inpOffsetY+150, 100, 100);
}
  
void keyPressed(){
  if(distanceBox.get(Textfield.class, " ").isFocus()){}else{
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

int distance(int x,int y){
  //println(sqrt((x*x)+(y*y)));
  return (int)sqrt((x*x)+(y*y));
}

void mouseClicked(){
  //Check if mouse has been moved in direction options
  if(distance(directionOffsetX+200-mouseX, directionOffsetY+200-mouseY ) < 150){
     newSpeed = (int)ceil(0.66666666666666666666666*distance(directionOffsetX+200-mouseX, directionOffsetY+200-mouseY));
     staticArrowX= endArrowX;
     staticArrowY = endArrowY;
     clickedOnDirection = true;
  }
}

void mouseMoved(){
  //Check if mouse has been moved in direction options
  if(distance(directionOffsetX+200-mouseX, directionOffsetY+200-mouseY ) < 150){
    endArrowX = mouseX;
    endArrowY = mouseY;
    movedInsideDirection = true;
    //rect(0,0,20,20);
  }else{
   
  }
  //println(distance(directionOffsetX+200-mouseX, directionOffsetY+200-mouseY ));
}

void drawArrow(int xStart, int yStart,int xEnd,int yEnd,int R,int G,int B){
  strokeWeight(3);
  
  stroke(R, G, B);
  line(xStart, yStart, xEnd, yEnd);
  float rotationVec = atan2((float)(xStart - xEnd), (float)(yStart- yEnd));
  float leftRotationVec = rotationVec - (0.65*PI);
  float rightRotationVec = rotationVec + (-0.35*PI);
  
  line(xEnd, yEnd, xEnd + cos(-leftRotationVec)*25, yEnd + sin(-leftRotationVec)*25);
  line(xEnd, yEnd, xEnd + cos(-rightRotationVec)*25, yEnd + sin(-rightRotationVec)*25);
  
  //println(rotationVec);
  
  
  
  
  
}

int directionToDegrees(int xCoord,int yCoord){
  int degrees = 0;
  
  float rotationVec = atan2((float)((directionOffsetX + 200)-(xCoord)), (float)((directionOffsetY + 200)-(yCoord)));
  rotationVec = (-rotationVec);
  //println(rotationVec);
  degrees = (int)ceil(rotationVec * 57.295795); 
  return degrees;
}

void drawKeys(){
  strokeWeight(0);
  if(keyList[0]){
    fill(0,255,0);
    
  }else{
    fill(255,0,0);
  }
  rect(inpOffsetX+150, inpOffsetY+25, 100, 100);
  if(keyList[1]){
    fill(0,255,0);
  }else{
    fill(255,0,0);
  }
  rect(inpOffsetX+25, inpOffsetY+150, 100, 100);
  
  if(keyList[2]){
    fill(0,255,0);
  }else{
    fill(255,0,0);
  }
  rect(inpOffsetX+150, inpOffsetY+150, 100, 100);
  
  if(keyList[3]){
    fill(0,255,0);
  }else{
    fill(255,0,0);
  }
  rect(inpOffsetX+275, inpOffsetY+150, 100, 100);
}


int keysToNumber(){
  int dataToSend = 128;
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
  return dataToSend;
}