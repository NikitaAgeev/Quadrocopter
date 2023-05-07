/*
 * servo.cpp
 *
 *  Created on: May 4, 2023
 *      Author: nikita
 */

#include "main.h"
#include "servo.h"

#define SET_PWM_CHANNEL(CHANNEL, VAL) htim_ptr->Instance->CCR ## CHANNEL  = VAL;

void servo_motor::change_value(double angle)
{
	new_value = ((angle/(2*PI))*((MAX_SERVO + 1 + MIN_SERVO + 1)/2)) + MIN_SERVO;
}

void servo_motor::refresh_value()
{
	if((htim_ptr == nullptr) || (Channel == 0))
		return;

	switch(Channel)
	{
	case 1:
		SET_PWM_CHANNEL(1, new_value);
		break;
	case 2:
		SET_PWM_CHANNEL(2, new_value);
		break;
	case 3:
		SET_PWM_CHANNEL(3, new_value);
		break;
	case 4:
		SET_PWM_CHANNEL(4, new_value);
		break;
	default:
		break;
	}
}


