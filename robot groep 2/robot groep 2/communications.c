/*
 * communications.c
 *
 * Created: 19-3-2017 09:14:28
 *  Author: Erik
 */ 
 #define F_CPU 8000000

#include "communications.h"
#include "MotorControl.h"
#include "../../shared/twi_codes.h"
#include "i2c.h"
#include <stdint.h>
#include <util/twi.h>
#include <avr/interrupt.h>

// received data
int8_t data_ont[20]; //max 20

// send and recieve functions
void ontvangData(uint8_t data[],uint8_t tel);
uint8_t verzendByte();

/************************************************************************/
/* initializes communication with pc on usb bus,						*/
/* i2c and the i2c send and recieve functions                           */
/************************************************************************/
void initCommunication(){
	initUSART();
	
	/* initializes the rp6 as a slave on the i2c bus */
	init_i2c_slave(8);

	/*ontvangData is de functie die uitgevoerd wordt 
	wanneer een byte via de i2c bus ontvangen wordt
	*/
	init_i2c_ontvang(ontvangData);
	
	/*verzendByte is de functie die aangeroepen wordt
	wanneer de slave een byte naar de master verzend*/
	init_i2c_verzend(verzendByte);
}

/************************************************************************/
/* vertelt de arduino of er een noodstop is geweest                     */
/************************************************************************/
uint8_t verzendByte() {
	return blocked;
}

/************************************************************************/
/* functie die wordt aangeroepen als er data is ontvangen van de master */
/************************************************************************/ 
void ontvangData(uint8_t data[], uint8_t tel){
	for(int i = 1; i < tel; ++i)
	    data_ont[i - 1] = data[i];

	switch (data[0]) {
		case EMERGENCY_BRAKE:
			emergencyBrake();
			break;
		case SET_DISTANCE:
			writeString("Setting distance to: ");
			writeInteger(data[1], 10);
			writeString(",  ");
			writeInteger(data[2], 10);
			writeString("\n\r");
			driveDistance(data[1], data[2]);
			
			break;
		case INCREASE :
			rightDesiredSpeed += 5;
			leftDesiredSpeed += 5;
			writeString("Increasing speed\n\r");
			break;
		case DECREASE :
			rightDesiredSpeed -= 5;
			leftDesiredSpeed -= 5;
			writeString("Decreasing speed\n\r");
			break;
		case TURN_RIGHT :
			rightDesiredSpeed -= 5;
			leftDesiredSpeed += 5;
			writeString("Turning right\n\r");
			break;
		case TURN_LEFT :
			rightDesiredSpeed += 5;
			leftDesiredSpeed -= 5;
			writeString("Turning left\n\r");
			break;
		case UNBLOCK :
			blocked = 0x00;
			break;
	}
}

/************************************************************************/
/* ISR gets called when something happens on the i2c bus                */
/************************************************************************/
ISR(TWI_vect) {
	slaaftwi();
}