/*
 * MotorControl.h
 *
 * Created: 17-3-2017 09:34:43
 *  Author: Erik
 */ 
 #include <stdint.h>

#ifndef MOTORCONTROL_H_
#define MOTORCONTROL_H_


/************************************************************************/
/* a structure which stores a register for controlling a wheels speed,  */
/* a bit used to set its direction                                      */
/* and the current speed as a number between -255 and 255               */
/************************************************************************/
typedef struct Wheel {
	uint8_t speed;
	uint8_t direction;
	int16_t currentSpeed;
} Wheel;

// functions
void initMotors();
void setMotors(int left, int right);
void motor(int speed, Wheel *wheel);
void driveDistance(uint8_t distance, uint8_t speed);
void emergencyBrake();

// external variables
int rightDesiredSpeed;
int leftDesiredSpeed;
uint8_t blocked;


int32_t leftEncTicks;
int32_t rightEncTicks;

uint32_t fired;

#endif /* MOTORCONTROL_H_ */