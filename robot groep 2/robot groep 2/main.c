/*
 * robot groep 2.c
 *
 * Created: 9-3-2017 20:00:45
 * Author : Erik
 */ 
 
 #include "rp6aansluitingen.h"
 #include "MotorControl.h"
 #include "communications.h"
 #include <stdint.h>
 #include <stdlib.h>
 #include <avr/io.h>
 #include <avr/interrupt.h>
 
 
 
 int main(void)
 {
	initMotors();
	initCommunication();
	
	sei(); //De slave van i2c werkt met interrupt
	

	 while(1){
		
		//usartToMotors(1);
		//if(data_flag){
			//writeInteger(data_ont[0], 10);
			//usartToMotors([0]);
			
			//usartToMotors(data_ont[0]);
			//data_flag = FALSE;
		//}
		
	 }
 
 }

 /************************************************************************/
 /* initializes i2c communication to Arduino                             */
 /************************************************************************/
 
 
 
 
 
 //Initialiseren van usart verbinding met pc voor directe besturing

 
 
 
 
 
 /*
 void usartToMotors(uint8_t leftOver){
	int leftTarget = 0, rightTarget = 0;
	int speed = 75;
	if(leftOver & W_KEY) { leftTarget += 2 * speed; rightTarget += 2 * speed; }
	if(leftOver & S_KEY) { leftTarget -= 2 * speed; rightTarget -= 2 * speed;}
	if(leftOver & A_KEY) { leftTarget -= speed; rightTarget += speed; }
	if(leftOver & D_KEY) { leftTarget += speed; rightTarget -= speed; }
	leftDesiredSpeed = leftTarget;
	rightDesiredSpeed = rightTarget;
 }
 */
 
 
 