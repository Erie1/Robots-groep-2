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

int8_t data_ont[20]; //max 20
volatile uint8_t data_flag;
volatile uint8_t databyte;

void ontvangData(uint8_t data[],uint8_t tel);
uint8_t verzendByte();

void quickAdjust(uint8_t data);

void motorControl();
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
/* vertelt de arduino of er een noodstop is geweest                     */
/************************************************************************/
uint8_t verzendByte() {
	return blocked;
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
		case EMERGENCY_BRAKE:
			emergencyBrake();
			break;
		case SET_DISTANCE:
			driveDistance(data_ont[0]);
			break;
		case INCREASE :
			setRightMotor(MOTORSPEED_R + 5);
			setLeftMotor(MOTORSPEED_L + 5);
			break;
		case DECREASE :
			setRightMotor(MOTORSPEED_R - 5);
			setLeftMotor(MOTORSPEED_L - 5);
			break;
		case TURN_RIGHT :
			setRightMotor(MOTORSPEED_R + 5);
			setLeftMotor(MOTORSPEED_L - 5);
			break;
		case TURN_LEFT :
			setRightMotor(MOTORSPEED_R - 5);
			setLeftMotor(MOTORSPEED_L + 5);
			break;
		case UNBLOCK :
			blocked = 0x00;
			break;
	}
}

ISR(TWI_vect) {
	slaaftwi();
}