/*------------------------------------------------------------------------------
main.h

Header file for main.c with declarations of macros and global variables.
------------------------------------------------------------------------------*/

/*------------------------------------------------
Include macro guard
------------------------------------------------*/
#ifndef __MAIN_H__
#define __MAIN_H__

/*------------------------------------------------
ID of microcontroller used in communication
------------------------------------------------*/
#define COMM_ID 2

/*------------------------------------------------
IDs of other microcontrollers
------------------------------------------------*/
#define KEY_ID 0
#define SEG_ID 1
#define MTR_ID 3

/*------------------------------------------------
List of possible communication messages
received in the serial port
------------------------------------------------*/
#define COMM_RESET 0xFF /* Reset the state of the microcontroller */
#define COMM_DELETE 0x0A /* Remove last digit of current timer value */
#define COMM_CONFIRM 0x0B /* Confirm currently set value for timer */
/* - */ /* Message with numercial value of currently pressed key on keyboard */

/*------------------------------------------------
List of possible communication messages
sent through the serial port.
------------------------------------------------*/
#define SEG_NO_TIMER 0x01 /* Mixer has started without a timer */
#define SEG_TIMER 0x02 /* Mixer has started with a timer */
#define COMM_TIMER_INC 0x03 /* Another 16.66% of timer has passed, increase LOADING_BAR */
#define COMM_TIMER_END 0x0A /* The timer has concluded */

/*------------------------------------------------
Declaration of states of the program
------------------------------------------------*/
#define STATE_STANDBY 0 /* Starting state */
#define STATE_SELECT_SPEED 1 /* Select speed of rotation */
#define STATE_SELECT_MODE 2 /* Select whether to mix with timer or without */
#define STATE_NO_TIMER 3 /* Mixer is running without a timer */
#define STATE_ENTER_TIMER 4 /* Enter value of the timer */
#define STATE_TIMER 5 /* Mixer is running with a timer */
#define STATE_TIMER_END 6 /* The timer has concluded */

/*------------------------------------------------
END: #ifndef __MAIN_H__
------------------------------------------------*/
#endif