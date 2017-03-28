/*
* i2c_communication.c
*
* Created: 28-3-2017 12:01:47
*  Author: Erik
*/
#include <avr/io.h>
#include <avr/interrupt.h>

#include "../../shared/twi_codes.h"
#include "i2c_communication.h"
#include "i2c_mst.h"


/************************************************************************/
/* initializes the i2c communication                                    */
/************************************************************************/
void init_i2c_communication(){
	PORTD |= 0x03;				//pullup SDA en SCL
	UCSR0B |= 1 << RXCIE0;		// enable receive interrupt
	init_master();
}

/************************************************************************/
/* met deze functie kan een array van bytes worden verzonden            */
/************************************************************************/
void verzenden_array(char address, char b[], char tel) {
	// send start bit, wait for ACK
	TWCR |= (1<<TWSTA);
	while(!(TWCR & (1<<TWINT)));

	// write address to i2c to initialize communications and wait for ACK
	TWDR=(address << 1);
	TWCR=(1 << TWINT) | (1 << TWEN);
	while(!(TWCR & (1 << TWINT)));

	// write data to bus, wait for ACK after each write
	for (int i = 0; i < tel; i++) {
		TWDR = b[i];
		TWCR = (1 << TWINT)|(1 << TWEN);
		while(!(TWCR & (1 << TWINT)));
	}

	// close I2C bus
	TWCR=(1<<TWINT)|(1<<TWSTO)|(1<<TWEN);
}

uint8_t getBlocked(){
	uint8_t blocked[1]; uint8_t temp = 0;
	ontvangen(DEVICE_ADRES, blocked, temp);
	return blocked[0];
}