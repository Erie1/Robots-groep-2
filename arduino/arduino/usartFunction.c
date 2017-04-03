/*
 * usartFunction.c
 *
 * Created: 28-3-2017 12:21:31
 *  Author: Erik
 */ 
#include "../../shared/twi_codes.h"
#include "i2c_communication.h"
#include "communications.h"

#include "usartFunction.h"
#include "sensors.h"

/************************************************************************/
/* changes desired motorspeeds according to input                       */
/************************************************************************/
void usartToMotors(char leftOver){
	if(leftOver & 8) verzendenRP6(INCREASE);
	if(leftOver & 4) verzendenRP6(TURN_LEFT);
	if(leftOver & 2) verzendenRP6(DECREASE);
	if(leftOver & 1) verzendenRP6(TURN_RIGHT);
	if(leftOver & 15) emergencyBrake();
}

void distanceAndDirection(char data[]){
	drive = (DistanceDirection) { data[0], data[1], data[2] };
	followDirection = 1;
	adjust = 1;
}

void getParcours(char dataOnt[]){
	while(head != tail) head = head->next;
	Node node = getNode(dataOnt);
	head = &node;
}

void continueParcours(char dataOnt[]){
	Node node = getNode(dataOnt);
	if(dataOnt[0] == 0){ parcours = 1; return; }
	tail = tail->next = &node;
}

Node getNode(char dataOnt[]){
	DistanceDirection temp = { dataOnt[0], dataOnt[1], dataOnt[2] };
	Node node = { .data = temp };
	return node;
}

void emergencyBrake(){
	verzendenRP6(EMERGENCY_BRAKE);
}

void brake(){
	emergencyBrake();
	verzendenRP6(UNBLOCK);
}