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
 #include <stdlib.h>
 #include <avr/io.h>
 #include <avr/interrupt.h>
 
//int32_t leftEncTicks = 0;
 

 int main(void)
 {
	initMotors();
	initCommunication();
	
	sei(); //De slave van i2c werkt met interrupt
	
	PORTD |= 1 << PIND4;
	
	//setMotors(179, 179);
	writeString("Test");
	while(1){
		/*
		if((leftEncTicks > (625*10)) && (rightEncTicks > (625*10))){
			emergencyBrake();
		}
		*/
	}
	
	
	return 0;
}
 
 
  