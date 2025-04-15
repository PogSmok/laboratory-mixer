/*------------------------------------------------------------------------------
comm.c

Source file with implementations of functions controlling
the the serial communcation port.
------------------------------------------------------------------------------*/

#include <REGX52.H> /* Special function register declarations */

#include "trans.h" /* Control transceiver to allow writing onto the bus */

#include "comm.h"

/*------------------------------------------------
Configures serial port to operate in 9-bit
communication mode and sets transceiever
to read by default.
------------------------------------------------*/
void comm_init(void) {
	
	/*------------------------------------------------
	Set mode of serial port to mode 2.
	------------------------------------------------*/
	SM0 = 1; 
	SM1 = 0;
	
	SM2 = 1; /* Activate multiprocess communication */
	
	REN = 1; /* Activate the receiver */
	
	/*------------------------------------------------
	Zero receive/send bits and data bits.
	------------------------------------------------*/
	TB8 = 0;
	RB8 = 0;
	TI = 0;
	RI = 0;
	
	trans_read(); /* Set transceiver to reading by default */
}

/*------------------------------------------------
First sends the message with address of the 
recipient. Then sends the actual value.
Recipient upon recieving the message with its
address must change the SM2 to 0.
------------------------------------------------*/
void comm_send(unsigned char addr, unsigned char message) {
	trans_send(); /* Enable transmitting for this microcontroller */
	
	TB8 = 1; /* Set ninth bit to 1 */
					 /* (all microcontrollers will recieve this message) */
	SBUF = addr; /* Prepare to send the address */
	
	while(TI != 1) {;} /* Wait until the transmission concludes */
	TI = 0; /* Reset the transmission bit */
		
	trans_read();
	trans_send();
		
	TB8 = 0; /* Set ninth bit to 0 */
					 /* (only the microcontroller with SM2 == 0 will recieve this message) */
	SBUF = message; /* Prepare to send the message */
		
	while(TI == 0) {;} /* Wait until the transmission concludes */
	TI = 0; /* Reset the transmission bit */

	trans_read(); /* Go back into receiving once the message is sent */
}