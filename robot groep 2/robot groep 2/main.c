/*
 * robot groep 2.c
 *
 * Created: 9-3-2017 20:00:45
 * Author : Erik
 */ 
 

 #include <stdint.h>
 #include <stdlib.h>
 #include <avr/io.h>
 #include <avr/interrupt.h>
 
#include "rp6aansluitingen.h"
#include "MotorControl.h"
#include "communications.h"
#include "i2c.h"
//#include "../../shared/twi_codes.h";
 
// void initCommunication();
 
 int main(void)
 {
	initMotors();
	initCommunication();
	
	sei(); //De slave van i2c werkt met interrupt
	
	
	
	
	
	DDRC &= ~(1 << PINC6)	;
	PORTC |= 1<<PINC6;
	setMotors(100, 100);
	while(1){
		
		if(PINC & 1<<PINC6){
			emergencyBrake();
		}
		//writeInteger(PINC, 10);
		
		//writeString("\n\r");
	
	}
	
	
	////////////
}
 