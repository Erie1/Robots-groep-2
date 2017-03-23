/*
 * sensors.c
 *
 * Created: 20-3-2017 15:54:08
 *  Author: Erik
 */ 

 #include "sensors.h"

 #include <avr/io.h>
 #include <util/delay.h>
 #include <avr/interrupt.h>

int direction = 0;
int adjust = 0;

void (*adjustDirection) ();

void initSensors(){
	adjustDirection = changeDirection;
}

 void changeDirection(){
	
 }

 void keepDirection(){
	
 }

 uint8_t getCompass(){
	// TODO
	return 0;
 }

 void sendSensors(){
	// TODO
 }