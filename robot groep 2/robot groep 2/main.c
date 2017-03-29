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
 #include <util/delay.h>
 
//int32_t leftEncTicks = 0;
 
 

 int timerValue = 0;
 uint32_t ticks = 0;

void initTimers2();

int found = 1;
 int main(void)
 {
	initMotors();
	initCommunication();
	
	sei(); //De slave van i2c werkt met interrupt
	fired = 0;
	PORTD |= 1 << PIND4;
	/*
	setMotors(179, 179);
	writeString("Test");
	for(int x=0; x<4; x++){
		_delay_ms(250);
	}
	
	initTimers2();
	leftEncTicks =0;
	rightEncTicks = 0;
	while(1){
		if(found){
			writeInteger(leftEncTicks, 10);
			writeString("\n");
		}
		if((leftEncTicks > (625*10)) && found){
			found = 0;
			setMotors(0, 0);
			timerValue = TCNT2;
			ticks = (fired);
			
			
			
			writeString("It took: ");
			writeInteger(fired, 10);
			writeString("\n");
		}
		
	}
	*/
	while(1){
		
	}
	return 0;
}
 
 
void initTimers2(){
	TCNT2 = 0;
	TCCR2 |= (1 << CS20);
	TIMSK |= (1 << TOIE2);
	
	
}

