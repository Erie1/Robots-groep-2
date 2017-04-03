/*
 * sensors.h
 *
 * Created: 20-3-2017 15:53:55
 *  Author: Erik
 */ 
 #include <stdint.h>
 #include "communications.h"

#ifndef SENSORS_H_
#define SENSORS_H_


void initSensors();
uint16_t getPulseWidth();
void sendSensors();
void changeDirection();
void driveParcours();

void init_timer1();
void init_PCINT2();


uint8_t adjust;
// a structure containing information for driving a distance in a direction
typedef struct DistanceDirection {
	uint8_t direction;
	uint8_t speed;
	uint8_t distance;
} DistanceDirection;

// contains the current distance and direction if necessary 
struct DistanceDirection drive;

// Node for a simple linkedList
typedef struct Node {
	struct Node *next;
	DistanceDirection data;
} Node;

// head and tail pointers for linkedList used in parcours
Node *head, *tail;

// stores if the robot is going through the distance and direction program
int followDirection;

// stores the last sonar distance measured
int sensorDistance;

int parcours;

#endif /* SENSORS_H_ */