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
 
 

 int timerValue = 0;
 uint32_t ticks = 0;

 int main(void)
 {
	//initialzes motors and communication
	initMotors();
	initCommunication();
	

	sei(); //De slave van i2c werkt met interrupt
	fired = 0;
	PORTD |= 1 << PIND4;
	
	while(1){
		
	}
	return 0;
}
 
 
void initTimers2(){
	TCNT2 = 0;
	TCCR2 |= (1 << CS20);
	TIMSK |= (1 << TOIE2);
	
	
}


	sei();

	while(1){
	}
}
 
 
  
