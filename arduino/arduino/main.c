/*
 * arduino.c
 *
 * Created: 12-3-2017 13:25:33
 * Author : Erik
 */ 
#define F_CPU 16000000
#define DEVICE_ADRES   8

#include "../../shared/twi_codes.h"
#include "../../shared/commando_codes.h"
#include "i2c_mst.h"
#include <avr/io.h>
#include <stdlib.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdint.h>

#define US_PORT			PORTD //the port of the sensor
#define US_PIN			PIND //the pin of the sensor
#define US_DDR			DDRD
#define US_POS			PD5 //PORTD5
#define US_ERROR		0xffff //time-out system
#define US_NO_OBSTACLE	0xfffe // no obstacle in range

// functions
void (*mode) (uint8_t);
void setMode(uint8_t set);
void verzenden_array(uint8_t address, uint8_t b[], uint8_t tel);

uint16_t getPulseWidth();

int main(void)
{
	mode = setMode;

	PORTD = 0x03; //pullup SDA en SCL
	initUSART();
	UCSR0B |= 1 << RXCIE0;
    init_master();
	sei();
	uint16_t PW; //Pulse width for US
	writeString("Ultra Sonic Active"); //debug message

	while (1)
	{
		US_DDR |= (1<<US_POS); //Set Ultra Sonic Port as out
		_delay_us(10);
		//Give the US pin a 15us High Pulse
		US_PORT |= (1<<US_POS); //High
		_delay_us(15);
		US_PORT &= (~(1<<US_POS)); //Low
		_delay_us(20);
		
		US_DDR &= (~(1<<US_POS)); //Make the US pin input
		PW = getPulseWidth();
		
		//Handle Errors
		if(PW == US_ERROR)
		{
			writeString("US ERROR!");
		}
		else if( PW == US_NO_OBSTACLE)
		{
			writeString("US no obstacle in range!");
		}
		//distance calculation
		else
		{
			uint32_t distance;
			distance = (PW/58); //convert to cm
			uint8_t dist[4];
			for( int i  = 0; i < 4; i++){
				dist[i] = distance;
				distance <<= 8;
			}
			verzenden_array(DEVICE_ADRES, dist, 4);
		}
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

uint16_t getPulseWidth()
{
	uint32_t i, result;
	
	//wait for the rising edge
	for(i=0;i<600000;i++)
	{
		if(!(US_PIN & (1<<US_POS)))
			continue;
		else
			break;
	}
	if(i == 600000)
		return 0xffff; //indicates time out
	
	//High edge found
	//Setup Timer1
	TCCR1A = 0x00;
	TCCR1B = (1<<CS11); //prescealer = Fcpu/8
	TCNT1 = 0x00; //init counter
	
	//Now wait for the falling edge
	for(i=0; i<600000;i++)
	{
		if(US_PIN & (1<<US_POS))
		{
			if(TCNT1 > 60000)
				break;
			else
				continue;
		}
		else
			break;
	}
	if(i == 60000)
		return 0xffff; //indicates time out

	result = TCNT1;// Falling edge found
	TCCR1B = 0x00; //stop timer
	
	if(result > 60000)
		return 0xfffe; // no obstacle in range
	else
		return (result >> 1);
		
}