/*------------------------------------------------------------------------------
lcd.h 

Header file for lcd.c, contains declarations of functions used to control
the LM041L liquid crystal display.

Dictionary:
LCD   - Liquid Crystal Display
DDRAM - Display Data Remote Access Memory. Stores characters that are being
		displayed on the LCD. To change displayed characters, change values
		of addressable units in DDRAM. For exact addresses refer to documentation.
------------------------------------------------------------------------------*/

/*------------------------------------------------
Include macro guard
------------------------------------------------*/
#ifndef __LCD_H__
#define __LCD_H__

/*------------------------------------------------
Number of columns where a character can be written
and number of rows of said columns in the LCD.
(each column can contain one and only one character)
------------------------------------------------*/
#define LCD_COLUMN_COUNT 16
#define LCD_ROW_COUNT 4

/*------------------------------------------------
Initializes the LCD and static variables.
Must be called before using any functions from
lcd.h

LCD is initialized into 4-bit mode with
4 character line without a cursor.
------------------------------------------------*/
void lcd_init(void);

/*------------------------------------------------
Turns on the display. Doesn't impact any data 
in the display. Only makes the screen
display characters.
------------------------------------------------*/
void lcd_on(void);

/*------------------------------------------------
Turns off the display. Doesn't impact any data 
in the display. Only makes the screen not
display characters.
------------------------------------------------*/
void lcd_off(void);

/*------------------------------------------------
Removes all characters from the screen.
Specifically sets all DDRAM addresses to character
of space (' '), so that they appear empty.
Continious calling of lcd_clear() and
any of lcd_write_ functions may cause the
screen to blink and appear to stutter.
------------------------------------------------*/
void lcd_clear(void);

/*------------------------------------------------
Writes a character onto the screen, where
row - indicates index line <0, 3>
column - indicates index of character in line <0,15>
    0 1 2 3 4 5 6 7 8 9 A B C D E F  (columns in hex)
 0 | | | | | | | | | | | | | | | | |
 1 | | | | | | | | | | | | | | | | |
 2 | | | | | | | | | | | | | | | | |
 3 | | | | | | | | | | | | | | | | |
(rows) 
 
More precisely writes into specific address
of DDRAM. Other DDRAM addresses will not be changed.
To avoid having leftovers from earlier message(s)
use lcd_clear() to overwrite all addresses,
so they appear empty. (they are set to a space: ' ')
------------------------------------------------*/
void lcd_write_char_at(char c, unsigned char row, unsigned char column);

/*------------------------------------------------
Writes characters onto the screen, where
row - indicates index line <0, 3>
column - indicates index of character in line <0,15>
    0 1 2 3 4 5 6 7 8 9 A B C D E F  (columns in hex)
 0 | | | | | | | | | | | | | | | | |
 1 | | | | | | | | | | | | | | | | |
 2 | | | | | | | | | | | | | | | | |
 3 | | | | | | | | | | | | | | | | |
(rows) 
 
More precisely writes into specific addresses
of DDRAM. Any not explicitly overwritten
addresses will not be changed.
To avoid having leftovers from earlier message(s)
use lcd_clear() to overwrite all addresses, so
they appear empty. (they are set to a space: ' ')
------------------------------------------------*/
void lcd_write_arr_at(char* arr, unsigned char row, unsigned char column);

/*------------------------------------------------
END: #ifndef __LCD_H__
------------------------------------------------*/
#endif