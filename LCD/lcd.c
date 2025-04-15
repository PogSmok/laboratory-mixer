/*------------------------------------------------------------------------------
lcd.c

Source file with implementations of functions controlling 
the LM041L liquid crystal display.
------------------------------------------------------------------------------*/

#include <REGX52.H> /* Special function register declarations */

#include "lcd.h"

/*------------------------------------------------
Labeling pins so they follow labels 
used in documentation of the lcd
------------------------------------------------*/
#define PORT P2 /* Port connected to LCD */
#define D4 P2_0
#define D5 P2_1
#define D6 P2_2
#define D7 P2_3
#define RS P2_4
#define RW P2_5
#define E  P2_6

/*------------------------------------------------
Definition of LCD commands in 8-bit mode
------------------------------------------------*/
#define CLEAR_DISPLAY 0x01
#define ENTRY_MODE_SET 0x06 /* Increment DDRAM and don't shift display on write */
#define DISPLAY_OFF 0x08
#define DISPLAY_ON 0x0C
#define FUNCTION_SET 0x28 /* Sets LCD into 4 rows with 5x8 dots for each character */

/*------------------------------------------------
Sends given 8-bit command, using 4-bit mode.
------------------------------------------------*/
static void lcd_send_cmd(unsigned char cmd) {	
	RS = 0;
	RW = 0;
	
	PORT &= 0xF0; /* Clear all data bits */
	PORT |= (cmd >> 4); /* Write 4 highest bits of cmd */
	E = 1; /* Send data */
	E = 0; /* Finish sending data */
	
	PORT &= 0xF0; /* Clear all data bits */
	PORT |= (cmd & 0x0F); /* Write 4 lowest bits of val */
	E = 1; /* Send data */
	E = 0; /* Finish sending data */
}

/*------------------------------------------------
Sends a character to lcd using 4-bit mode.
The function is semantically the same as lcd_send_cmd(char)
except it sets RS to 1.
------------------------------------------------*/
static void lcd_send_char(unsigned char c) {
	RS = 1;
	RW = 0;
	
	PORT &= 0xF0; /* Clear all data bits */
	PORT |= (c >> 4); /* Write 4 highest bits of char */
	E = 1; /* Send data */
	E = 0; /* Finish sending data */
	
	PORT &= 0xF0; /* Clear all data bits */
	PORT |= (c & 0x0F); /* Write 4 lowest bits of char */
	E = 1; /* Send data */
	E = 0; /* Finish sending data */
}

/*------------------------------------------------
Initializes LCD to work in 4-bit mode,
turns it on and declares ready for receiving
characters to display on screen.
------------------------------------------------*/
void lcd_init(void) {
	PORT = 0x00; /* Clear all data bits */
	
	/*------------------------------------------------
	Program LCD to work in 4-bit mode.
	------------------------------------------------*/
	RS = 0;
	RW = 0;
	D5 = 1;
	E = 1; /* Send data */
	E = 0; /* Finish sending data */
	
	lcd_send_cmd(FUNCTION_SET);
	lcd_send_cmd(DISPLAY_ON);
	lcd_send_cmd(ENTRY_MODE_SET);
}

/*------------------------------------------------
Turns on display, doesn't configure anything.
------------------------------------------------*/
void lcd_on(void) {
	lcd_send_cmd(DISPLAY_ON);
}

/*------------------------------------------------
Turns off display, no data is lost.
------------------------------------------------*/
void lcd_off(void) {
	lcd_send_cmd(DISPLAY_OFF);
}

/*------------------------------------------------
Clears screen of any characters
------------------------------------------------*/
void lcd_clear(void) {
	unsigned char i = 0;
	lcd_send_cmd(CLEAR_DISPLAY);
	
	for(i = 0; i < 100; i++) {;} 	/* since clearing takes a while, wait */
}

/*------------------------------------------------
Before any lcd_write_ can start sending characters
to LCD, cursor must be set to an index where 
the first character will be written.
------------------------------------------------*/
static void cursor_set(unsigned char row, unsigned char column) {
	if(row == 0) lcd_send_cmd(0x80 + column);
	else if(row == 1) lcd_send_cmd(0xc0 + column);
	else if(row == 2) lcd_send_cmd(0x90 + column);
	else lcd_send_cmd(0xd0 + column);
}

/*------------------------------------------------
Writes character at a specified index (more info in lcd.h)
The function doesn't check if row or column are valid.
Provading incorrect indexes will result in character
being displayed at unexpected places.
------------------------------------------------*/
void lcd_write_char_at(unsigned char c, unsigned char row, unsigned char column) {
	cursor_set(row, column);
	lcd_send_char(c);
}

/*------------------------------------------------
Writes an array of characters at a specified index
(more info in lcd.h)
The function doesn't check if row or column are valid.
Provading incorrect indexes will result in characters
being displayed at unexpected places.
The function doesn't verify if array is null terminated
and if the array won't overflow the row.
------------------------------------------------*/
void lcd_write_arr_at(unsigned char* arr, unsigned char row, unsigned char column) {
	cursor_set(row, column);
	while(*arr != 0) {
		lcd_send_char(*arr);
		arr++;
	}
}