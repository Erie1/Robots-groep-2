/*
 * i2c_communication.h
 *
 * Created: 28-3-2017 12:01:32
 *  Author: Erik
 */ 
#include <stdint.h>

#ifndef I2C_COMMUNICATION_H_
#define I2C_COMMUNICATION_H_

#define DEVICE_ADRES   8

void init_i2c_communication();

void verzenden_array(char address, char b[], char tel);
void verzendenRP6(char command);
uint8_t getCompass();

uint8_t getBlocked();


#endif /* I2C_COMMUNICATION_H_ */