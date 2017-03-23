/*
 * sensors.h
 *
 * Created: 20-3-2017 15:53:55
 *  Author: Erik
 */ 
 #include <stdint.h>

#ifndef SENSORS_H_
#define SENSORS_H_


uint16_t getPulseWidth();
void sendSensors();
void changeDirection();
void initSensors();

int followDirection;
uint8_t degrees;

#endif /* SENSORS_H_ */