/*
 * communications.c
 *
 * Created: 20-3-2017 15:16:29
 *  Author: Erik
 */
 #include "../../shared/twi_codes.h"
 #include "../../shared/commando_codes.h"

 #include "communications.h"
 #include "sensors.h"
 #include "i2c_mst.h"

 #include <stdint.h>
 #include <avr/io.h>
 #include <avr/interrupt.h>
 
 void verzenden_array(uint8_t address, uint8_t b[], uint8_t tel);

 int setInputMode(uint8_t set);
 // modes
 int usartToMotors(uint8_t leftOver);
 int distanceAndDirection(uint8_t data);
 int parcours(uint8_t data);
 
 void sendDistance(uint8_t distance);

 int (*mode) (uint8_t);

/************************************************************************/
/* initializes all communications                                       */
/************************************************************************/
 void initCommunication(){
	 mode = setInputMode;

	 PORTD = 0x03; //pullup SDA en SCL
	 initUSART();
	 UCSR0B |= 1 << RXCIE0;
	 init_master();
 }
 
 /************************************************************************/
 /* met deze functie kan een array van bytes worden verzonden            */
 /************************************************************************/
  void verzenden_array(uint8_t address, uint8_t b[], uint8_t tel) {
	 // send start bit, wait for ack
	 TWCR |= (1<<TWSTA);
	 while(!(TWCR & (1<<TWINT)));

	 // write addres to i2c to initialize communications and wait for ack
	 TWDR=(address << 1);
	 TWCR=(1 << TWINT) | (1 << TWEN);
	 while(!(TWCR & (1 << TWINT)));

	 // write data to bus, wait for ack after each write
	 for (int i = 0; i < tel; i++) {
		 TWDR = b[i];
		 TWCR = (1 << TWINT)|(1 << TWEN);
		 while(!(TWCR & (1 << TWINT)));
	 }

	 // stop data
	 TWCR=(1<<TWINT)|(1<<TWSTO)|(1<<TWEN);
 }

 void sendDistance(uint8_t distance){
	uint8_t data[] = { SET_DISTANCE, distance };
	verzenden_array(DEVICE_ADRES, data, 2);
 }

 /************************************************************************/
 /* sets the function to be executed on the next bytes sent by the pc    */
 /************************************************************************/ 
 int setInputMode(uint8_t set){
	 switch (set & 0xE0){
		case COM_CONTROL:
			mode = usartToMotors;
			break;
		case COM_AFSTANDRICHTING :
			mode = distanceAndDirection;
			break;
		case COM_PARCOURS :
			mode = parcours;
			break;
		case COM_REQUEST_SENSORS :
			sendSensors();
			break;
		default:
			return 1;
	 } return 0;
 }

/************************************************************************/
/* changes desired motorspeeds according to input                       */
/************************************************************************/
int usartToMotors(uint8_t leftOver){
	if(leftOver & 8) verzenden(DEVICE_ADRES, INCREASE); 
	if(leftOver & 4) verzenden(DEVICE_ADRES, TURN_LEFT);
	if(leftOver & 2) verzenden(DEVICE_ADRES, DECREASE);
	if(leftOver & 1) verzenden(DEVICE_ADRES, TURN_RIGHT);
	if(leftOver & 15) emergencyBrake();
	mode = setInputMode;
	return 0;
}

int distanceAndDirection(uint8_t data){
	// TODO
	return 0;
}

int parcours(uint8_t data){
	// TODO
	return 0;
}

void emergencyBrake(){
	verzenden(DEVICE_ADRES, EMERGENCY_BRAKE);
}

 /************************************************************************/
 /* interupt service routine for input from the pc                       */
 /************************************************************************/
 ISR(USART0_RX_vect){
	 uint8_t data = UDR0;
	 mode(data) == 0 ? writeString((char)ACK) : writeString((char)NACK);
 }