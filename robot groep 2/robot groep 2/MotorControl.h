/*
 * MotorControl.h
 *
 * Created: 17-3-2017 09:34:43
 *  Author: Erik
 */ 
 #include <stdint.h>

#ifndef MOTORCONTROL_H_
#define MOTORCONTROL_H_


// defines
#define MAXSPEED					180
#define MOTORSPEED_R				OCR1A
#define MOTORSPEED_L				OCR1B

// adjustment defines
#define MOTOR_ADJUST_FREQUENTIE	50
#define MOTOR_ADJUST_TIME			0.5
#define MOTOR_ADJUST_DELAY			F_CPU / MOTOR_ADJUST_FREQUENTIE * MOTOR_ADJUST_TIME / 1024

// functions
void initMotors();
void setMotors(int left, int right);
void setLeftMotor(int speed);
void setRightMotor(int speed);
void driveDistance(uint8_t distance, uint8_t speed);
void emergencyBrake();

// external variables
int rightDesiredSpeed;
int leftDesiredSpeed;
uint8_t blocked;

#endif /* MOTORCONTROL_H_ */