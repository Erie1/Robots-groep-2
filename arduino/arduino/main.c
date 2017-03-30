/*
 * arduino.c
 *
 * Created: 12-3-2017 13:25:
 * Author : Oetze
 */ 

#include "communications.h"
#include "sensors.h"

#include "../../shared/twi_codes.h"
#include "../../shared/commando_codes.h"
#include "i2c_mst.h"
#include "evasion.h"

#include <avr/io.h>
#include <stdlib.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdint.h>


int main(void)
{
	sei();
	initCommunication();
	initSensors();
	
	
    while (1) 
    {
		
		/*
	   if(getBlocked() == 0xF0) evade();
	   if(parcours == 1) driveParcours();
	   if(followDirection == 1) changeDirection();
	   */
    }
}