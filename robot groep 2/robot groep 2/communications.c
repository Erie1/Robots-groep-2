/*
 * communications.c
 *
 * Created: 19-3-2017 09:14:28
 *  Author: Erik
 */ 

#include "rp6aansluitingen.h"
#include "communications.h"
#include "MotorControl.h"
#include "../../shared/twi_codes.h"
#include "i2c.h"
#include <stdint.h>
#include <util/twi.h>
#include <avr/interrupt.h>

uint8_t data_ont[20]; //max 20
volatile uint8_t data_flag;
volatile uint8_t databyte;

void usartToMotors(uint8_t leftOver);
void sonar(uint8_t data[], uint8_t tel);

void initCommunication(){
	data_flag = FALSE;
	databyte = 0x00;
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
}

/************************************************************************/
/* de functie om byte mee te verzenden                                  */
/************************************************************************/
uint8_t verzendByte() {
	return 0x22;
}

/************************************************************************/
/* functie die wordt aangeroepen als er data is ontvangen van de master */
/************************************************************************/ 
void ontvangData(uint8_t data[],uint8_t tel){
	uint8_t description = data[0];
	for(int i = 1; i < tel; ++i)
	    data_ont[i - 1] = data[i];
	data_flag = TRUE;


	switch (description) {
		case CONTROL:
			usartToMotors(data_ont[0]);
			break;
		case SONAR_DIS:
			sonar(data_ont, tel - 1);
			break;
	}
}

/************************************************************************/
/* sets the sonar	                                                    */
/************************************************************************/
void sonar(uint8_t data[], uint8_t tel){
	sonar_dis = atoi((char*)&data);
	if(sonar_dis <= 8.0) emergencyBrake();
}

/************************************************************************/
/* changes desired motorspeeds according to input                       */
/************************************************************************/
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
void usartToMotors(uint8_t leftOver){
	int leftTarget = 0, rightTarget = 0;
	int speed = 75;
	
	if(leftOver == 'w') {leftTarget += 2 * speed; rightTarget += 2 * speed; }
	if(leftOver == 's') {leftTarget -= 2 * speed; rightTarget -= 2 * speed;}
	if(leftOver == 'a') { leftTarget -= speed; rightTarget += speed; }
	if(leftOver == 'd') { leftTarget += speed; rightTarget -= speed; }
	if(leftOver == 'e') {leftTarget = 0; rightTarget = 0;}
	rightDesiredSpeed = rightTarget;
	leftDesiredSpeed = leftTarget;
}

ISR(TWI_vect) {
	writeString("interupt");
	slaaftwi();
}