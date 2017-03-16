/*
 * robot groep 2.c
 *
 * Created: 9-3-2017 20:00:45
 * Author : Erik
 */ 
 
 #include "rp6aansluitingen.h"
 #include "i2c.h"
 #include <stdint.h>
 #include <stdlib.h>
 #include <util/delay.h>
 #include <avr/io.h>
 #include <util/twi.h>
 #include <avr/interrupt.h>

 #define MAXSPEED					255
 #define MOTORSPEED_R				OCR1A
 #define MOTORSPEED_L				OCR1B
 
 #define MOTOR_ADJUST_FREQUENTIE	100
 #define MOTOR_ADJUST_DELAY			F_CPU / MOTOR_ADJUST_FREQUENTIE / 1024

 #define TRUE 0xFF;
 #define FALSE 0;


 
 // movement functions
 void initMotors();
 void setMotors(int left, int right);
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
	 //initCommunication();
	 initMotors();

	 sei();

	 while (1)
	 {
	 }
 }


 /************************************************************************/
 /* initialize the motors                                                */
 /************************************************************************/
 void initMotors(){
	 // set the PWM timer registers
	 TCCR1A = 1 << WGM10;					// phase corrected PWM 8 bit w/ ICR1 and OCR1x
	 ICR1 = 210;							// max safe power
	 TCCR1A |= 1 << COM1A1 | (1 << COM1B1);	// non inverted mode on both motors
	 TCCR1B |= 1 << CS10;					// no prescaler
	 TIMSK = 1 << OCIE1A | (1 << OCIE1B);	// enable the timer interrupt mask bits


	 // sets timer0 for acceleration and deceleration
	 TCCR0 = WGM01;							// CTC mode
	 TCCR0 |= 1 << CS00 | (1 << CS02);		// 1024 prescaler
	 TIMSK |= 1 << OCIE0;					// enable the timers interrupt mask bit
	 OCR0 = MOTOR_ADJUST_DELAY;				// set the compare register for timer0

	 
	 // set the motor registers
	 DDRC |= DIR_R | DIR_L;					// set direction pins as output
	 DDRD |= MOTOR_R | MOTOR_L;				// MOTOR_R & MOTOR_L as output

	 OCR1A = OCR1B = 0;						// initialize motor PWM timers with no speed
 }

 /************************************************************************/
 /* set the motors                                                       */
 /************************************************************************/
 void setMotors(int left, int right){
	// adjusts the motors PWM OCR's if necessary
	 if(MOTORSPEED_R != right) MOTORSPEED_R = abs(right);
	 if(MOTORSPEED_L != left) MOTORSPEED_L = abs(left);


	 // set direction so ports can be adjusted as necessary
	 int direction = 0;
	 if(right < 0) direction |= DIR_R;
	 if(left < 0) direction |= DIR_L;
	 PORTC = (PORTC & ~(1 << DIR_R | ( 1 << DIR_L))) | direction;
 }

 /************************************************************************/
 /* initializes the communications with arduino via i2c                  */
 /************************************************************************/
 void initCommunication(){
	initUSART();
	init_i2c_slave(8);
	init_i2c_verzend(verzendByte);
	init_i2c_ontvang(ontvangData);

 }

  /*slave heeft data ontvangen van de master
 data[] een array waarin de ontvangen data staat
 tel het aantal bytes dat ontvangen is*/
 
void ontvangData(uint8_t data[],uint8_t tel){
	for(int i=0;i<tel;++i)
	    data_ont[i]=data[i];
	data_flag = TRUE;
	writeString("o\n\r");
}

 uint8_t verzendByte(){
	return databyte++;
 }

 /************************************************************************/
 /* lets the robot make an emergency brake and resets all movement       */
 /************************************************************************/
 void emergencyBrake(){
	 setMotors(0, 0);
	 rightDesiredSpeed = leftDesiredSpeed = 0;
 }

/*  ISR(TWI_vect) {

	 slaaftwi();

 }*/

 /************************************************************************/
 /* chances the speed of the motors on timer0 COMP interrupt             */
 /************************************************************************/
 ISR(TIMER0_COMP_vect){
 /*
	int rightTarget = MOTORSPEED_R;
	int leftTarget = MOTORSPEED_L;

	if(rightDesiredSpeed != rightTarget)
		rightTarget += (rightDesiredSpeed - rightTarget) / MOTOR_ADJUST_FREQUENTIE + 1;
	if(leftDesiredSpeed != leftTarget)
		leftTarget += (leftDesiredSpeed - leftTarget) / MOTOR_ADJUST_FREQUENTIE + 1;
 
	 setMotors(leftTarget, rightTarget);*/
 }