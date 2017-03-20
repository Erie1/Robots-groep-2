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
		if(data_flag){
			//writeInteger(data_ont[0], 10);
			//usartToMotors([0]);
			
			usartToMotors(data_ont[0]);
			data_flag = FALSE;
		}
		
	 }
 
 }

 /************************************************************************/
 /* initializes i2c communication to Arduino                             */
 /************************************************************************/
 
 void initCommunication(){
	// TODO
 }
 
 
 
 //Initialiseren van usart verbinding met pc voor directe besturing
 void initUsartPC(){
	
	/* Set baud rate */
	UCSRC = 0;
	UBRRH = 0;
	UBRRL = 103;
	/* Enable receiver and transmitter */
	UCSRB = (1<<RXEN)|(1<<TXEN);
	/* Set frame format: 8data, 2stop bit */
	UCSRC =(0<<URSEL)|(3<<UCSZ0);
 }
 
 
 void usartToMotors(uint8_t leftOver){
	if(leftOver == 'w') {leftTarget += 2 * speed; rightTarget += 2 * speed; }
	if(leftOver == 's') {leftTarget -= 2 * speed; rightTarget -= 2 * speed;}
	if(leftOver == 'a') { leftTarget -= speed; rightTarget += speed; }
	if(leftOver == 'd') { leftTarget += speed; rightTarget -= speed; }
	if(leftOver == 'e') {leftTarget = 0; rightTarget = 0;}
	rightDesiredSpeed = rightTarget;
	leftDesiredSpeed = leftTarget;
}
 
 /*
 void usartToMotors(uint8_t leftOver){
	if(leftOver == 'w') {leftTarget += 2 * speed; rightTarget += 2 * speed; }
	if(leftOver == 's') {leftTarget -= 2 * speed; rightTarget -= 2 * speed;}
	if(leftOver == 'a') { leftTarget -= speed; rightTarget += speed; }
	if(leftOver == 'd') { leftTarget += speed; rightTarget -= speed; }
	if(leftOver == 'e') {leftTarget = 0; rightTarget = 0;}
	rightDesiredSpeed = rightTarget;
	leftDesiredSpeed = leftTarget;
}
 
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
 
 
 ISR(TWI_vect) {
	 slaaftwi();
 }
 
 ISR(USART_RXC_vect){
	  
}


/*slave heeft data ontvangen van de master
 data[] een array waarin de ontvangen data staat
 tel het aantal bytes dat ontvangen is*/
 
void ontvangData(uint8_t data[],uint8_t tel){
	for(int i=0;i<tel;++i)
	    data_ont[i]=data[i];
	data_flag = TRUE;
	
}

/* het byte dat de slave verzend naar de master
in dit voorbeeld een eenvoudige teller
*/

uint8_t verzendByte() {
		return 0x22;
}

