/*
 * robot groep 2.c
 *
 * Created: 9-3-2017 20:00:45
 * Author : Erik
 */ 

#include <avr/io.h>
#include "rp6aansluitingen.h"

#define F_CPU			8000000

#define MAXSPEED		0xffff
#define MOTORSPEED_R	OCR1A
#define MOTORSPEED_L	OCR1B

void initMotors();
void setMotors(int left, int right);
int getLeftMotor();
int getRightMotor();

void initCommunication();

int main(void)
{
	initMotors();
	initCommunication();
	
	// TEST
	setMotors(MAXSPEED, MAXSPEED);
	
    while (1) 
    {
    }
}

/************************************************************************/
/* initialize the motors pwm system                                     */
/************************************************************************/
void initMotors(){
	TCCR1B = 1 << WGM13 | (1 << WGM12);		// fast pwm w/ OCR1x
	TCCR1A = 1 << WGM11 | (1 << WGM10);

	TCCR1A |= 1 << COM1A1 | (1 << COM1B1);	// non inverted mode on both motors

	TCCR1B |= 1 << CS10;					// no prescaler

	DDRD = MOTOR_R | MOTOR_L;				// MOTOR_R & MOTOR_L as output

	OCR1A = 0;								// initialize motors with no speed
	OCR1B = 0;
}

/************************************************************************/
/* set the motors, if null as input motor won't be changed              */
/************************************************************************/
void setMotors(int left, int right){
	MOTORSPEED_R = 0xff & right;
	MOTORSPEED_L = 0xff & left;
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