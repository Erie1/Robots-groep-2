/*
 * communications.c
 *
 * Created: 20-3-2017 15:16:29
 *  Author: Erik
 */
#include <avr/interrupt.h>

#include "../../shared/commando_codes.h"

#include "communications.h"
#include "usartFunction.h"

#include "i2c_mst.h"


 
/************************************************************************/
/* initializes pc communications                                        */
/************************************************************************/
 void initCommunication(){
	 initUSART();
 }

 /************************************************************************/
 /* interupt service routine for input from the pc                       */
 /************************************************************************/
 ISR(USART0_RX_vect){
	UCSR0B &= ~(1 << RXCIE0);
	char mode = UDR0;
	char dataAmount = mode & 0x1F; //Bitmask for data ammount.
	char data[dataAmount];
	// receive data bytes and store them in an array
	for(int i = 0; i < dataAmount; i++){
		while(!(UCSR0A & (1 << RXC0))){}
		data[i] = UDR0;
		//writeString("Data: ");
		//writeChar(data[i]);
			
	}
	// decide what to do with data 0xE0 Bitmask 3 MSB's
	switch (mode & 0xE0){
		case COM_CONTROL:
			usartToMotors(data[0]);
			break;
		case COM_AFSTANDRICHTING :
			distanceAndDirection(data);
			break;
		case COM_PARCOURS :
			getParcours(data);
			break;
		case COM_CONTINUE_PARCOURS:
			continueParcours(data);
			break;
		case COM_REQUEST_SENSORS :
			// sendSensors();
			break;
		default:
			writeChar(NACK);
			break;
	 }

	 writeChar(ACK);
	 UCSR0B |= 1 << RXCIE0;
 }
 
 
 
