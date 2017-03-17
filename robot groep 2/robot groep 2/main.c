/*
 * robot groep 2.c
 *
 * Created: 9-3-2017 20:00:45
 * Author : Erik
 */ 
 
 #include "rp6aansluitingen.h"
 #include "i2c.h"
 #include "MotorControl.h"
 #include <stdint.h>
 #include <stdlib.h>
 #include <avr/io.h>
 #include <util/twi.h>
 #include <avr/interrupt.h>

 #define TRUE 0xFF;
 #define FALSE 0;

 void usartToMotors(uint8_t leftOver);
 
 // communication functions
 void initCommunication();
 void ontvangData(uint8_t data[],uint8_t tel);
 uint8_t verzendByte();
 
 // global variables
 uint8_t data_ont[20]; //max 20
 volatile uint8_t data_flag = FALSE;
 volatile uint8_t databyte=0x33;


 int main(void)
 {
	 initMotors();
	 //initCommunication();
	 initUSART();
	 writeString("Passed usart \n\r");
	
	init_i2c_slave(8);
	writeString("Passed slave init \n\r");
	
	/*ontvangData is de functie die uitgevoerd wordt 
	wanneer een byte via de i2c bus ontvangen wordt
	*/
	init_i2c_ontvang(ontvangData); 
	writeString("Passed ontvangData init \n\r");
	
	/*verzendByte is de functie die aangeroepen wordt
	wanneer de slave een byte naar de master verzend*/
	init_i2c_verzend(verzendByte);
	writeString("Passed verzendByte \n\r");
	
	sei(); //De slave van i2c werkt met interrupt
	writeString("TESTESTESTET");
	//usartToMotors(0x84);
	
	
	 while(1){
		
		usartToMotors(1);
		if(data_flag){
			//writeInteger(data_ont[0], 10);
			writeString("\n\r");
			
			
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
	UCSRC =
	 (0<<URSEL)|(3<<UCSZ0);
 }
 
 
 
 
 void usartToMotors(uint8_t leftOver){
	int leftTarget = 0, rightTarget = 0;
	int speed = 100;
	if(leftOver & 1) { leftTarget += speed; rightTarget -= speed; }
	if(leftOver & 2) { leftTarget -= speed; rightTarget -= speed;}
	if(leftOver & 4) { leftTarget -= speed; rightTarget += speed; }
	if(leftOver & 8) { leftTarget += speed; rightTarget += speed; }
	leftDesiredSpeed = leftTarget;
	rightDesiredSpeed = rightTarget;

	/*
	//int leftOver;
	 int wPressed = 0;
	 int aPressed = 0;
	 int sPressed = 0;
	 int dPressed = 0;
	 int voorAchterRichting;
	 int stuurRichting;
	 
	 leftOver -= 128;
	
	 if(leftOver >= 8){
		wPressed = 1;
		leftOver -= 8;
	 }
	 if(leftOver >= 4){
		 aPressed = 1;
		 leftOver -= 4;
	 }
	 if(leftOver >= 2){
		 sPressed =  1;
		 leftOver -= 2;
	 }
	 if(leftOver >= 1){
		 dPressed = 1;
		 
	 }
	if(wPressed & sPressed){
		voorAchterRichting = 0;
	}else if(wPressed){
		voorAchterRichting = 1;
	}else if(sPressed){
		voorAchterRichting = -1;
	}else{
		voorAchterRichting = 0;
	}

	if(aPressed & dPressed){
		stuurRichting = 0;
	}else if(aPressed){
		stuurRichting = -1;
	}else if(dPressed){
		stuurRichting = 1;
	}else{
		stuurRichting = 0;
	}
	
	
	if((voorAchterRichting == 1) & (stuurRichting == 0)) {
		setMotors(200, 200);
	}else if((voorAchterRichting == 0) & (stuurRichting == 1)){
		setMotors(200, -200);
	}else if((voorAchterRichting == 0) & (stuurRichting == -1)){
		setMotors(-200, 200);
	}else if((voorAchterRichting == -1) & (stuurRichting == 0)){
		setMotors(-200, -200);
	}else if((voorAchterRichting == 1) & (stuurRichting == 1)){
		setMotors(200, 100);
	}else if((voorAchterRichting == 1) & (stuurRichting == -1)){
		setMotors(100, 200);
	}else if((voorAchterRichting == -1) & (stuurRichting == 1)){
		setMotors(-200, -100);
	}else if((voorAchterRichting == -1) & (stuurRichting == -1)){
		setMotors(-100, 200);
	}else{
		setMotors(0,0);
	}*/
 }
 
 
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
	usartToMotors(data[0]);
	//writeInteger(10, 10);
}

/* het byte dat de slave verzend naar de master
in dit voorbeeld een eenvoudige teller
*/

uint8_t verzendByte() {
		return 0x22;
}
