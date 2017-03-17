/*
 * robot groep 2.c
 *
 * Created: 9-3-2017 20:00:45
 * Author : Erik
 */ 
 
 #include "rp6aansluitingen.h"
 #include <stdint.h>
 #include <stdlib.h>
 #include <util/delay.h>
 #include <avr/io.h>
 #include <util/twi.h>
 #include <avr/interrupt.h>

 #define MAXSPEED					255
 #define MOTORSPEED_R				OCR1A
 #define MOTORSPEED_L				OCR1B
 

 #define MOTOR_ADJUST_FREQUENTIE	50
 #define MOTOR_ADJUST_TIME			0.5
 #define MOTOR_ADJUST_DELAY			F_CPU / MOTOR_ADJUST_FREQUENTIE * MOTOR_ADJUST_TIME / 1024

 #define TRUE 0xFF;
 #define FALSE 0;


 
 // movement functions
 void initMotors();
 void setMotors(int left, int right);
 void setLeftMotor(int speed);
 void setRightMotor(int speed);
 void emergencyBrake();
 
 // global variables
 int rightDesiredSpeed = 0;		// these variables are used to store the desired speed between -maxspeed(-255) and +maxspeed(255)
 int leftDesiredSpeed = 0;		// they are used to adjust motor speed accordingly in the main while loop



 // communication functions
 void initCommunication();
 void ontvangData(uint8_t data[],uint8_t tel);
 uint8_t verzendByte();
 
 // global variables
 uint8_t data_ont[20]; //max 20
 volatile uint8_t data_flag = FALSE;
 volatile uint8_t databyte=0x33;


 int main(void)
 {
	 initCommunication();
	 initMotors();

	 sei();
	 
	 rightDesiredSpeed = -200;

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
	int absSpeed = abs(speed);
	if(MOTORSPEED_L != absSpeed) MOTORSPEED_L = absSpeed;

	speed < 0 ? (PORTC |= DIR_L) : (PORTC &= ~DIR_L);
 }

  void setRightMotor(int speed){
	  int absSpeed = abs(speed);
	  if(MOTORSPEED_R != absSpeed) MOTORSPEED_R = absSpeed;

	  speed < 0 ? (PORTC |= DIR_R) : (PORTC &= ~DIR_R);
  }

 /************************************************************************/
 /* initializes the communications with arduino via i2c                  */
 /************************************************************************/
 void initCommunication(){
	// TODO
 }

 /************************************************************************/
 /* lets the robot make an emergency brake and resets all movement       */
 /************************************************************************/
 void emergencyBrake(){
	 setMotors(0, 0);
	 rightDesiredSpeed = leftDesiredSpeed = 0;
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

	writeInteger(rightTarget, 10);
	writeString("  ");
	writeInteger(leftTarget, 10);
	writeString("\n\r");
 }