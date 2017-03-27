/*
 * MotorControl.c
 *
 * Created: 17-3-2017 09:35:00
 *  Author: Erik
 */ 
 #define  SPEEDSCALER				100/100
 #include "rp6aansluitingen.h"
 #include "MotorControl.h"
 #include "i2c.h"
 #include <stdlib.h>
 #include <avr/io.h>
 #include <avr/interrupt.h>

 double distance;
 int timeScale0, timeScale2;

 /************************************************************************/
 /* initialize the motors                                                */
 /************************************************************************/
 void initMotors(){
	 // set the PWM timer registers
	 TCCR1A = 1 << WGM10;					// phase corrected PWM 8 bit w/ OCR1x
	 TCCR1A |= 1 << COM1A1 | (1 << COM1B1);	// non inverted mode on both motors
	 TCCR1B |= 1 << CS10;					// no prescaler

	 
	 // set the motor registers
	 DDRC |= DIR_R | DIR_L;					// set direction pins as output
	 DDRD |= MOTOR_R | MOTOR_L;				// MOTOR_R & MOTOR_L as output
	 
	 DDRD &= ~(ENC_R | ENC_L);				//Set encoder bits to input.

	 OCR1A = OCR1B = 0;						// initialize motor PWM timers with no speed
	 
	 GICR |= (1 << 7) | (1 << 6);			//Enable interupts on the encoders (int1, int0)
	 MCUCR |= (1 << ISC00) | (1 << ISC10);	//Set when interupts occure(At falling and rising edge)
	 
	 
	 PORTB |= PWRON;
	 
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
	uint8_t absSpeed = abs(speed);
	
	if(MOTORSPEED_L != absSpeed && absSpeed <= MAXSPEED) MOTORSPEED_L = absSpeed;
	
	speed < 0 ? (PORTC |= DIR_L) : (PORTC &= ~DIR_L);
 }

 void setRightMotor(int speed){
	if(blocked == 0xff) return;
	uint8_t absSpeed = abs(speed);
	
    if(MOTORSPEED_R != absSpeed && absSpeed <= MAXSPEED) MOTORSPEED_R = absSpeed;

	speed < 0 ? (PORTC |= DIR_R) : (PORTC &= ~DIR_R);
 }

 /************************************************************************/
 /* lets the robot make an emergency brake and resets all movement       */
 /************************************************************************/
 void emergencyBrake(){
	 setMotors(0, 0);
	 rightDesiredSpeed = leftDesiredSpeed = 0;
	 blocked = 0xF0;
 }

 void driveDistance(uint8_t length, uint8_t speed){
	rightDesiredSpeed = leftDesiredSpeed = speed;
	distance = length;
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

 void driving(){
	if(distance <= 0.0) return;
	distance -= 0.012;
	if(distance <= 0.0) {
		rightDesiredSpeed = leftDesiredSpeed = 0;
		setMotors(0, 0);
		blocked |= 0x0f;
	}
 }

ISR(INT0_vect){
	driving();
	if(leftDesiredSpeed > 100) leftDesiredSpeed = 100;
	uint8_t temp = TCNT1;
	uint8_t scale = abs(leftDesiredSpeed) * SPEEDSCALER;
	if(temp < scale) setLeftMotor(MOTORSPEED_L + 5);
	else if(temp > scale) setLeftMotor( MOTORSPEED_L - 5);
	TCNT0 = timeScale0 = 0;
}

ISR(INT1_vect){
	driving();
	if(rightDesiredSpeed > 100) rightDesiredSpeed = 100;
	uint8_t temp = TCNT2;
	uint8_t scale = abs(rightDesiredSpeed) * SPEEDSCALER;
	if(temp < scale) setRightMotor(MOTORSPEED_L + 5);
	else if(temp > scale) setRightMotor(MOTORSPEED_L - 5);
	TCNT2 = timeScale2 = 0;
}

ISR(TIMER0_OVF_vect){
	++timeScale0;
}
ISR(TIMER2_OVF_vect){
	++timeScale2;
}