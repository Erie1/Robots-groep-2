/*
 * sensors.c
 *
 * Created: 20-3-2017 15:54:08
 *  Author: Erik
 */
 #include "../../shared/twi_codes.h"

 #include "sensors.h"
 #include "communications.h"
 #include "i2c_mst.h"

 #include <avr/io.h>
 #include <util/delay.h>
 #include <avr/interrupt.h>

void changeDirection();

uint8_t adjust;

void initSensors(){
	head->next = tail;
	adjust = 1;
}

uint8_t getBlocked(){
	uint8_t blocked[1]; uint8_t temp = 0;
	ontvangen(DEVICE_ADRES, blocked, temp);
	return blocked[0];
}

void changeDirection(){
	// check of de robot niet klaar/geblokeerd is
	uint8_t blocked = getBlocked();
	if(blocked == 0xFF) {
		followDirection = 0;
		adjust = 1;
		verzenden(DEVICE_ADRES, UNBLOCK);
		return;
	}

	uint8_t compass = getCompass();
	if((compass == distanceDirection[2]) && adjust) {
		brake(); 
		char temp[] = { SET_DISTANCE, distanceDirection[0], distanceDirection[1] };
		verzenden_array(DEVICE_ADRES, temp, 3);
		adjust = 0;
	}

	int turn = distanceDirection[0] - compass;
	if(turn > 127 && turn <= -127) verzenden(DEVICE_ADRES, TURN_RIGHT);
	else verzenden(DEVICE_ADRES, TURN_LEFT); 
 }

 void driveParcours(){
	if(getBlocked() == 0xFF){
		if(head != tail){
			for (int i = 0; i < 3; i++) distanceDirection[i] = head->data[i];
			head = head->next;
		} else {
			parcours = 0;
			writeString("finished");
		}
	}
 }

 uint8_t getCompass(){
	verzenden(192, 0x01);
	uint8_t temp[1], temp2 = 0;
	ontvangen(192, temp, temp2);
	return temp[0];
 }

 void sendSensors(){
	// TODO
 }