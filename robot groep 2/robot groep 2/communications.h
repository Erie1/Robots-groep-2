/*
 * communications.h
 *
 * Created: 19-3-2017 09:10:56
 *  Author: Erik
 */ 

#include <stdint.h>

#ifndef COMMUNICATIONS_H_
#define COMMUNICATIONS_H_

#define TRUE 0xFF;
#define FALSE 0;

#define D_KEY				1
#define S_KEY				2
#define A_KEY				4
#define W_KEY				8

// communication functions
void initCommunication();
void ontvangData(uint8_t data[],uint8_t tel);
uint8_t verzendByte();

// variables
double sonar_dis;

#endif /* COMMUNICATIONS_H_ */