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


uint16_t getPulseWidth();
void sendSensors();
void changeDirection();
void initSensors();
void driveParcours();
uint8_t getCompass();

void init_timer1();
void init_PCINT2();


typedef struct Node Node;
struct Node {
	struct Node *next;
	char data[3];
};

struct Node *head, *tail;
int followDirection;
uint8_t distanceDirection[3];
int sensorDistance;

#endif /* SENSORS_H_ */