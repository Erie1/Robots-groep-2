/*
 * robot groep 2.c
 *
 * Created: 9-3-2017 20:00:45
 * Author : Erik
 */ 
 
 #include "rp6aansluitingen.h"
// #include "i2c.h"

 #include <stdint.h>
 #include <util/delay.h>
 #include <avr/io.h>
 #include <avr/interrupt.h>


 #define MAXSPEED		255
 #define MOTORSPEED_R	OCR1A
 #define MOTORSPEED_L	OCR1B
 
 #define MOTOR_ADJUST_FREQUENTIE   100
 #define MOTOR_ADJUST_DELAY		F_CPU / MOTOR_ADJUST_FREQUENTIE / 1024
 
 #define SCL_frequentie 100000

 #define BAUDRATE		38400
 #define UBRR_BAUD	(((long)F_CPU/((long)16 * BAUDRATE))-1)
 #define resetData()  for(uint8_t i=0;i<20;++i) data[i]=0
 
 void (*ontfunc) (uint8_t[],uint8_t);
 uint8_t (*verfunc) ();
 //i2c prototypes
 
 void init_i2c_slave(uint8_t ad);
 void init_i2c_ontvang( void (*ontvanger) (uint8_t [],uint8_t));
 void init_i2c_verzend( uint8_t (*verzender) ());
 void slaaftwi();

void ontvangData(uint8_t data[],uint8_t tel);
uint8_t verzendByte();

 void initUSART();
 void writeInteger(int16_t number, uint8_t base);
 void writeString(char *string);
 
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


 int main(void)
 {
	 initMotors();
	 //initCommunication();
	 initUsartPC();
	 
	init_i2c_slave(8);
	
	/*ontvangData is de functie die uitgevoerd wordt 
	wanneer een byte via de i2c bus ontvangen wordt
	*/
	init_i2c_ontvang(ontvangData); 
	
	/*verzendByte is de functie die aangeroepen wordt
	wanneer de slave een byte naar de master verzend*/
	init_i2c_verzend(verzendByte);
	
	sei(); //De slave van i2c werkt met interrupt
	

	 // TEST
	 rightDSpeed = 255;
	 leftDSpeed = 255;
	 /*for(int i = 0; i < 40; i++){
		 _delay_ms(250);
	 }
	 emergencyBrake();*/
	 int i = 0;
	 while (1)
	 {
		
		
		writeInteger(i, 10);
		i++;
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

	 TIMSK = 1 << OCIE1A | (1 << OCIE1B);	// enable the timer interrupt mask bits


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
 ISR(TIMER0_COMP_vect){
	int rightAcceleration = (MOTORSPEED_R - rightDSpeed) / MOTOR_ADJUST_FREQUENTIE;
	int leftAcceleration = (MOTORSPEED_L - leftDSpeed) / MOTOR_ADJUST_FREQUENTIE;
	
	rightDSpeed += rightAcceleration;
	rightDSpeed += leftAcceleration;
	
	setMotors(leftDSpeed, rightDSpeed);
 }
 
 //ISR(TWI_vect) {
//	 slaaftwi();
// }
 
 //ISR(USART_RXC_vect){
//	  
//}



void init_i2c_slave(uint8_t ad) {
	
	TWSR = 0;
	TWBR = ((F_CPU / SCL_frequentie) - 16) / 2;
	TWCR = (1 << TWIE) | (1 << TWEN) | (1 << TWEA);
	TWAR = ad<<1;
}

void slaaftwi() {
	static uint8_t data[40];
	static uint8_t teller=0;
	switch(TWSR) {
		case 0x10:
		case 0x08:
		break;
		
		case 0x60:

		teller=0;

		break;
		case 0x68:

		break;
		case 0x80:
		data[teller++] = TWDR;

		break;
		case 0xA0:
		ontfunc(data,teller);
		resetData();
		break;
		case 0xA8:
		teller=0;
		TWDR=verfunc();
		break;
		case 0xB8:
		TWDR=verfunc();
		break;
		case 0xC0:   //NACK
		break;
		case 0xC8:
		break;
	}
	TWCR |= (1<<TWINT);    // Clear TWINT Flag
}

void initUSART() {

	UBRRH = UBRR_BAUD >> 8;
	UBRRL = (uint8_t) UBRR_BAUD;
	UCSRA = 0x00;
	UCSRC = (1<<URSEL)|(1<<UCSZ1)|(1<<UCSZ0);
	UCSRB = (1 << TXEN) | (1 << RXEN);
	writeString("usart werkt nog\n\r");
}


void init_i2c_ontvang( void (*ontvanger) (uint8_t [],uint8_t)) {
	ontfunc=ontvanger;
}

void init_i2c_verzend( uint8_t (*verzender) ()) {
	verfunc=verzender;
}

void writeChar(char ch)
{
	while (!(UCSRA & (1<<UDRE)));
	UDR = (uint8_t)ch;
}

void writeString(char *string)
{
	while(*string)
	writeChar(*string++);
}

void writeInteger(int16_t number, uint8_t base)
{
	char buffer[17];
	itoa(number, &buffer[0], base);
	writeString(&buffer[0]);
}

 
 void ontvangData(uint8_t data[],uint8_t tel){
	
 }

 /* het byte dat de slave verzend naar de master
 in dit voorbeeld een eenvoudige teller
 */


 uint8_t verzendByte() {
	 return 0;
 }
 




