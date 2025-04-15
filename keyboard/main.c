/*------------------------------------------------------------------------------
main.c 

Main file of the microcontroller responsible for operation of the
keyboard.
------------------------------------------------------------------------------*/

#include <REGX52.H> /* Special function register declarations */

#include "main.h"

#include "key.h" /* Keyboard control */

#include "../lib/comm.h" /* Serial communication control */

static unsigned char data state;

/*------------------------------------------------
The main C function.
------------------------------------------------*/
void main(void) {
	unsigned char c;
	
	state = STATE_STANDBY;
	
	key_init(); /* Initialize keyboard */
	
	comm_init(); /* Initialize serial communication port */
	ES = 1; /* Enable serial interrupt */
	EA = 1; /* Enable global interrupts */
	
	while(1) {
		c = key_scan();
		if(c != KEY_NULL) {
			if(state == STATE_STANDBY) {
					comm_send(LCD_ID, key_to_char(c));
					state = STATE_SELECT_SPEED;
			} else if(state == STATE_SELECT_SPEED) {
					if(c == KEY_STAR || c == KEY_HASH) continue;
					state = STATE_SELECT_MODE;
					comm_send(LCD_ID, key_to_char(c));
			} else if(state == STATE_SELECT_MODE) {
					if(c != KEY_0) {
						state = STATE_NO_TIMER;
						comm_send(LCD_ID, key_to_char(c));
					} else {
						state = STATE_ENTER_TIMER;
						comm_send(LCD_ID, key_to_char(c));
					}
			} else if(state == STATE_NO_TIMER || state == STATE_TIMER) {
					if(c == KEY_STAR) continue;
					if(c == KEY_HASH) {
						comm_send(SEG_ID, COMM_RESET);
						comm_send(MTR_ID, COMM_RESET);
						comm_send(LCD_ID, COMM_RESET);
						comm_send(SEG_ID, COMM_RESET);
						comm_send(MTR_ID, COMM_RESET);
						state = STATE_STANDBY;
					} else {
						comm_send(MTR_ID, key_to_char(c)-'0');
						comm_send(LCD_ID, key_to_char(c));
						comm_send(MTR_ID, key_to_char(c)-'0');
					}
			} else if(state == STATE_ENTER_TIMER) {
					c = key_to_char(c);
					if(c ==  '#') {
						state = STATE_TIMER;
					}
					comm_send(LCD_ID, c);
			}
		}
	}
}