/*
 * robotSensor.c
 *
 * Created: 17-3-2017 09:56:03
 * Author : giova
 */ 

#include <avr/io.h>
#include <util/delay.h>

#define US_PORT PORTA			 //the port of the sensor
#define US_PIN PINA			 //the pin of the sensor
#define US_DDR DDRA
#define US_POS PA0				 //PORTA0
#define US_ERROR 0xffff			 //time-out system 
#define US_NO_OBSTACLE 0xfffe    //no obstacle in range

/*******************************************************************/
/* This function measusers the width of high pulse in micro second */
/*******************************************************************/

 uint16_t getPulsewidth()
 {
	 uint32_t i, result;
	 
	 //wait for the rising edge
	 for (i=0;i<600000;i++)
	 {
		 if(!(US_PIN & (1<<US_POS)))
			continue;
		 else 
			break;
	 }
	 if(i == 600000)
		return 0xffff; //indicates time-out
    
	 //High Edge Found
	 
	 //Setup Timer1
	 TCCR1A = 0x00;
	 TCCR1B = (1<<CS11); //Prescealer = Fcpu/8
	 TCNT1 = 0x00;		 //Init counter
	 
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
	 
	 //Now wait for the falling edge
	 for(i=0;1<600000;i++)
	 {
		 if (US_PIN & (1<<US_POS))
		 {
			 if(TCNT1 >60000)
				break;
			 else
				continue;
		 }
		 else
			break;
	 }
	 if(1==600000)
		return 0xffff; //indicates time out
		
	result = TCNT1; //Falling Edge Found
	TCCR1B = 0x00; //Stop Timer
	
	if(result > 60000)
		return 0xfffe; // no obstacle in range
	else
		return (result>>1);
 }
 
int main(void)
{
	uint16_t PW; //Pulse Width
	verstuur("Ultra Sonic Active"); //Debug message
	
    while (1) 
    {
		US_DDR |= (1<<US_POS) //Set Ultra Sonic Port as out
		_delay_us(10);
		
		//Give the US pin a 15us High Pulse
		US_PORT |= (1<<US_POS); //High
		_delay_us(15);
		US_PORT &= (~(1<<US_POS)); //Low
		_delay_us(20);
		
		US_DDR &= (~(1<<US_POS)); //make the pin input
		PW = getPulsewidth();
		
		//Handle Errors
		if(PW == US_ERROR)
		{
			verstuur("US Error!");
		}
		else if(PW == US_NO_OBSTACLE)
		{
			verstuur("US no obstacle in range!");
		}
		//Distance calculation
		else
		{
			int distance;
			distance = (PW/58.0); //Convert to cm
			
			if(distance <= 8.0) // obstacle closer then 8 cm
			{
				setMotors(0,0); //emergency break
				//emergencyBreak(); als deze weer werkt
			}
			
		}
    }
}

