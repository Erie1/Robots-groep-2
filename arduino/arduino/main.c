/*
 * arduino.c
 *
 * Created: 12-3-2017 13:25:33
 * Author : Erik
 */ 
#define US_PORT			PORTD //the port of the sensor
#define US_PIN			PIND //the pin of the sensor
#define US_DDR			DDRD
#define US_POS			PD5 //PORTD5
#define US_ERROR		0xffff //time-out system
#define US_NO_OBSTACLE	0xfffe // no obstacle in range

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
	if(getBlocked() == 0xF0) evade();
	initSensors();
	initCommunication();
	sei();

	while (1)
	{
	}
}