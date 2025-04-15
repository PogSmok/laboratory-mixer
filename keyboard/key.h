/*------------------------------------------------------------------------------
key.h 

Header file for key.c, contains declarations of functions used to control
the keyboard.
------------------------------------------------------------------------------*/

/*------------------------------------------------
Include macro guard
------------------------------------------------*/
#ifndef __KEY_H__
#define __KEY_H__

/*------------------------------------------------
Definition of every single key on keyboard.
------------------------------------------------*/
#define KEY_NULL -1 /* Symbolizes no key being pressed */
#define KEY_0 0
#define KEY_1 1
#define KEY_2 2
#define KEY_3 3
#define KEY_4 4
#define KEY_5 5
#define KEY_6 6
#define KEY_7 7
#define KEY_8 8
#define KEY_9 9
#define KEY_STAR 10 /* '*' key */
#define KEY_HASH 11 /* '#' key */

/*------------------------------------------------
Initializes static variables.
Must be called before using any functions from
key.h
------------------------------------------------*/
void key_init(void);

/*------------------------------------------------
Returns the currently pressed key. 
If a key is being held, function returns 
the key upon press detection and then KEY_NULL
until another key is pressed.
------------------------------------------------*/
unsigned char key_scan(void);

/*------------------------------------------------
Converts value returned by key_scan to a
character representing pressed key.
------------------------------------------------*/
unsigned char key_to_char(unsigned char key);

/*------------------------------------------------
END: #ifndef __KEY_H__
------------------------------------------------*/
#endif