/*
 * robot groep 2.c
 *
 * Created: 9-3-2017 20:00:45
 * Author : Erik
 */ 
 
 #include "rp6aansluitingen.h"
 #include "i2c.h"

 #include <stdint.h>
 //#include <util/delay.h>
 #include <avr/io.h>
 #include <avr/interrupt.h>


 #define MAXSPEED		255
 #define MOTORSPEED_R	OCR1A
 #define MOTORSPEED_L	OCR1B
 
 #define TRUE 0xFF
 #define FALSE 9
 
 #define MOTOR_ADJUST_FREQUENTIE   100
 #define MOTOR_ADJUST_DELAY		F_CPU / MOTOR_ADJUST_FREQUENTIE / 1024
 
 #define SCL_frequentie 100000

 void ontvangData(uint8_t [],uint8_t);
 uint8_t verzendByte();
 

 
 // movement functions
 void initMotors();
 void setMotors(int left, int right);
 void emergencyBrake();
 void adjustmotors();

 // communication functions
 void initCommunication();
 void initUsartPC();
 void usartToMotors(uint8_t leftOver);

 int rightDSpeed;		// these variables are used to store the desired speed between -maxspeed(-255) and +maxspeed(255)
 int leftDSpeed;		// they are used to adjust motor speed accordingly in the main while loop

uint8_t data_ont[20]; //max 20
volatile uint8_t data_flag = FALSE;
volatile uint8_t databyte=0x33;


 int main(void)
 {
	 initMotors();
	 setMotors(-100, 100);
	 //initCommunication();
	 initUSART();
	 writeString("Passed usart \n\r");
	
	init_i2c_slave(8);
	writeString("Passed slave init \n\r");
	
	/*ontvangData is de functie die uitgevoerd wordt 
	wanneer een byte via de i2c bus ontvangen wordt
	*/
	init_i2c_ontvang(ontvangData); 
	writeString("Passed ontvangData init \n\r");
	
	/*verzendByte is de functie die aangeroepen wordt
	wanneer de slave een byte naar de master verzend*/
	init_i2c_verzend(verzendByte);
	writeString("Passed verzendByte \n\r");
	
	sei(); //De slave van i2c werkt met interrupt
	writeString("TESTESTESTET");
	//usartToMotors(0x84);
	
	
	 while(1){
		writeString("Attempting to!!!!!!");
		verzendByte();
		
		for(int i=0; i<10; i++){
			//_delay_ms(250);
		}
		writeString("Something is happening\r\n");
	 }
 
 }

 /************************************************************************/
 /* initialize the motors                                                */
 /************************************************************************/
 void initMotors(){
	 // set the PWM timer registers
	 TCCR1A = 1 << WGM10;					// phase corrected PWM 8 bit w/ OCR1x

	 TCCR1A |= 1 << COM1A1 | (1 << COM1B1);	// non inverted mode on both motors

	 TCCR1B |= 1 << CS10;					// no prescaler



	 // sets timer0 for acceleration and deceleration
	 TCCR0 = WGM01;							// CTC mode

	 TCCR0 |= 1 << CS00 | (1 << CS02);		// 1024 prescaler

	 TIMSK |= 1 << OCIE0;					// enable the timers interrupt mask bit

	 OCR0 = MOTOR_ADJUST_DELAY;				// set the compare register for timer0

	 
	 // set the motor registers
	 DDRC |= DIR_R | DIR_L;					// set direction pins as output
	 DDRD |= MOTOR_R | MOTOR_L;				// MOTOR_R & MOTOR_L as output

	 OCR1A = OCR1B = 0;						// initialize motor PWM timers with no speed
 }

 /************************************************************************/
 /* set the motors                                                       */
 /************************************************************************/
 void setMotors(int left, int right){
	 MOTORSPEED_R = right;
	 MOTORSPEED_L = left;

	 // set direction so ports can be adjusted as necessary
	 int direction = 0;
	 if(right < 0) direction |= DIR_R;
	 if(left < 0) direction |= DIR_L;
	 PORTC = (PORTC & ~(1 << DIR_R | ( 1 << DIR_L))) | direction;
 }

 /************************************************************************/
 /* lets the robot make an emergency brake and resets all movement        */
 /************************************************************************/
 void emergencyBrake(){
	 setMotors(0, 0);
	 rightDSpeed = leftDSpeed = 0;
 }

 /************************************************************************/
 /* initializes i2c communication to Arduino                             */
 /************************************************************************/
 
 /*
 void initCommunication(){
	 // TODO
	 
	
	 
	 void ontvangData(uint8_t [],uint8_t);
	 uint8_t verzendByte();
	 
	 init_i2c_slave(8);
	 
	 /*ontvangData is de functie die uitgevoerd wordt 
	 wanneer een byte via de i2c bus ontvangen wordt
	 */
	 //init_i2c_ontvang(ontvangData); 
	
	 /*verzendByte is de functie die aangeroepen wordt
	 wanneer de slave een byte naar de master verzend*/
	 //init_i2c_verzend(verzendByte);
	
	 //sei(); //De slave van i2c werkt met interrupt
 //}
 
 
 
 //Initialiseren van usart verbinding met pc voor directe besturing
 void initUsartPC(){
	
	/* Set baud rate */
	UCSRC = 0;
	UBRRH = 0;
	UBRRL = 103;
	/* Enable receiver and transmitter */
	UCSRB = (1<<RXEN)|(1<<TXEN);
	/* Set frame format: 8data, 2stop bit */
	UCSRC =
	 (0<<URSEL)|(3<<UCSZ0);
 }
 
 
 
 
 void usartToMotors(uint8_t leftOver){
	 //int leftOver;
	 int wPressed = 0;
	 int aPressed = 0;
	 int sPressed = 0;
	 int dPressed = 0;
	 int voorAchterRichting;
	 int stuurRichting;
	 
	 leftOver -= 128;
	
	 if(leftOver >= 8){
		wPressed = 1;
		leftOver -= 8;
	 }
	 if(leftOver >= 4){
		 aPressed = 1;
		 leftOver -= 4;
	 }
	 if(leftOver >= 2){
		 sPressed =  1;
		 leftOver -= 2;
	 }
	 if(leftOver >= 1){
		 dPressed = 1;
		 
	 }
	if(wPressed & sPressed){
		voorAchterRichting = 0;
	}else if(wPressed){
		voorAchterRichting = 1;
	}else if(sPressed){
		voorAchterRichting = -1;
	}else{
		voorAchterRichting = 0;
	}

	if(aPressed & dPressed){
		stuurRichting = 0;
	}else if(aPressed){
		stuurRichting = -1;
	}else if(dPressed){
		stuurRichting = 1;
	}else{
		stuurRichting = 0;
	}
	
	
	if((voorAchterRichting == 1) & (stuurRichting == 0)) {
		setMotors(200, 200);
	}else if((voorAchterRichting == 0) & (stuurRichting == 1)){
		setMotors(200, -200);
	}else if((voorAchterRichting == 0) & (stuurRichting == -1)){
		setMotors(-200, 200);
	}else if((voorAchterRichting == -1) & (stuurRichting == 0)){
		setMotors(-200, -200);
	}else if((voorAchterRichting == 1) & (stuurRichting == 1)){
		setMotors(200, 100);
	}else if((voorAchterRichting == 1) & (stuurRichting == -1)){
		setMotors(100, 200);
	}else if((voorAchterRichting == -1) & (stuurRichting == 1)){
		setMotors(-200, -100);
	}else if((voorAchterRichting == -1) & (stuurRichting == -1)){
		setMotors(-100, 200);
	}else{
		setMotors(0,0);
	}
	
		
	
	 
	
	 
	 setMotors(leftDSpeed, rightDSpeed);
 }
 
 
 

 /************************************************************************/
 /* chances the speed of the right motor on timer0 COMPA interupt        */
 /************************************************************************/
 /*
 ISR(TIMER0_COMP_vect){
	int rightAcceleration = (MOTORSPEED_R - rightDSpeed) / MOTOR_ADJUST_FREQUENTIE;
	int leftAcceleration = (MOTORSPEED_L - leftDSpeed) / MOTOR_ADJUST_FREQUENTIE;
	
	rightDSpeed += rightAcceleration;
	rightDSpeed += leftAcceleration;
	
	setMotors(leftDSpeed, rightDSpeed);
 }
 */
 
 //ISR(TWI_vect) {
//	 slaaftwi();
// }
 
 //ISR(USART_RXC_vect){
//	  
//}


/*slave heeft data ontvangen van de master
 data[] een array waarin de ontvangen data staat
 tel het aantal bytes dat ontvangen is*/
 
void ontvangData(uint8_t data[],uint8_t tel){
	for(int i=0;i<tel;++i)
	    data_ont[i]=data[i];
	data_flag = TRUE;
	writeString("o\n\r");
}

/* het byte dat de slave verzend naar de master
in dit voorbeeld een eenvoudige teller
*/

uint8_t verzendByte() {
		return 1;
}