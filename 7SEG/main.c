/*------------------------------------------------------------------------------
main.c 

Main file of the microcontroller responsible for operation of
7-segment digital display.
------------------------------------------------------------------------------*/

#include <REGX52.H> /* Special function register declarations */

#include "main.h"

#include "seg.h" /* 7-segment digitial display control */

#include "../lib/comm.h" /* Serial communication control */

static volatile bit timer; /* Stores whether the mixer runs with timer or without */

/*------------------------------------------------
Timer 0 interrupt.
On interrupt change state of blinking segment.
(powered/unpowered)
------------------------------------------------*/
void t0_int(void) interrupt TF0_VECTOR {
	TR0 = 0; /* Stop timer 0 */
	TF0 = 0; /* Reset overflow flag */
	TH0 = 0x00; /* Reset value for 8 higher bits */
	TL0 = 0x00; /* Reset value for 8 lower bits */
	
	seg_display_blink(); /* Blink the display */
	
	TR0 = 1; /* Start timer 0 */
}

/*------------------------------------------------
Timer 1 interrupt.
On interrupt change state of the 
7-segment digital display. (powered/unpowered)
------------------------------------------------*/
void t1_int(void) interrupt TF1_VECTOR {
	TR1 = 0; /* Stop timer 1 */
	
	if(timer == 0) {
		seg_display_animation();
	} else {
		seg_display_loading(); /* Display loading screen */
	}
	
	TR1 = 1; /* Start timer 1 */
}

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
	
	if (SBUF == COMM_RESET) {
		timer = 0;
		TR0 = 0; /* Turn off Timer 0 */
		TR1 = 0; /* Turn off Timer 1 */
		seg_init();
	} else if(seg_bar() != 0 && seg_bar()) {
			timer = 1;
			seg_loading_inc();
	} else if(SBUF == COMM_NO_TIMER) {
		timer = 0;
		TR0 = 1; /* Turn on Timer 0 */
		TR1 = 1; /* Turn on Timer 1 */
	} else if(SBUF == COMM_TIMER) {
		timer = 1;
		TR0 = 1; /* Turn on Timer 0 */
		TR1 = 1; /* Turn on Timer 1 */
	} else if(SBUF == COMM_TIMER_INC) {
		timer = 1;
		seg_loading_inc();
	}	
	
	EA = 1;
}

/*------------------------------------------------
The main C function.
------------------------------------------------*/
void main(void) {
	
	comm_init(); /* Initialise the serial port */
	ES = 1; /* Enable serial interrupts */
	
	/*------------------------------------------------
	Initialize timer 0 in 16 bit counter mode.
	Timer 0 is responsible for generation of clock
	signal controlling blinking or speed of animation
	displayed on 7-segment digital display.
	------------------------------------------------*/
	TMOD |= 0x01; /* Mode 1: 16bit counter */
	ET0 = 1; /* Enable timer 0 interrupt */
	TH0 = 0x00; /* Set value for 8 higher bits */
	TL1 = 0x00; /* Set value for 8 lower bits */
	TR0 = 0;
	
	/*------------------------------------------------
	Initialize timer 1 in 8 bit auto-reload mode.
	Timer 1 is responsible for generation of clock
	signal controlling power flow into
	7-segment digital displays, so that they do not
	constantly consume power, but appear to be
	constantly on to the human eye.
	------------------------------------------------*/
	TMOD |= 0x20; /* Mode 2: Auto-reload */
	ET1 = 1; /* Enable timer 1 interrupt */
	TH1 = 0x00; /* Set 8 bit value for auto-reload */
	TL1 = 0x00; /* Initialize the timer */
	TR1 = 0;
	
	EA = 1; /* Enable global interrupts */
	
	seg_init();
	
	while(1) {;}
}