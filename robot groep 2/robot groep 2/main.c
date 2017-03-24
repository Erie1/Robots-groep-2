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
	setMotors(100, 100);
	while(1){
		//writeInteger(GIFR, 2);
		//writeString("\n\r");
		////testVariable = leftEncTicks >> 16;
		////writeInteger(testVariable, 10);
		//writeString("\n\r");
	}
	
	
	return 0;
}
 
 
  