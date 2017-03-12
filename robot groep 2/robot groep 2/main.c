/*
 * robot groep 2.c
 *
 * Created: 9-3-2017 20:00:45
 * Author : Erik
 */ 

 
#include "rp6aansluitingen.h"
#include <avr/io.h>
#include <stdint.h>
#include <util/delay.h>


#define MAXSPEED		0xff
#define MOTORSPEED_R	OCR1A
#define MOTORSPEED_L	OCR1B


// movement functions
void initMotors();
void setMotors(int left, int right);
void emergencyBrake();

// communication functions
void initCommunication();

int rightDSpeed;		// these variables are used to store the desired speed between -maxspeed and +maxspeed
int leftDSpeed;			// they are used to adjust motor speed accordingly in the main while loop


int main(void)
{
	initMotors();
	initCommunication();
	
	// TEST
	setMotors(MAXSPEED, MAXSPEED);
	for(int i = 0; i < 40; i++){
		_delay_ms(250);
	}
	emergencyBrake();

    while (1) 
    {
    }
}


/************************************************************************/
/* initialize the motors pwm system                                     */
/************************************************************************/
void initMotors(){
	// TODO set to 8 bit mode, phase corrected
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
/* set the motors                                                       */
/************************************************************************/
void setMotors(int left, int right){
	if(right > MAXSPEED) right = MAXSPEED;
	else if(right < -MAXSPEED) right = -MAXSPEED;
	MOTORSPEED_R = 0xff & right;
	if(left > MAXSPEED) right = MAXSPEED;
	else if(left < -MAXSPEED) right = -MAXSPEED;
	MOTORSPEED_L = 0xff & left;

	// set direction so ports can be adjusted as necessary
	int direction = 0;
	if(right > 0) direction |= DIR_R;
	if(left > 0) direction |= DIR_L;
	PORTC = (PORTC & ~(1 << DIR_R | ( 1 << DIR_L))) | direction;
}

/************************************************************************/
/* lets the robot make an emergency brakeand resets all movement        */
/************************************************************************/
void emergencyBrake(){
	setMotors(0, 0);
	// TODO reset all tasks
}

/************************************************************************/
/* initialises i2c communication to Arduino                             */
/************************************************************************/
void initCommunication(){
	// TODO
}