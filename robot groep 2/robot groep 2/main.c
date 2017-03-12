/*
 * robot groep 2.c
 *
 * Created: 9-3-2017 20:00:45
 * Author : Erik
 */ 
 
 #include "rp6aansluitingen.h"
 #include <stdint.h>
 #include <util/delay.h>
 #include <avr/io.h>
 #include <avr/interrupt.h>


 #define MAXSPEED		0xff
 #define MOTORSPEED_R	OCR1A
 #define MOTORSPEED_L	OCR1B
 
 #define ACCELERATION   100
 #define ACCELTIMER		78
 
 // movement functions
 void initMotors();
 void setMotors(int left, int right);
 void emergencyBrake();
 void rightMotor();
 void leftMotor();

 // communication functions
 void initCommunication();

 int rightDSpeed;		// these variables are used to store the desired speed between -maxspeed(-255) and +maxspeed(255)
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
 /* initialize the motors                                                */
 /************************************************************************/
 void initMotors(){
	 // set the PWM timer registers
	 TCCR1A = 1 << WGM10;					// phase corrected PWM 8 bit w/ OCR1x

	 TCCR1A |= 1 << COM1A1 | (1 << COM1B1);	// non inverted mode on both motors

	 TCCR1B |= 1 << CS10;					// no prescaler

	 TIMSK = 1 << OCIE1A | (1 << OCIE1B);	// enable the timer interrupt mask bits


	 // sets timer0 for acceleration and deceleration
	 TCCR0 = WGM01;							// CTC mode

	 TCCR0 |= 1 << CS00 | (1 << CS02);		// 1024 prescaler

	 TIMSK |= 1 << OCIE0;					// enable the timers interupt mask bit

	 OCR0 = ACCELTIMER;						// set the compare register for timer0

	 
	 // set the motor registers
	 DDRC |= DIR_R | DIR_L;					// set direction pins as output
	 DDRD |= MOTOR_R | MOTOR_L;				// MOTOR_R & MOTOR_L as output

	 PORTC |= DIR_R | DIR_L;					// set motor direction to ???
	 OCR1A = OCR1B = 0;						// initialize motor PWM timers with no speed
 }

 /************************************************************************/
 /* set the motors                                                       */
 /************************************************************************/
 void setMotors(int left, int right){
	 // TODO streamline code
	 MOTORSPEED_R = right;
	 MOTORSPEED_L = left;

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

 /************************************************************************/
 /* chances the speed of the right motor on timer0 COMPA interupt        */
 /************************************************************************/
 ISR(TIMER0_COMP_vect){
	int rightAcceleration = (MOTORSPEED_R - rightDSpeed) / ACCELERATION;
	int leftAcceleration = (MOTORSPEED_L - leftDSpeed) / ACCELERATION;
	
	rightDSpeed += rightAcceleration;
	rightDSpeed += leftAcceleration;
	
	setMotors(leftDSpeed, rightDSpeed);
 }