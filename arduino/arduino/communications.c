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
 
 void verzenden_array(uint8_t address, uint8_t b[], uint8_t tel);

 void setMode(uint8_t set);
 // modes
 void sendControl(uint8_t targets[]);
 void sendDistance(uint8_t distance);
 void distanceAndDirection(uint8_t);

 void usartToMotors(uint8_t leftOver);

 void (*mode) (uint8_t);

/************************************************************************/
/* initializes all communications                                       */
/************************************************************************/
 void initCommunication(){
	 mode = setMode;

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

 /************************************************************************/
 /* sends a series of bytes regarding key presses                        */
 /************************************************************************/
 void sendControl(uint8_t targets[]){
	 uint8_t data[] = { CONTROL, targets[0], targets[1] };
	 verzenden_array(DEVICE_ADRES, data, 3);
 }

 void sendDistance(uint8_t distance){
	uint8_t data[] = { SET_DISTANCE, distance };
	verzenden_array(DEVICE_ADRES, data, 2);
 }

 /************************************************************************/
 /* sets the function to be executed on the next bytes sent by the pc    */
 /************************************************************************/ 
 void setMode(uint8_t set){
	 switch (set & 0xE0){
		 case CONTROL:
			mode = usartToMotors;
			break;
		 case SET_DISTANCE :
			mode = distanceAndDirection;
			break;
		 default:
		 mode = setMode;
	 }
 }

/************************************************************************/
/* changes desired motorspeeds according to input                       */
/************************************************************************/
void usartToMotors(uint8_t leftOver){
	uint8_t targets[] = { 0, 0 };
	int speed = 75;
	
	if(leftOver == 'w') {targets[1] += 2 * speed; targets[0] += 2 * speed; }
	if(leftOver == 's') {targets[1] -= 2 * speed; targets[0] -= 2 * speed;}
	if(leftOver == 'a') { targets[1] -= speed; targets[0] += speed; }
	if(leftOver == 'd') { targets[1] += speed; targets[0] -= speed; }
	if(leftOver == 'e') {targets[1] = 0; targets[0] = 0;}
	sendControl(targets);
}

 /************************************************************************/
 /* interupt service routine for input from the pc                       */
 /************************************************************************/
 ISR(USART0_RX_vect){
	 uint8_t data = UDR0;
	 usartToMotors(data);
	 setMode(data);
 }