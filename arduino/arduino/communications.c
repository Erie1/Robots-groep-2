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
 Node makeNode(char data[]);


 
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
	
}

void getParcours(char dataOnt[]){
	while(head != tail) head = head->next;
	Node node = makeNode(dataOnt);
	head = &node;
}

void continueParcours(char dataOnt[]){
	Node node = makeNode(dataOnt);
	if(dataOnt[0] == 0){ parcours = 1; return; }
	tail = tail->next = &node;
}

Node makeNode(char dataOnt[]){
	DistanceDirection temp = { dataOnt[0], dataOnt[1], dataOnt[2] };
	Node node = { .data = temp };
	return node;
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
	UCSR0B &= ~(1 << RXCIE0);
	char mode = UDR0;
	char dataAmount = mode & 0x1F; //Bitmask for data ammount.
	char data[dataAmount];
	//writeChar(ACK);
	//writeString("D ");
	//writeInteger(dataAmount, 10);
	//writeString("\n\r");
	// receive data bytes and store them in an array
	for(int i = 0; i < dataAmount; i++){
		while(!(UCSR0A & (1 << RXC0))){}
		data[i] = UDR0;
		//writeString("Data: ");
		//writeChar(data[i]);
			
	}
	//writeChar(ACK);
	// decide what to do with data 0xE0 Bitmask 3 MSB's
	switch (mode & 0xE0){
		case COM_CONTROL:
			usartToMotors(data[0]);
			//writeChar(126);
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
	 UCSR0B |= 1 << RXCIE0;
 }
 
 
 
