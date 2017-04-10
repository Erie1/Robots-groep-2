//COPYRIGHT GROEP 2


//import libaries
import processing.serial.*;
import controlP5.*;

//Definieer variables.
int ACK  = 0x06;
int NACK = 0x15;

int frames = 0;
boolean ACK_received = false;
boolean NACK_received = false;
boolean headerSend = false;

boolean receivedSomething;
int message = 0;


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

boolean drawArrow = false;


//Maak variable aan voor textinvoer
ControlP5 distanceBox;

//Maak array's aan voor het verzenden van data
int array[] = new int[1];    //Controldata
int disDir[] = new int[3];  //Distance and direction data


boolean canSend;
int receivedData;


int ACK_startTime = 0;
boolean canSendDisDir = false;

int someCounter = 0;

//Begin programma.
void setup(){
  //Setup the canvas
  size(400,875);
  
  //Definieer snelheid voor control datqa
  frameRate(10);
  
  
  //Tegen op de canvas.
  drawBackground();
  drawEmptyKeys();
  drawSpeedMeter(6);
  drawDirectionBox();
  
  //maak font
  PFont font1 = createFont("arial", 30);
  
  //Initialize inputBox 
  distanceBox = new ControlP5(this);
  distanceBox.addTextfield(" ").setPosition(50, 480).setSize(300, 30).setFont(font1).setValue(100);
  
  
  textSize(20);
  
  //Open serial communcation on first open port
  
  //Begin communicatie op de laaste poort
  //String portName = "COM4";
  String portName = Serial.list()[0]; 
  
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
 //<>//


 //<>//
void draw(){
    //<>//
  println("---------------------------------------------"); //<>//
  
  //Krijg de afstand. //<>// //<>// //<>// //<>// //<>// //<>// //<>//
  afstand = int(distanceBox.get(Textfield.class, " ").getText());
  distanceBox.get(Textfield.class, " ").isFocus();
 
  //Als de richting, afstand en snelheid verstuurd kan worden, doe dat. //<>// //<>// //<>// //<>//
  if(canSendDisDir){
    canSendDisDir = false;
    println("Attempting to send distance, direction and speed");
    //Set afstand, richting en snelheid in de arraay
    disDir[0] = directionToDegrees(staticArrowX, staticArrowY);
    disDir[1] = 100;
    if(afstand > 255){
      afstand = 255;
    }
    canSend = true;
    disDir[2] = afstand;
    //Verstuur (command + groote + data);
    if(!verzend(0x40, disDir)){
      println("Something went wrong");
    }
  //Als af, dis, richt niet kan verstuur worden, vestuur besturingsdata.  
  }else{
    //Krijg key input.
    array[0] = keysToNumber();
    println("Array length: "+array.length);
    println("Array[0]: "+array[0]);
    
    //Verstuur control data.
    canSend = true; 
    println("Attempting to send control data");
    boolean send;
    do{
      send = verzend(0x20, array);
    }while(!send);
  }
  
  println("Frame: "+frames++); //<>//
  
  //Teken de huidige canvas.
  drawBackground(); //<>//
  drawKeys();
  drawSpeedMeter(343);
  checkMovedAndClicked(); //<>//
  drawValues(); //<>//
  drawSendButton();
 
  
}

//Functie voor tekenen van verzend knop
void drawSendButton(){
  fill(0,255,0);
  rect(50, 510, 300, 40);
  fill(255);
  text("Uitvoeren", 60, 545);

}

//Kijkt bij een muisklik of er op de knop is geklikt.
boolean isButton(){
  if(mouseX > 50 && mouseX < 350 && mouseY > 510 && mouseY < 550){
    return true;
  }else{
    return false;
  }
}

//Wacht voor een ACK van de arduino, met een timemout van 
boolean waitForMessage(){
  //Set startimer
  ACK_startTime = millis();
  //zolang er nog geen ack is
  while(!ACK_received){
   //Kijkt of er een timeout is.
    if((ACK_startTime+5000)<millis()){
     println("ACK Timed out");
     canSend = false;
     return false;
   }
  }
  ACK_received = false;
  //println("Received ACK");
  startTimer = millis();
  return true;
}

boolean verzend(int header, int data[]){
  // check if data amount fits in size bitmask
  if(data.length > 63) return false; 
  header += data.length;
  
  if(canSend)
    myPort.write(header);
  else
    return false;
   
  for(int msg = 0; msg < data.length; msg++){
     if(canSend)
        myPort.write(data[msg]);
     else
        return false;
  }
  return waitForMessage();
}

//Wordt uitgevoerd wanneer er data wordt ontvagen via de seriele poort.
void serialEvent(Serial test){
  //Set data in variable
  receivedData = test.read();
  //Kijk wat er ontvangen is
  if(receivedData == 6){
     //Als er een een ack is ontvangen, zorg dat er geen nieuwe dingen worden verstuurd worden zodat de arduino niet in de war raakt.
     ACK_received = true;
     canSend = false;
     println("ACK received on serial");
  }else if(receivedData == 0x15){
    println("NACK received");
  }else{
    //Als waarde geen ack of nack is, print het.
    print((char)receivedData);
  }
}

//Wordt aangeroepen wanneer de muis is bewogen //<>//
void checkMovedAndClicked(){
  if(clickedOnDirection){
    //Teken pijl
    drawArrow(directionOffsetX+200, directionOffsetY + 200, staticArrowX, staticArrowY, 255,0,0);
  }
  //Als er bewogen is in de cirkel, tken de juiste pijlen en breken de snelheid.
  if(movedInsideDirection){
    if(drawArrow){
      drawArrow(directionOffsetX+200, directionOffsetY + 200, endArrowX, endArrowY, 0,0,0);
      drawSpeed = (int)ceil(0.6666666666666666666666666*distance(endArrowX - (directionOffsetX+200), endArrowY-(directionOffsetY + 200)));
      if(drawSpeed > 100){
        drawSpeed = 100;
      }
    }
  }
}

//Print de waardes onder de circel
void drawValues(){
  text("Direction: "+directionToDegrees(staticArrowX, staticArrowY)+"°   ("+directionToDegrees(endArrowX, endArrowY)+"°) ", directionOffsetX + 50, directionOffsetY + 400);  
  text("Snelheid:  "+newSpeed + "%  ("+drawSpeed+"%)", directionOffsetX + 50, 430); 
  text("Afstand: " + afstand + " cm", 50, 460);
  //text(millis(), 400, 400);
}

//Maak een circle aan voor de richting.
void drawDirectionBox(){
  strokeWeight(0);
  //Verplaats de coordinatengrid naar de juiste positie
  translate(directionOffsetX, directionOffsetY);
  
  fill(255,255,255);
  
  ellipse(200, 200, 300, 300);
  arc(200,200,300, 300, 0, TWO_PI,CHORD);
  //Verplaats coordinatengrid terug.
  translate(-directionOffsetX, -directionOffsetY);
}


//Teken de cirkel.
void drawSpeedMeter(int speed){
  strokeWeight(0);
  fill(255,255,255);
  ellipse(speedOffsetX+200, speedOffsetY+200, 310,310);
  
}

//Maak de achterrgrond blauw.
void drawBackground(){
  fill(13,36,73);
  strokeWeight(0);
  rect(0,0,xSize, ySize); 
  
  
}

//Teken grijze knoppen
void drawEmptyKeys(){
  strokeWeight(0);
  fill(100,100,100);
  rect(inpOffsetX+150, inpOffsetY+25, 100, 100);
  rect(inpOffsetX+25, inpOffsetY+150, 100, 100);
  rect(inpOffsetX+150, inpOffsetY+150, 100, 100);
  rect(inpOffsetX+275, inpOffsetY+150, 100, 100);
}
 
 
//Wordt uitegevooer wanneer een knop worst ingedrukt, zet de waarde in een array die de knop representeerd op true,
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

//Wordt uitgevoerd wanneer een knop wordt losgelaten, zet de waarde die het represteteerd in de array op false.
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

//Functie voor het berekenden van afstand. 
int distance(int x,int y){
  //println(sqrt((x*x)+(y*y)));
  return (int)sqrt((x*x)+(y*y));
}


//Wanneer de muis wordt ingedrukt wordt dit uitgevoerd
void mouseClicked(){
  //Check if mouse has been moved in direction options
  //Als de afstand in de circle past, 
  if(distance(directionOffsetX+200-mouseX, directionOffsetY+200-mouseY ) < 155){
     //Set de snelehdid
     newSpeed = (int)ceil(0.66666666666666666666666*distance(directionOffsetX+200-mouseX, directionOffsetY+200-mouseY));
     if(newSpeed > 100){
       newSpeed = 100;
     }
     //Zorgt dat de rode pijl verplaastst wordt
     staticArrowX= endArrowX;
     staticArrowY = endArrowY;
     //Set variable als er in de circel wordt geklikt.
     clickedOnDirection = true;
  }
  //Als de knop is ingedrukt, kan de richting wroden versturd.
  if(isButton()){
    canSendDisDir = true;
  }
}


//Wordt uitgevoerd als de muis bewogen wordg.
void mouseMoved(){
  //Check if mouse has been moved in direction options
  //Als de beweging in de, cicel is, verplaats de pijl
  if(distance(directionOffsetX+200-mouseX, directionOffsetY+200-mouseY ) < 155){
    endArrowX = mouseX;
    endArrowY = mouseY;
    drawArrow = true;
    movedInsideDirection = true;
    //rect(0,0,20,20);
  }else{
     drawArrow = false;
  }
}

//Functie voor het tekenen van pijl.
void drawArrow(int xStart, int yStart,int xEnd,int yEnd,int R,int G,int B){
  strokeWeight(3);
  
  stroke(R, G, B);
  line(xStart, yStart, xEnd, yEnd);
  float rotationVec = atan2((float)(xStart - xEnd), (float)(yStart- yEnd));
  float leftRotationVec = rotationVec - (0.65*PI);
  float rightRotationVec = rotationVec + (-0.35*PI);
  
  line(xEnd, yEnd, xEnd + cos(-leftRotationVec)*25, yEnd + sin(-leftRotationVec)*25);
  line(xEnd, yEnd, xEnd + cos(-rightRotationVec)*25, yEnd + sin(-rightRotationVec)*25);
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
  //println("Data to send                                                             "+dataToSend);
  return dataToSend;
}