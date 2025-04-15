/*------------------------------------------------------------------------------
seg.c

Source file with implementations of functions controlling the motor.
------------------------------------------------------------------------------*/

#include <REGX52.H> /* Special function register declarations */

#include "motor.h"

/*------------------------------------------------
Definitions of motor pins
------------------------------------------------*/
#define MOTOR_CLOCKWISE P2_5 /* If on motor will rotate clockwise */
#define MOTOR_CNT_CLOCKWISE P2_6 /* If on motor will rotate counter-clockwise */

/*------------------------------------------------
Rotation speed is implemented using
pulse width modulation with help of timer 0
------------------------------------------------*/
void motor_rotate(void) {
	/*------------------------------------------------
	Initially stop both motors
	------------------------------------------------*/
	MOTOR_ENABLE = 0;
	
	/*------------------------------------------------
	Prepate timer 2
	------------------------------------------------*/
	TR2 = 0; /* In case timer has been running stop it */
	EXEN2 = 0; /* Don't trigger on falling slope of T2EX */
	RCAP2L = 0xFF; /* Set value for 8 lower bits */
	RCAP2H = 0xFF; /* Set value for 8 higher bits */
	ET2 = 1; /* Enable timer 2 interrupt */
	TH2 = 0xFF; /* Initialize the timer */
	TL2 = 0xFF; /* Initialize the timer */
	
	/*------------------------------------------------
	Define direction of rotation for motor.
	------------------------------------------------*/
	MOTOR_CLOCKWISE = 1;
	MOTOR_CNT_CLOCKWISE = 0;
}

void motor_stop(void) {
	MOTOR_CNT_CLOCKWISE = 1;
}

void motor_start(void) {
	MOTOR_CNT_CLOCKWISE = 0;
}