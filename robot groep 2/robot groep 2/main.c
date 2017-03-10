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
	TCCR1A = 1 << WGM11 | (1 << WGM10);		// fast pwm w/ OCR1x
	TCCR1B = 1 << WGM13 | (1 << WGM12);

	TCCR1A |= 1 << COM1A1 | (1 << COM1B1);	// non inverted mode on both motors

	TCCR1B |= 1 << CS10;					// no prescaler

	DDRC |= DIR_R | DIR_L;					// set direction pins as output
	DDRD |= MOTOR_R | MOTOR_L;				// MOTOR_R & MOTOR_L as output

	PORTC |= DIR_R | DIR_L;					// set motor direction to ???
	OCR1A = OCR1B = 0;						// initialize motors with no speed
}

/************************************************************************/
/* set the motors, if -1 as input motor won't be changed              */
/************************************************************************/
void setMotors(int left, int right){
	if(right >= 0)
		MOTORSPEED_R = 0xff & right;
	if(left >= 0)
		MOTORSPEED_L = 0xff & left;
}

/************************************************************************/
/* initialises i2c communication with Arduino                           */
/************************************************************************/
void initCommunication(){
	// TODO
}