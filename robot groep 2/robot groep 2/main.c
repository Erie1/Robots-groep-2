/*
 * robot groep 2.c
 *
 * Created: 9-3-2017 20:00:45
 * Author : Erik
 */ 
 
 #include "rp6aansluitingen.h"
 #include "MotorControl.h"
 #include "communications.h"
 #include <stdint.h>
 #include <stdlib.h>
 #include <avr/io.h>
 #include <avr/interrupt.h>
 
 
 
 int main(void)
 {
	initMotors();
	initCommunication();
	
	sei(); //De slave van i2c werkt met interrupt
	
	
	
	
	DDRD = 0x00;
	DDRC = 0x00;
	DDRB = 0x00;
	DDRA = 0x00;
	
	PORTD = 0xFF;
	PORTC = 0xFF;
	PORTB = 0xFF;
	PORTA = 0xFF;
	
	
	while(1){
		writeInteger(PINA, 10);
		writeInteger(PINB, 10);
		writeInteger(PINC, 10);
		writeInteger(PIND, 10);
		writeString("\n\r");
	}
	
	
	return 0;
}
 