/*
 * arduino.c
 *
 * Created: 12-3-2017 13:25:
 * Author : Oetze, Erik
 */ 

#include "communications.h"
#include "sensors.h"
#include "i2c_communication.h"

#include "evasion.h"

#include <avr/interrupt.h>


int main(void)
{
	sei();
	initCommunication();
	init_i2c_communication();
	initSensors();
    while (1) 
    {
		
	   //if(getBlocked() == 0xF0) evade();
	   // if(parcours == 1) driveParcours();
	   //if(followDirection == 1) changeDirection();
    }
}