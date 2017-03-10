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
void accelerate(int desired);
void turn(int turnspeed);
int getLeftMotor();
int getRightMotor();

void initCommunication();

int main(void)
{
	initMotors();
	initCommunication();
	
	
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
/* accelarate (or deccelerate) to desired speed                         */
/************************************************************************/
void accelerate(int desired){
	// TODO
}

/************************************************************************/
/* lets the robot turn, turnspeed sets turnspeed,                       */
/* positive means right turn, negative means left                       */
/************************************************************************/
void turn(int turnspeed){
	// TODO
}

/************************************************************************/
/* returns current speed of the motor                                   */
/************************************************************************/
int getLeftMotor(){
	// TODO
}
int getRightMotor(){
	// TODO
}

/************************************************************************/
/* initialises i2c communication with Arduino                           */
/************************************************************************/
void initCommunication(){
	// TODO
}