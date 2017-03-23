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

#include <avr/io.h>
#include <stdlib.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdint.h>


int main(void)
{
	initSensors();
	initCommunication();
	sei();
	uint16_t PW; //Pulse width for US

	while (1)
	{
		if(followDirection == 1) adjustDirection();

		US_DDR |= (1<<US_POS); //Set Ultra Sonic Port as out
		_delay_us(10);
		//Give the US pin a 15us High Pulse
		US_PORT |= (1<<US_POS); //High
		_delay_us(15);
		US_PORT &= (~(1<<US_POS)); //Low
		_delay_us(20);
		
		US_DDR &= (~(1<<US_POS)); //Make the US pin input
		PW = getPulseWidth();
		
		//Handle Errors
		if(PW == US_ERROR)
		{
			writeString("US ERROR!");
		}
		else if( PW == US_NO_OBSTACLE)
		{
			writeString("US no obstacle in range!");
		}
		//distance calculation
		else
		{
			uint32_t distance;
			distance = (PW/58); //convert to cm
		}
	}
}



uint16_t getPulseWidth()
{
	uint32_t i, result;
	
	//wait for the rising edge
	for(i=0;i<600000;i++)
	{
		if(!(US_PIN & (1<<US_POS)))
			continue;
		else
			break;
	}
	if(i == 600000)
		return 0xffff; //indicates time out
	
	//High edge found
	//Setup Timer1
	TCCR1A = 0x00;
	TCCR1B = (1<<CS11); //prescealer = Fcpu/8
	TCNT1 = 0x00; //init counter
	
	//Now wait for the falling edge
	for(i=0; i<600000;i++)
	{
		if(US_PIN & (1<<US_POS))
		{
			if(TCNT1 > 60000)
				break;
			else
				continue;
		}
		else
			break;
	}
	if(i == 60000)
		return 0xffff; //indicates time out

	result = TCNT1;// Falling edge found
	TCCR1B = 0x00; //stop timer
	
	if(result > 60000)
		return 0xfffe; // no obstacle in range
	else
		return (result >> 1);
		
}