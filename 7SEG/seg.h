/*------------------------------------------------------------------------------
seg.h 

Header file for seg.c, contains declarations of functions used to control
the 7-segment digital display (7SEG-MPX2-CC-BLUE).
------------------------------------------------------------------------------*/

/*------------------------------------------------
Include macro guard
------------------------------------------------*/
#ifndef __SEG_H__
#define __SEG_H__

/*------------------------------------------------
Initializes static variables.
Must be called before using any functions from
seg.h
------------------------------------------------*/
void seg_init(void);

unsigned char seg_bar(void);

/*------------------------------------------------
Display current frame of the animation.
This function doesn't change the animation frame.
Call seg_display_blink() to enter next frame.
------------------------------------------------*/
void seg_display_animation(void);

/*------------------------------------------------
Displays static (non-blinking) part of
loading screen.
------------------------------------------------*/
void seg_display_loading(void);

/*------------------------------------------------
Displays blinking part of
loading screen.
Each time the function is called the
blinking segment changes its state (on/off).
For animation, changes the current frame.
------------------------------------------------*/
void seg_display_blink(void);

/*------------------------------------------------
Increment progress of the loading bar.
------------------------------------------------*/
void seg_loading_inc(void);

/*------------------------------------------------
END: #ifndef __SEG_H__
------------------------------------------------*/
#endif