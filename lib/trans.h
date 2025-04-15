/*------------------------------------------------------------------------------
trans.h 

Header file for key.c, contains declarations of functions used to control
the MAX487 transceiver.
------------------------------------------------------------------------------*/

/*------------------------------------------------
Include macro guard
------------------------------------------------*/
#ifndef __TRANS_H__
#define __TRANS_H__

/*------------------------------------------------
Enable driver of the transceiver,
allowing to write data onto the bus.
Only one transceiver should be in 
the writing state at once.
------------------------------------------------*/
void trans_send(void);

/*------------------------------------------------
Enable receiver of the transceiver,
allowing to read data from the bus.
------------------------------------------------*/
void trans_read(void);

/*------------------------------------------------
END: #ifndef __TRANS_H__
------------------------------------------------*/
#endif