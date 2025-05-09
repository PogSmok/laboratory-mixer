/*------------------------------------------------------------------------------
trans.c

Source file with implementations of functions controlling
the MAX487 transceiver.
------------------------------------------------------------------------------*/

#include <REGX52.H> /* Special function register declarations */

#include "trans.h"

/*------------------------------------------------
DE = 1 (P3_4)
RE = 0 (~P3_4)
------------------------------------------------*/
void trans_send(void) {
	P3_4 = 1;
}

/*------------------------------------------------
DE = 0 (P3_4)
RE = 1 (~P3_4)
------------------------------------------------*/
void trans_read(void) {
	P3_4 = 0;
}