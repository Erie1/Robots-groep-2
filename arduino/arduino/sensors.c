/*
 * sensors.c
 *
 * Created: 20-3-2017 15:54:08
 *  Author: Erik
 */
 #include "../../shared/twi_codes.h"

 #include "sensors.h"
 #include "communications.h"
 #include "i2c_mst.h"

 #include <avr/io.h>
 #include <util/delay.h>
 #include <avr/interrupt.h>

void changeDirection();

uint8_t adjust;

void initSensors(){
	DDRD |= 1 <<PIND2;
	PORTD |= (1 << PIND2);

/*
	head->next = tail;
	adjust = 1;
	writeString("read_Ulatrasone entered"); //debug purposes
	PORTD |= (1 << PIND2); // turn on PIND2
	init_timer1();*/
}

void changeDirection(){
	// check of de robot niet klaar/geblokeerd is
	uint8_t blocked = getBlocked();
	if(blocked == 0x0F) {
		followDirection = 0;
		adjust = 1;
		verzenden(DEVICE_ADRES, UNBLOCK);
		return;
	}

	uint8_t compass = getCompass();
	if((compass == distanceDirection[2]) && adjust) {
		brake(); 
		char temp[] = { SET_DISTANCE, distanceDirection[0], distanceDirection[1] };
		verzenden_array(DEVICE_ADRES, temp, 3);
		adjust = 0;
	}

	int turn = distanceDirection[0] - compass;
	if(turn > 127 && turn <= -127) verzenden(DEVICE_ADRES, TURN_RIGHT);
	else verzenden(DEVICE_ADRES, TURN_LEFT); 
 }

 void driveParcours(){
	if(getBlocked() == 0x0F){
		if(head != tail){
			for (int i = 0; i < 3; i++) distanceDirection[i] = head->data[i];
			head = head->next;
		} else {
			parcours = 0;
			writeString("finished");
		}
	}
 }

 uint8_t getCompass(){
	verzenden(192, 0x01);
	uint8_t temp[1], temp2 = 0;
	ontvangen(192, temp, temp2);
	return temp[0];
 }

 void sendSensors(){
	// TODO
 }

 /************************************************************************/
 /* initialize timer, interrupt and variable  (100 ms timer)             */
 /************************************************************************/
 void init_timer1()
 {
	 TCCR1B |= (1 << WGM12)|(1 << CS11)|(1 << CS10); // set up timer with prescealer = 64 and enable CTC mode
	 TCNT1 = 0; 	// initialize counter
	 TIMSK1 |= (1 << OCIE1A); // enable compare interrupt
	 OCR1A = 24999;
 }

 /************************************************************************/
 /* initialize PCINT2, interrupt and variable                            */
 /************************************************************************/
 void init_PCINT2()
 {
	 TCNT1 = 0;
	 DDRD &= ~(1 << PIND2); //PD2 (PCINT2 pin) is now an input
	 
	 EICRA |= (1 << ISC21) | (1 << ISC20);		//set PCIE2 to enable PCMSK2 scan
	 EIMSK |= (1 << INT2);	//set PCINT2 to trigger an interrupt on status chance
 }

 /**************************************************************************************/
 /* TIMER1 compare interrupt service routine called whenever there is a compare match  */
 /**************************************************************************************/
 ISR(TIMER1_COMPA_vect)
 {
	DDRD |= 1 << PIND2;
	_delay_us(15);
	init_PCINT2();
 }

 /** ***********************************************************************************/
 /* Pin Chance interrupt2 service routine called upon when sensor pin state chances   */
 /*************************************************************************************/
 ISR(INT2_vect)
 {
	writeString("moggel");
	 sensorDistance = TCNT1;
	 sensorDistance /= 58;
	 if(sensorDistance < 8.0) emergencyBrake();
	 writeInteger(sensorDistance, 10);
 }