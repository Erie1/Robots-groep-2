/*
 * robot groep 2.c
 *
 * Created: 9-3-2017 20:00:45
 * Author : Erik
 */ 
 
 #include "rp6aansluitingen.h"
 #include "MotorControl.h"
 #include "communications.h"
 #include "i2c.h"
 #include <stdint.h>
 #include <util/delay.h>
 #include <stdlib.h>
 #include <avr/io.h>
 #include <avr/interrupt.h>
 

 int main(void)
 {
	//initialzes motors and communication
	initMotors();
	initCommunication();

	sei(); //De slave van i2c werkt met interrupt
	fired = 0;
	PORTD |= 1 << PIND4;

	while(1){
		if(leftDesiredSpeed > 255) leftDesiredSpeed = 255;
		if(leftDesiredSpeed < -255) leftDesiredSpeed = -255;
		if(rightDesiredSpeed > 255) rightDesiredSpeed = 255;
		if(rightDesiredSpeed < -255) rightDesiredSpeed = -255;
		
		/*
		setMotors(-254, -254);
		for (int i = 0; i < 20; i++)
		{
			_delay_ms(262);
		}
		setMotors(255, 255);
		for (int i = 0; i < 20; i++)
		{
			_delay_ms(262);
		}
		*/
		setMotors(leftDesiredSpeed, rightDesiredSpeed);
		
	}
	return 0;
}