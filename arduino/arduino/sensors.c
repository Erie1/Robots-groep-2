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
 #include <avr/interrupt.h>

 void changeDirection();

 uint8_t adjust;

// initializes all the sensors
void initSensors(){
	head->next = tail;
	adjust = 1;
	writeString("read_Ulatrasone entered"); //debug purposes
	PORTD |= (1 << PIND3); // turn on PIND2
	init_timer1();
}

// function used for driving in a certain direction and keeping that direction
void changeDirection(){
	// check if the robot isn't finished or crashed
	uint8_t blocked = getBlocked();
	if(blocked == 0x0F) {
		followDirection = 0;
		adjust = 1;
		verzenden(DEVICE_ADRES, UNBLOCK);
		return;
	}

	// set the rp6 to drive forward at the desired speed when pointing the right direction
	uint8_t compass = getCompass();
	if((compass == drive.direction) && adjust) {
		brake(); 
		char temp[] = { SET_DISTANCE, drive.speed, drive.distance };
		verzenden_array(DEVICE_ADRES, temp, 3);
		adjust = 0;
	}

	// adjust the robot a little bit so it stays on course
	int turn = drive.direction - compass;
	if(turn > 127 && turn <= -127) verzenden(DEVICE_ADRES, TURN_RIGHT);
	else verzenden(DEVICE_ADRES, TURN_LEFT); 
 }

 void driveParcours(){
	if(getBlocked() == 0x0F){
		if(head != tail){
			for (int i = 0; i < 3; i++) drive = head->data;
			head = head->next;
		} else {
			parcours = 0;
			writeString("finished");
		}
	}
 }

 uint8_t getCompass(){
	 verzenden(96, 0x01);
	 uint8_t temp[1];
	 ontvangen(96, temp, 1);
	 return temp[0];
 }

 void sendSensors(){
	// TODO
 }

 /************************************************************************/
 /* initialize timer, interrupt and variable  (100 ms timer)             */
 /******************************************** ****************************/
 void init_timer1()
 {
	 TCCR1B |= (1 << WGM12)|(1 << CS11)|(1 << CS10); // set up timer with prescaler = 64 and enable CTC mode
	 TCNT1 = 0; 	// initialize counter
	 TIMSK1 |= (1 << OCIE1A); // enable compare interrupt
	 OCR1A = 24999;
 }

 /****************************** ******************************************/
 /* initialize PC INT2, interrupt and variable                            */
 /************************************************************************/
 void init_PCINT2()
 {
	 TCNT1 = 0;
	 DDRD &= ~(1 << PIND3); //PD2 (PCINT2 pin) is now an input
	 
	 EICRA |= (1 << ISC31);
	 EIMSK |= (1 << INT3);	//set PCINT2 to trigger an interrupt on status chance
 }

 /*********************************************** ***************************************/
 /* TIMER1 compare interrupt service routine called whenever there is a compare match  */
 /**************************************************************************************/
 ISR(TIMER1_COMPA_vect)
 {
	DDRD |= 1 << PIND3;
	init_PCINT2();
 }

 /** ***********************************************************************************/
 /* Pin Chance interrupt2 service routine called upon when sensor pin state chances   */
 /************************************************************* ************************/
 ISR(INT3_vect)
 {
	 writeString("\n\r");
	 sensorDistance = TCNT1;
	 if(sensorDistance < 8.0) emergencyBrake();
	 writeInteger(sensorDistance, 10);
	 TCNT1 = 0;
 }