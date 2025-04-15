/*------------------------------------------------------------------------------
seg.c

Source file with implementations of functions controlling the 
7-segment digital display (7SEG-MPX2-CC-BLUE).
------------------------------------------------------------------------------*/

#include <REGX52.H> /* Special function register declarations */

#include "seg.h"

/*------------------------------------------------
Definition of animation frames, for both
7-segment digital displays.
------------------------------------------------*/
#define DISP1_FRAME_0 0x06
#define DISP1_FRAME_1 0x09
#define DISP2_FRAME_0 0x30
#define DISP2_FRAME_1 0x09

/*------------------------------------------------
Since 7-segment digital display is connected
to P0 which is an open drain port it must be
accessed in a specific way.
------------------------------------------------*/
static unsigned char xdata DISP_1 _at_ 0xFD00;
static unsigned char xdata DISP_2 _at_ 0xFE00;

static volatile unsigned char data LOADING_BAR; /* Stores what fraction of mixer timer has passed from 0 (none) to 5 (finished) */
static bit ANIMATION_FRAME; /* Current frame of the animation, animation consists of two frames for each display */

static unsigned char code BAR_STATES[6] = {0x08, 0x14, 0x40, 0x22, 0x01, 0x7F}; /* BAR_STATES[LOADING_BAR] contains currently blinking segment, */
																																							  /*	all segments stored at lesser indexes must be displayed */
unsigned char seg_bar(void) {
	return LOADING_BAR;
}

/*------------------------------------------------
Initializes LOADING_BAR and ANIMATION_FRAME 
to their starting values 0 and 0 respectively.
------------------------------------------------*/
void seg_init(void) {
	LOADING_BAR = 0;
	ANIMATION_FRAME = 0;
}

/*------------------------------------------------
Sends values to both displays representing
current state of the animation and then turns 
off power for the port.
------------------------------------------------*/
void seg_display_animation(void) {
	if(ANIMATION_FRAME == 0) {
			DISP_1 = DISP1_FRAME_0;
			DISP_2 = DISP2_FRAME_0;
	} else {
			DISP_1 = DISP1_FRAME_1;
			DISP_2 = DISP2_FRAME_1;
	}
	
	P0 = 0; /* Stop providing power into the 7-digit display */
}

/*------------------------------------------------
Sends values to both displays representing
current state of the loading screen
and then turns off power for the port.
------------------------------------------------*/
void seg_display_loading(void) {
	unsigned char val = 0x00;
	if(LOADING_BAR == 1) {
		val = BAR_STATES[0];
	} else if(LOADING_BAR == 2) {
		val = BAR_STATES[0] | BAR_STATES[1];
	} else if(LOADING_BAR == 3) {
		val = BAR_STATES[0] | BAR_STATES[1] | BAR_STATES[2];
	} else if(LOADING_BAR == 4) {
		val = BAR_STATES[0] | BAR_STATES[1] | BAR_STATES[2] | BAR_STATES[3];
	} else if(LOADING_BAR == 5) {
		val = 0x00; /* For last state the entire segment blinks */ 
	}
	
	if(ANIMATION_FRAME == 1) {	/* If the blinking segment is lit on */
		val |= BAR_STATES[LOADING_BAR]; /* Add the segment to the displayed value */
	}
	DISP_1 = val;
	DISP_2 = val;
  
	P0 = 0; /* Stop providing power into the 7-digit display */
}

/*------------------------------------------------
Flips value of ANIMATION_FRAME to keep track
of whether the blinking segment is lit or not.
------------------------------------------------*/
void seg_display_blink(void) {
	ANIMATION_FRAME = ~ANIMATION_FRAME;
}

/*------------------------------------------------
Increments LOADING_BAR modulo 6
(size of BAR_STATES).
------------------------------------------------*/
void seg_loading_inc(void) {
	LOADING_BAR = LOADING_BAR+1;
	if(LOADING_BAR == 6) LOADING_BAR = 5;
}
