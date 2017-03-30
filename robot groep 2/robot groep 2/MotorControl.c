/*
 * MotorControl.c
 *
 * Created: 17-3-2017 09:35:00
 *  Author: Erik
 */ 
 // a prescaler when calculating speed (equal to PWM with 180 on full charge)
 #define  SPEEDSCALER				2700/100

 #include "rp6aansluitingen.h"
 #include "MotorControl.h"
 #include "i2c.h"
 #include <stdlib.h>
 #include <avr/io.h>
 #include <avr/interrupt.h>

 Wheel right;
 Wheel left;

 // distance left in distance and direction mode
 double distance;
 // timer overflow counters for slow speeds
 int timeScale0, timeScale2;

 /************************************************************************/
 /* initialize the motors                                                */
 /************************************************************************/
 void initMotors(){	
	 right = (Wheel) { OCR1A, DIR_R, 0 };
	 left = (Wheel) { OCR1B, DIR_L, 0 };

	 // set the PWM timer registers
	 TCCR1A = 1 << WGM10;					// phase corrected PWM 8 bit w/ OCR1x
	 TCCR1A |= 1 << COM1A1 | (1 << COM1B1);	// non inverted mode on both motors
	 TCCR1B |= 1 << CS10;					// no prescaler

	 TCCR0 = TCCR2 = 1 << CS00;

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
 void setMotors(int leftSpeed, int rightSpeed){
     motor(leftSpeed, &left);
	 motor(rightSpeed, &right);
 }

 /************************************************************************/
 /* set a motor stored in a wheel struct to a certain speed              */
 /************************************************************************/
 void motor(int speed, Wheel *wheel){
	// return when robot has hit something or is finished and waiting for new instructions
	if(blocked == 0xff) return;

	// sets speed of the motor
	uint8_t absSpeed = abs(speed);
	if(wheel->speed != absSpeed) wheel->speed = absSpeed;
	
	// sets the direction of the motor based on positive or negative speed
	speed < 0 ? (PORTC |= wheel->direction) : (PORTC &= ~(wheel->direction));
	wheel->currentSpeed = speed;
 }

 /************************************************************************/
 /* lets the robot make an ncy brake and resets all movement       */
 /************************************************************************/
 void emergencyBrake(){
	 setMotors(0, 0);
	 rightDesiredSpeed = leftDesiredSpeed = 0;
	 blocked = 0xF0;
 }

 /************************************************************************/
 /* set a distance and a speed to drive                                  */
 /************************************************************************/
 void driveDistance(uint8_t length, uint8_t speed){
	rightDesiredSpeed = leftDesiredSpeed = speed;
	distance = length;
 }

 /************************************************************************/
 /* function used to calculate if the robot has reached its              */
 /* destination or checkpoint                                            */
 /************************************************************************/
 void driving(){
	if(distance <= 0.0) return;
	distance -= 0.012;
	if(distance <= 0.0) {
		rightDesiredSpeed = leftDesiredSpeed = 0;
		setMotors(0, 0);
		blocked |= 0x0f;
	}
 }

 /************************************************************************/
 /* ISR's thrown when a wheels encoder changes,                          */
 /* used to maintain speed and calculate distance driven                 */
 /************************************************************************/
ISR(INT0_vect){
	driving();
	if(leftDesiredSpeed > 100) leftDesiredSpeed = 100;
	uint8_t temp = TCNT1 + timeScale0 * 256;
	uint8_t scale = abs(leftDesiredSpeed) * SPEEDSCALER;
	if(temp < scale) motor(left.currentSpeed + 5, &left);
	else if(temp > scale) motor( left.currentSpeed - 5, &left);
	TCNT0 = timeScale0 = 0;
}
ISR(INT1_vect){
	driving();
	if(rightDesiredSpeed > 100) rightDesiredSpeed = 100;
	uint8_t temp = TCNT2 + timeScale2 * 256;
	uint8_t scale = abs(rightDesiredSpeed) * SPEEDSCALER;
	if(temp < scale)  motor(right.currentSpeed + 5, &right);
	else if(temp > scale) motor(right.currentSpeed - 5, &right);
	TCNT2 = timeScale2 = 0;
}

/************************************************************************/
/* ISR's used to keep track of the amount of overflows on timer0 and 2  */
/* used to calculate time between changes on motor encoders             */
/************************************************************************/
ISR(TIMER0_OVF_vect){
	++timeScale0;
}

ISR(TIMER2_OVF_vect){
	++timeScale2;
	fired++;
}

