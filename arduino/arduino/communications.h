/*
 * communications.h
 *
 * Created: 20-3-2017 15:16:14
 *  Author: Erik
 */ 
 #include <stdint.h>

#ifndef COMMUNICATIONS_H_
#define COMMUNICATIONS_H_

#define DEVICE_ADRES   8

// functions
void initCommunication();
void sendControl(int targets[]);
void emergencyBrake();


void usartToMotors(uint8_t leftOver);

#endif /* COMMUNICATIONS_H_ */