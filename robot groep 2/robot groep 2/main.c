/*
 * robot groep 2.c
 *
 * Created: 9-3-2017 20:00:45
 * Author : Erik
 */ 

#include <avr/io.h>
#include "rp6aansluitingen.h"

void initMotors();
void setMotors(int left, int right);
int getLeftMotor();
int getRightMotor();

void initCommunication();

int main(void)
{
	initMotors();
	initUsart();
	
	
    while (1) 
    {
    }
}

/************************************************************************/
/* initialize the motors pwm system                                     */
/************************************************************************/
void initMotors(){
	// TODO
}

/************************************************************************/
/* set the motors, if null as input motor won't be changed              */
/************************************************************************/
void setMotors(int left, int right){
	// TODO
}

/************************************************************************/
/* returns current speed of the motor                                   */
/************************************************************************/
int getLeftMotor(){
	// TODO
}
int getLeftMotor(){
	// TODO
}

/************************************************************************/
/* initialises i2c communication with Arduino                           */
/************************************************************************/
void initCommunication(){
	// TODO
}