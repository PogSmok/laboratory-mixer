/*------------------------------------------------------------------------------
main.c 

Main file of the microcontroller responsible for operation of motor.
------------------------------------------------------------------------------*/

#include <REGX52.H> /* Special function register declarations */

#include "main.h"

#include "motor.h" /* motor control */

#include "../lib/comm.h" /* Serial communication control */

static volatile unsigned char data speed;
static unsigned char pwm_state;

/*------------------------------------------------------------------------------
Triggers on message transmission from keyboard, indicating change of state.
Since serial port is configured in 9-bit multiprocess communication mode.
Each message consists of two messages. First one contains address of the 
recipient microcontroller, second one the actual message. 
After reading the address addressed microcontroller changes SM2 to 0.
------------------------------------------------------------------------------*/
void SIO_int(void) interrupt SIO_VECTOR {
	EA = 0;
	
	while(RI != 1) {;} /* Wait until the recieving concludes */
	RI = 0; /* Reset recieving bit */
		
	if(SM2 == 1) {
		if(SBUF == COMM_ID) SM2 = 0;
		EA = 1;
		return;
	}
	SM2 = 1;

	if(SBUF == COMM_RESET) {
		motor_stop();
		TR2 = 0;
		MOTOR_ENABLE = 0;
		
		/* Turn off the lamps */
		P2_3 = 0;
		P2_2 = 0;
		P2_1 = 0;
	} else if(SBUF == COMM_TIMER_END) {
		motor_stop();
		TR2 = 0;
		MOTOR_ENABLE = 0;
		
		/* Turn on the lamps */
		P2_3 = 1;
		P2_2 = 1;
		P2_1 = 1;
	} else {
		motor_start();
		TR2 = 1;
		speed = SBUF;
	}
	EA = 1;
}

void t2_int(void) interrupt TF2_VECTOR {
	if(pwm_state < speed*10 + 10) {
		MOTOR_ENABLE = 1;
	} else {
		MOTOR_ENABLE = 0;
	}
	
	pwm_state++;
}

/*------------------------------------------------
The main C function.
------------------------------------------------*/
void main(void) {
	comm_init(); /* Initialise the serial port */
	
	/* Turn off the lamps */
	P2_3 = 0;
	P2_2 = 0;
	P2_1 = 0;
	
	ES = 1; /* Enable serial interrupts */
	EA = 1; /* Enable global interrutps */
	
	speed = 0;
	motor_rotate();
	
	while(1) {;}
}