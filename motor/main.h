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
#define COMM_ID 3

/*------------------------------------------------
List of possible communication messages
received in the serial port
------------------------------------------------*/
#define COMM_RESET 0xFF /* Reset the state of the microcontroller */
#define COMM_TIMER_END 0x0A /* The timer has concluded */
/* - */ /* Message with numercial value of currently chosen speed mode */

/*------------------------------------------------
END: #ifndef __MAIN_H__
------------------------------------------------*/
#endif