/*
 * robot groep 2.c
 *
 * Created: 9-3-2017 20:00:45
 * Author : Erik
 */ 
 
 #include "rp6aansluitingen.h"
 #include "MotorControl.h"
 #include <stdint.h>
 #include <stdlib.h>
 #include <avr/io.h>
 #include <util/twi.h>
 #include <avr/interrupt.h>

 #define TRUE 0xFF;
 #define FALSE 0;
 
 // communication functions
 void initCommunication();
 void ontvangData(uint8_t data[],uint8_t tel);
 uint8_t verzendByte();
 
 // global variables
 uint8_t data_ont[20]; //max 20
 volatile uint8_t data_flag = FALSE;
 volatile uint8_t databyte=0x33;


 int main(void)
 {
	 initCommunication();
	 initMotors();

	 sei();
	 
	 rightDesiredSpeed = -200;

	 while (1)
	 {
	 }
 }


 /************************************************************************/
 /* initializes the communications with arduino via i2c                  */
 /************************************************************************/
 void initCommunication(){
	// TODO
 }