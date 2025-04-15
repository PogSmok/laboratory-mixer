/*------------------------------------------------------------------------------
comm.h 

Header file for comm.c, contains declarations of functions used to control
the serial communcation port.

comm_read(void) is not implemented here and must be implemented
in each microcontroller's main.c separately, as the function will greatly differ
for each one.
------------------------------------------------------------------------------*/

/*------------------------------------------------
Include macro guard
------------------------------------------------*/
#ifndef __COMM_H__
#define __COMM_H__

/*------------------------------------------------
Initializes the serial port.
Must be called before using any functions from
comm.h
------------------------------------------------*/
void comm_init(void);

/*------------------------------------------------
Sends a message to microcontroller of given
address. For this to work all other microcontrollers
must be initialized with comm_init(void) beforehand.
------------------------------------------------*/
void comm_send(unsigned char addr, unsigned char message);

/*------------------------------------------------
END: #ifndef __COMM_H__
------------------------------------------------*/
#endif