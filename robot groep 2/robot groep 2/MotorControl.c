/*
 * MotorControl.c
 *
 * Created: 17-3-2017 09:35:00
 *  Author: Erik
 */ 
 
 #include "rp6aansluitingen.h"
 #include "MotorControl.h"
 #include <stdlib.h>
 #include <avr/io.h>
 #include <avr/interrupt.h>
 
 void setLeftMotor(int speed);
 void setRightMotor(int speed);

 /************************************************************************/
 /* initialize the motors                                                */
 /************************************************************************/
 void initMotors(){
	 // set the PWM timer registers
	 TCCR1A = 1 << WGM10;					// phase corrected PWM 8 bit w/ OCR1x
	 TCCR1A |= 1 << COM1A1 | (1 << COM1B1);	// non inverted mode on both motors
	 TCCR1B |= 1 << CS10;					// no prescaler


	 // sets timer0 for acceleration and deceleration
	 TCCR2 = WGM21;							// CTC mode
	 TCCR2 |= 1 << CS20 | (1 << CS22);		// 1024 prescaler
	 TIMSK |= 1 << OCIE2;					// enable the timers interrupt mask bit
	 OCR2 = MOTOR_ADJUST_DELAY;				// set the compare register for timer0

	 
	 // set the motor registers
	 DDRC |= DIR_R | DIR_L;					// set direction pins as output
	 DDRD |= MOTOR_R | MOTOR_L;				// MOTOR_R & MOTOR_L as output

	 OCR1A = OCR1B = 0;						// initialize motor PWM timers with no speed
 }

 
 /************************************************************************/
 /* set the motors                                                       */
 /************************************************************************/
 void setMotors(int left, int right){
	 setLeftMotor(left);
	 setRightMotor(right);
 }
 void setLeftMotor(int speed){
	if(blocked == 0xff) return;
	 int absSpeed = abs(speed);
	 if(MOTORSPEED_L != absSpeed) MOTORSPEED_L = absSpeed;

	 speed < 0 ? (PORTC |= DIR_L) : (PORTC &= ~DIR_L);
 }
 void setRightMotor(int speed){
	if(blocked == 0xff) return;
	 int absSpeed = abs(speed);
	 if(MOTORSPEED_R != absSpeed) MOTORSPEED_R = absSpeed;

	 speed < 0 ? (PORTC |= DIR_R) : (PORTC &= ~DIR_R);
 }

 /************************************************************************/
 /* lets the robot make an emergency brake and resets all movement       */
 /************************************************************************/
 void emergencyBrake(){
	 setMotors(0, 0);
	 rightDesiredSpeed = leftDesiredSpeed = 0;
	 blocked = 0xff;
 }

 void driveDistance(uint8_t distance){
	// TODO
 }

 /************************************************************************/
 /* chances the speed of the motors on timer0 COMP interrupt             */
 /************************************************************************/
 ISR(TIMER2_COMP_vect){
	 int rightTarget = MOTORSPEED_R;
	 if(PORTC & DIR_R) rightTarget = -rightTarget;
	 int leftTarget = MOTORSPEED_L;
	 if(PORTC & DIR_L) leftTarget = -leftTarget;

	 if(rightDesiredSpeed != rightTarget){
		 rightTarget += (rightDesiredSpeed - rightTarget) / MOTOR_ADJUST_FREQUENTIE;
		 rightDesiredSpeed > rightTarget ? setRightMotor(++rightTarget) : setRightMotor(--rightTarget);
	 }
	 if(leftDesiredSpeed != leftTarget){
		 leftTarget += (leftDesiredSpeed - leftTarget) / MOTOR_ADJUST_FREQUENTIE;
		 leftDesiredSpeed > leftTarget ? setLeftMotor(++leftTarget) : setLeftMotor(--leftTarget);
	 }
 }