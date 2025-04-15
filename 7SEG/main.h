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
#define COMM_ID 1

/*------------------------------------------------
List of possible communication messages
received in the serial port
------------------------------------------------*/
#define COMM_RESET 0xFF /* Reset the state of the microcontroller */
#define COMM_NO_TIMER 0x01 /* Mixer has started without a timer */
#define COMM_TIMER 0x02 /* Mixer has started with a timer */
#define COMM_TIMER_INC 0x03 /* Another 16.66% of timer has passed, increase LOADING_BAR */

/*------------------------------------------------
END: #ifndef __MAIN_H__
------------------------------------------------*/
#endif