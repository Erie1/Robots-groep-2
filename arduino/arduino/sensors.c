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
	// TODO
	adjust = 1;
}

void changeDirection(){
	// check of de robot niet klaar/geblokeerd is
	uint8_t blocked[1]; uint8_t temp;
	ontvangen(DEVICE_ADRES, blocked, temp);
	if(blocked[0] == 0xFF) {
		followDirection = 0;
		writeString("finished");
		adjust = 1;
		return;
	}

	uint8_t compass = getCompass();
	if((compass == distanceDirection[0]) && adjust) {
		brake(); 
		uint8_t temp[] = { SET_DISTANCE, distanceDirection[1], distanceDirection[2] };
		verzenden_array(DEVICE_ADRES, temp, 3);
		adjust = 0;
	}

	int turn = distanceDirection[0] - compass;
	if(turn > 127 && turn <= -127) verzenden(DEVICE_ADRES, TURN_RIGHT);
	else verzenden(DEVICE_ADRES, TURN_LEFT); 
 }

 uint8_t getCompass(){
	// TODO
	return 0;
 }

 void sendSensors(){
	// TODO
 }