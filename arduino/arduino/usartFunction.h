/*
 * usartFunction.h
 *
 * Created: 28-3-2017 12:21:09
 *  Author: Erik
 */ 


#ifndef USARTFUNCTION_H_
#define USARTFUNCTION_H_


void usartToMotors(char leftOver);
void distanceAndDirection(char data[]);
void getParcours(char data[]);
void continueParcours(char data[]);
Node getNode(char data[]);



#endif /* USARTFUNCTION_H_ */