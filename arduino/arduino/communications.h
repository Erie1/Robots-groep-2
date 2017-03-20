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
void (*mode) (uint8_t);
void setMode(uint8_t set);
void verzenden_array(uint8_t address, uint8_t b[], uint8_t tel);



#endif /* COMMUNICATIONS_H_ */