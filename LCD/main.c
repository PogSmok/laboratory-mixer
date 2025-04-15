/*------------------------------------------------------------------------------
main.c 

Main file of the microcontroller responsible for operation of the
LM041L liquid crystal display. (LMO41L will be referred from now on as "LCD")
------------------------------------------------------------------------------*/

#include <REGX52.H> /* Special function register declarations */

#include <STDIO.H> /* sprintf() */

#include "main.h"

#include "lcd.h" /* LCD control */

#include "../lib/comm.h" /* Serial communication control */

static volatile unsigned char data state;
static unsigned char data loading_progress; 
static unsigned char data seg_progress;

/*------------------------------------------------------------------------------
All the globals below will be changed with interrupt routine. 
Either by a timer or a serial interrupt. The structure of code guarantees 
these globals will never be changed outside of normal scope of code.
Thus allowing us to not mark them as volatile.
------------------------------------------------------------------------------*/
static unsigned int data timer; /* Stores timer value input by user (in minutes) */
static unsigned int data cur_timer; /* Stores minutes left until the timer concludes */
static unsigned int data cur_seconds; /* Stores the value of last minute in timer in seconds */
static unsigned char data speed_mode; /* Current speed mode of the mixer */
static bit data display_state; /* Stores whether display is on (1) or off (0) */
static bit data timer_0_state; /* Stores if current interrupt of timer is odd */

/*------------------------------------------------------------------------------
Displays a welcome message onto the display of LCD.
"PRESS ANY KEY"
"TO START"
------------------------------------------------------------------------------*/
static void display_welcome(void) {
	lcd_clear();
	lcd_write_arr_at("PRESS ANY KEY", 1, 1);
	lcd_write_arr_at("TO START", 2, 4);
}

/*------------------------------------------------------------------------------
Displays a message onto the display of LCD, prompting user to enter a
numerical value for the speed mode the motor will operate in.
"ENTER ROTATION"
"MODE"
"(FROM 0 TO 9)"
------------------------------------------------------------------------------*/
static void display_select_speed(void) {
	lcd_clear();
	lcd_write_arr_at("ENTER ROTATION", 0, 1);
	lcd_write_arr_at("MODE", 1, 6);
	lcd_write_arr_at("(FROM 0 TO 9)", 2, 2);	
}

/*------------------------------------------------------------------------------
Displays a message onto the display of LCD, prompting user to decide
whether they want to run the mixer with timer or without. 
'0' - means with a timer
default - everything else means without a timer
"DO YOU WANT TO"
"SET A MIX TIMER"
"0 - YES"
"DEFAULT - NO"
------------------------------------------------------------------------------*/
static void display_select_mode(void) {
	lcd_clear();
	lcd_write_arr_at("DO YOU WANT TO", 0, 0);
	lcd_write_arr_at("SET A MIX TIMER", 1, 0);
	lcd_write_arr_at("0 - YES", 2, 0);
	lcd_write_arr_at("DEFAULT - NO", 3, 0);
}

/*------------------------------------------------------------------------------
After the user decides that they don't want the timer, this will be the screen
they will see until they terminate the mixing.
"SPEED MODE: x" - where x is replaced by the value of chosen speed mode
"TIMER: OFF"

DO NOT call this function when speed mode is updated, use update_speed(void) instead.
------------------------------------------------------------------------------*/
static void display_no_timer(void) {
	lcd_clear();
	lcd_write_arr_at("SPEED MODE: ", 0, 0);
	lcd_write_char_at(speed_mode, 0, 12);
	lcd_write_arr_at("TIMER: OFF", 1, 0);
	lcd_write_arr_at("PRESS # TO END", 3, 0);
}

static void update_speed(void) {
	lcd_write_char_at(speed_mode, 0, 12);
}

/*------------------------------------------------------------------------------
After the user decides that they do want the timer, this will be the screen
they will see until they confirm their input timer length. Timer length is input
in minutes.
"TIMER VALUE (MIN)
"x" - where x is currently input timer
"* TO DELETE"
"# TO CONFIRM"

DO NOT call this function when the input timer is changed,
use update_enter_timer(void) instead.
------------------------------------------------------------------------------*/
static void display_enter_timer(void) {
	unsigned char s[6]; /* 5 is the maximum number of digits an unsigned int can hold */
	sprintf(s, "%u", timer);
	lcd_clear();
	lcd_write_arr_at("SET TIMER (MIN)", 0, 0);
	lcd_write_arr_at(s, 1, 0);
	lcd_write_arr_at("* TO DELETE", 2, 0);
	lcd_write_arr_at("# TO CONFIRM", 3, 0);
}

/*------------------------------------------------------------------------------
Changes the value of the timer displayed on display_enter_timer(void) when 
the user modifies it in any way with the keyboard.
------------------------------------------------------------------------------*/
static void update_enter_timer(void) {
		unsigned char s[6] = {' ',' ',' ',' ',' ',0}; /* 5 is the maximum number of digits an unsigned int can hold */
		lcd_write_arr_at(s, 1, 0);
		sprintf(s, "%u", timer);
		lcd_write_arr_at(s, 1, 0);
}

/*------------------------------------------------------------------------------
After the timer is set, this will be the visible screen until the timer runs out.
"SPEED MODE: x" - where x is replaced by the value of chosen speed mode
"TIMER: HH:MM:SS" - where HH:MM:SS are hours, minutes and seconds left from the timer
"[..............]" - progression bar of the timer,
										 once a progress is made '.' is changed to '='

DO NOT call this function when the value of the timer is changed (a second passes),
use update_timer(void) instead.
------------------------------------------------------------------------------*/
static void display_timer(void) {
	unsigned char s[9];
	sprintf(s, "%02u:%02u:%02u", timer/60, timer%60, 0);
	lcd_clear();
	lcd_write_arr_at("SPEED MODE: ", 0, 0);
	lcd_write_char_at(speed_mode, 0, 12);
	lcd_write_arr_at("TIMER: ", 1, 0);
	lcd_write_arr_at(s, 1, 7);
	lcd_write_arr_at("[..............]", 2, 0);
	lcd_write_arr_at("PRESS # TO END", 3, 0);
}

/*------------------------------------------------------------------------------
After the timer runs out user is prompted to press # to reset the device.
"TIMER ENDED"
"PRESS #"
------------------------------------------------------------------------------*/
static void display_timer_end(void) {
	lcd_clear();
	lcd_write_arr_at("TIMER ENDED", 0, 0);
	lcd_write_arr_at("PRESS #", 1, 0);
}

/*------------------------------------------------------------------------------
Changes the value of the timer displayed on display_timer(void) after a
quantum of time passes. (one second)
------------------------------------------------------------------------------*/
static void update_timer(void) {
	unsigned char s[9];
	float perc;
	
	if(cur_seconds == 0 && cur_timer == 0) {
		TR0 = 0;
		state = STATE_TIMER_END;
		display_timer_end();
		comm_send(MTR_ID, COMM_TIMER_END);
		comm_send(MTR_ID, COMM_TIMER_END);
		return;
	}
	
	if(cur_seconds != 0) cur_seconds--;
	else if(cur_timer != 0) {
		cur_timer--;
		cur_seconds = 59;
	}
	sprintf(s, "%02u:%02u:%02u", cur_timer/60, cur_timer%60, cur_seconds);
	lcd_write_arr_at(s, 1, 7);
	
	perc = timer-cur_timer;
	perc = perc - (float)(cur_seconds)/60;
	perc = perc/timer;
	if(perc > 0.0714*(loading_progress+1)) { /* 0.0714 == 1/14   */
                                             /* There is 14 bars */
		loading_progress++;
		lcd_write_char_at('=', 2, loading_progress);
	}
	
	if(perc > 0.1666*(seg_progress+1)) { /* 0.1666 == 1/6 */
                                         /* 7SEG increments it's bars, there is 6 bars in 7SEG */
		seg_progress++;
		comm_send(SEG_ID, COMM_TIMER_INC);
	}
}

/*------------------------------------------------------------------------------
On button press turn on/off the display.
------------------------------------------------------------------------------*/
void IE1_int(void) interrupt IE1_VECTOR {
	if(display_state == 1) {
		lcd_off();
	} else {
		lcd_on();
	}
	display_state = ~display_state;
}

/*------------------------------------------------------------------------------
This timer overflows every second. It is used for measuring when timer concludes.
------------------------------------------------------------------------------*/
void TF0_int(void) interrupt TF0_VECTOR {
	TR0 = 0; /* Stop timer 0 */
	TF0 = 0; /* Reset overflow flag */
	TH0 = 0x00; /* Reset value for 8 higher bits */
	TL0 = 0x00; /* Reset value for 8 lower bits */
	
	timer_0_state = ~timer_0_state;
	if(timer_0_state == 0) update_timer();
	
	TR0 = 1;
}

/*------------------------------------------------------------------------------
Triggers on message transmission from keyboard, indicating change of state.
Since serial port is configured in 9-bit multiprocess communication mode.
Each message consists of two messages. First one contains address of the 
recipient microcontroller, second one the actual message. 
After reading the address addressed microcontroller changes SM2 to 0.
------------------------------------------------------------------------------*/
void SIO_int(void) interrupt SIO_VECTOR {
	while(RI != 1) {;} /* Wait until the recieving concludes */
	RI = 0; /* Reset recieving bit */
		
	if(SM2 == 1) {
		if(SBUF == COMM_ID) SM2 = 0;
		return;
	}
	SM2 = 1;

	if(state == STATE_STANDBY) {
		state = STATE_SELECT_SPEED;
		display_select_speed();
		
	} else if(state == STATE_SELECT_SPEED) {
		state = STATE_SELECT_MODE;
		display_select_mode();
		speed_mode = SBUF;
		
	} else if(state == STATE_SELECT_MODE) {
		if(SBUF != '0') {
			state = STATE_NO_TIMER;
			display_no_timer();
			
			/* Inform SEG and MOTOR to start working */
			comm_send(SEG_ID, SEG_NO_TIMER);
			comm_send(MTR_ID, speed_mode-'0');
		} else {
			state = STATE_ENTER_TIMER;
			timer = 0;
			cur_seconds = 0;
			display_enter_timer();
		}
		
	} else if(state == STATE_NO_TIMER || state == STATE_TIMER) {
		if(SBUF == COMM_RESET) {
			TR0 = 0; /* Stop timer 0 */
			state = STATE_STANDBY;
			display_welcome();
			return;
		}
		speed_mode = SBUF;
		update_speed();
		
	} else if(state == STATE_ENTER_TIMER) {
			if(SBUF == '*') {
				timer /= 10;
				update_enter_timer();
			} else if(SBUF == '#') {
				state = STATE_TIMER;
				cur_timer = timer;
				timer_0_state = 0;
				loading_progress = 0;
				seg_progress = 0;
				cur_seconds = 0;
				TMOD |= 0x01; /* Mode 1: 16bit counter */
				TH0 = 0xFF; /* Set value for 8 higher bits */
				TL0 = 0xF0; /* Set value for 8 lower bits */
				ET0 = 1; /* Enable timer 0 interrupt */
				TR0 = 1; /* Start timer 0 */
				display_timer();
				
				/* Inform SEG and MOTOR to start working */
				comm_send(SEG_ID, SEG_TIMER);
				comm_send(MTR_ID, speed_mode-'0');
			} else {
				if((timer-'0'+SBUF)*10/10 != timer-'0'+SBUF) return;
				timer = timer*10 + SBUF - '0';
				update_enter_timer();
			}
	} else if(state == STATE_TIMER_END) {
			if(SBUF == COMM_RESET) {
				TR0 = 0; /* Stop timer 0 */
				state = STATE_STANDBY;
				display_welcome();
				return;
			}
	}
}

/*------------------------------------------------
The main C function.
------------------------------------------------*/
void main(void) {
	state = STATE_STANDBY;
	display_state = 1; /* Display is on by default */
	
	lcd_init();
	
	comm_init(); /* Initialize serial communication port */
	ES = 1; /* Enable serial interrupt */
	
	IT1 = 1; /* Send interrupt 1, only on falling edge H->L */
	EX1 = 1; /* Enable external interrupt 1 */
	EA = 1; /* Enable global interrupts */
	
	display_welcome();
	while(1){;}
}