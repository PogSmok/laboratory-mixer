/*------------------------------------------------------------------------------
key.c

Source file with implementations of functions controlling the keyboard.
------------------------------------------------------------------------------*/

#include <REGX52.H> /* Special function register declarations */

#include "key.h"

#define KEYBOARD P2 /* Keyboard port */

/*------------------------------------------------
Definition of keyboard columns, which
key_scan() iterates on
------------------------------------------------*/
#define COLUMN_1 P2_3
#define COLUMN_2 P2_2
#define COLUMN_3 P2_1 

static unsigned char LAST_SCANNED; /* Stores last scanned key, to detect whether a key is being held down */

/*------------------------------------------------
Initializes LAST_SCANNED
to its starting value KEY_NULL.
------------------------------------------------*/
void key_init(void) {
	LAST_SCANNED = KEY_NULL;
}

/*------------------------------------------------
Keyboard works on negative logic, so no current
means a key is pressed. 
If a key has been pressed change state of 
LAST_SCANNED. As long as pressed
key == LAST_SCANNED return KEY_NULL to prevent
sending multiple signals on key hold.
------------------------------------------------*/
unsigned char key_scan(void) {
	
	unsigned char pressed_key = KEY_NULL;
	
	/*------------------------------------------------
	Scanning column 1 
	
	|
	1 2 3
	4 5 6
	7 8 9
	* 0 #
	|
	
	------------------------------------------------*/
	COLUMN_1 = 0; /* Keyboard works on negative logic */
	COLUMN_2 = 1;
	COLUMN_3 = 1;
	if(KEYBOARD == 0xE7) { /* '1' */
		pressed_key = KEY_1;
	} else if(KEYBOARD == 0xD7) { /* '4' */
		pressed_key = KEY_4;
	} else if(KEYBOARD == 0xB7) { /* '7' */
		pressed_key = KEY_7;
	} else if(KEYBOARD == 0x77) { /* '*' */
		pressed_key = KEY_STAR;
	}
	
	/*------------------------------------------------
	Scanning column 2 
	
	  |
	1 2 3
	4 5 6
	7 8 9
	* 0 #
	  |
	
	------------------------------------------------*/
	COLUMN_1 = 1;
	COLUMN_2 = 0; /* Keyboard works on negative logic */
	COLUMN_3 = 1;
	if(KEYBOARD == 0xEB) { /* '2' */
		pressed_key = KEY_2;
	} else if(KEYBOARD == 0xDB) { /* '5' */
		pressed_key = KEY_5;
	} else if(KEYBOARD == 0xBB) { /* '8' */
		pressed_key = KEY_8;
	} else if(KEYBOARD == 0x7B) { /* '0' */
		pressed_key = KEY_0;
	}
	
	/*------------------------------------------------
	Scanning column 3
	
	    |
	1 2 3
	4 5 6
	7 8 9
	* 0 #
	    |
	
	------------------------------------------------*/
	COLUMN_1 = 1;
	COLUMN_2 = 1; 
	COLUMN_3 = 0; /* Keyboard works on negative logic */
	if(KEYBOARD == 0xED) { /* '3' */
		pressed_key = KEY_3;
	} else if(KEYBOARD == 0xDD) { /* '6' */
		pressed_key = KEY_6;
	} else if(KEYBOARD == 0xBD) { /* '9' */
		pressed_key = KEY_9;
	} else if(KEYBOARD == 0x7D) { /* '#' */
		pressed_key = KEY_HASH;
	}
	
	if(pressed_key == LAST_SCANNED) return KEY_NULL;
	LAST_SCANNED = pressed_key;
	return pressed_key;
}

/*------------------------------------------------
If provided key is not a valid key, returns 0
------------------------------------------------*/
unsigned char key_to_char(unsigned char key) {
	if(key == KEY_0) return '0';
	if(key == KEY_1) return '1';
	if(key == KEY_2) return '2';
	if(key == KEY_3) return '3';
	if(key == KEY_4) return '4';
	if(key == KEY_5) return '5';
	if(key == KEY_6) return '6';
	if(key == KEY_7) return '7';
	if(key == KEY_8) return '8';
	if(key == KEY_9) return '9';
	if(key == KEY_STAR) return '*';
	if(key == KEY_HASH) return '#';
	return 0;
}