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

 #include <avr/io.h>
 #include <avr/interrupt.h>

 void usartToMotors(char leftOver);
 void distanceAndDirection(char data[]);
 void getParcours(char data[]);
 void continueParcours(char data[]);

 
/************************************************************************/
/* initializes all communications                                       */
/************************************************************************/
 void initCommunication(){
	 PORTD |= 0x03; //pullup SDA en SCL
	 initUSART();
	 UCSR0B |= 1 << RXCIE0;
	 init_master();
 }
 
 /************************************************************************/
 /* met deze functie kan een array van bytes worden verzonden            */
 /************************************************************************/
  void verzenden_array(char address, char b[], char tel) {
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
/* changes desired motorspeeds according to input                       */
/************************************************************************/
void usartToMotors(char leftOver){
	if(leftOver & 8) verzenden(DEVICE_ADRES, INCREASE);
	if(leftOver & 4) verzenden(DEVICE_ADRES, TURN_LEFT);
	if(leftOver & 2) verzenden(DEVICE_ADRES, DECREASE);
	if(leftOver & 1) verzenden(DEVICE_ADRES, TURN_RIGHT);
	if(leftOver & 15) emergencyBrake();
}

void distanceAndDirection(char data[]){
	for(int i = 0; i < 3; i++)
		distanceDirection[i] = data[i];
	followDirection = 1;
}

void getParcours(char data[]){
	struct Node node;
	for(int i = 0; i < 3; i++) node.data[i] = data[i];
	while(head != tail) head = head->next;
	head = tail = &node;
}

void continueParcours(char data[]){
	struct Node node;
	if(data[0] == 0){ parcours = 1; return; }
	for(int i = 0; i < 3; i++) node.data[i] = data[i];
	tail->next = &node;
	tail = &node;
}

void emergencyBrake(){
	verzenden(DEVICE_ADRES, EMERGENCY_BRAKE);
}

void brake(){
	emergencyBrake();
	verzenden(DEVICE_ADRES, UNBLOCK);
}

uint8_t getBlocked(){
	uint8_t blocked[1]; uint8_t temp = 0;
	ontvangen(DEVICE_ADRES, blocked, temp);
	return blocked[0];
}

 /************************************************************************/
 /* interupt service routine for input from the pc                       */
 /************************************************************************/
 ISR(USART0_RX_vect){
	char mode = UDR0;
	char dataAmount = mode & 0x1F;
	char data[dataAmount];
	writeChar(ACK);

	// receive data bytes and store them in an array
	for(int i = 0; i < dataAmount; i++){
		while(UCSR0A & (1 << RXC0));
		data[i] = UDR0;
		writeChar(ACK);
	}
	// decide what to do with data
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
			sendSensors();
			break;
		default:
			writeChar(NACK);
			break;
	 }

	 writeChar(ACK);
 }
 
 
 
