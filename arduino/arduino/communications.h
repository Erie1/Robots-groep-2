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
void verzenden_array(char address, char b[], char tel);
void initCommunication();
void emergencyBrake();
void brake();
uint8_t getBlocked();

uint8_t parcours;

#endif /* COMMUNICATIONS_H_ */