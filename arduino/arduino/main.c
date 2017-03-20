/*
 * arduino.c
 *
 * Created: 12-3-2017 13:25:33
 * Author : Erik
 */ 
#define F_CPU 16000000
#define DEVICE_ADRES   8

#include "../../shared/twi_codes.h"
#include "i2c_mst.h"
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdint.h>

// functions
void (*mode) (uint8_t);
void setMode(uint8_t set);

int main(void)
{
	mode = setMode;

	PORTD = 0x03; //pullup SDA en SCL
	initUSART();
	UCSR0B |= 1 << RXCIE0;
    init_master();
	sei();

	while (1)
	{
	}

}

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

void sendControl(uint8_t key){
	uint8_t data[] = { CONTROL, key };
	verzenden_array(DEVICE_ADRES, data, 2);
}

void setMode(uint8_t set){
	switch (set){
		case CONTROL:
			mode = sendControl;
			break;
		default:
			mode = setMode;
	}
}

ISR(USART0_RX_vect){
	uint8_t data = UDR0;
	sendControl(data);
}