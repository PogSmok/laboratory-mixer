/*------------------------------------------------------------------------------
seg.h 

Header file for seg.c, contains declarations of functions used to control
the motor.
------------------------------------------------------------------------------*/

/*------------------------------------------------
Include macro guard
------------------------------------------------*/
#ifndef __MOTOR_H__
#define __MOTOR_H__


#define MOTOR_ENABLE P2_4 /* Pin enabling the motor */

/*------------------------------------------------
Prepares for motor rotation.
------------------------------------------------*/
void motor_rotate(void);

/*------------------------------------------------
Stops the motor.
------------------------------------------------*/
void motor_stop(void);

/*------------------------------------------------
Starts the motor.
------------------------------------------------*/
void motor_start(void);


/*------------------------------------------------
END: #ifndef __MOTOR_H__
------------------------------------------------*/
#endif