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

 void setInputMode(uint8_t set);
 // modes
 void usartToMotors(uint8_t leftOver);
 void distanceAndDirection(uint8_t data);
 void parcours(uint8_t data);
 
 void sendDistance(uint8_t distance);

 void (*mode) (uint8_t);

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

 /************************************************************************/
 /* sends a series of bytes regarding key presses                        */
 /************************************************************************/
 void sendControl(int targets[]){
	 uint8_t direction = 0;
	 if(targets[0] < 0) direction |= 1;
	 if (targets[0] < 0) direction |= 2;
	 uint8_t data[] = { CONTROL, direction, targets[0], targets[1] };
	 verzenden_array(DEVICE_ADRES, data, 4);
 }

 void sendDistance(uint8_t distance){
	uint8_t data[] = { SET_DISTANCE, distance };
	verzenden_array(DEVICE_ADRES, data, 2);
 }

 /************************************************************************/
 /* sets the function to be executed on the next bytes sent by the pc    */
 /************************************************************************/ 
 void setInputMode(uint8_t set){
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
		 default:
		 mode = setInputMode;
	 }
 }

/************************************************************************/
/* changes desired motorspeeds according to input                       */
/************************************************************************/
void usartToMotors(uint8_t leftOver){
	int targets[] = { 0, 0 };
	int speed = 75;
	
	if(leftOver == 'w') { targets[1] += 2 * speed; targets[0] += 2 * speed; }
	if(leftOver == 's') { targets[1] -= 2 * speed; targets[0] -= 2 * speed;}
	if(leftOver == 'a') { targets[1] -= speed; targets[0] += speed; }
	if(leftOver == 'd') { targets[1] += speed; targets[0] -= speed; }
	if(leftOver == 'e') { targets[1] = 0; targets[0] = 0;}
	writeInteger(targets[0], 10); writeString(" "); writeInteger(targets[1], 10);
	sendControl(targets);
	mode = setInputMode;
}

void distanceAndDirection(uint8_t data){
	// TODO
}

void parcours(uint8_t data){
	// TODO
}

void emergencyBrake(){
	verzenden(DEVICE_ADRES, EMERGENCY_BRAKE);
}

 /************************************************************************/
 /* interupt service routine for input from the pc                       */
 /************************************************************************/
 ISR(USART0_RX_vect){
	 uint8_t data = UDR0;
	 usartToMotors(data);
	 mode(data);
	 //writeString(ACK);
 }