/*
 * servo.cpp
 *
 *  Created on: May 4, 2023
 *      Author: nikita
 */

#include "main.h"
#include "servo.h"

void servo_motor::change_value(double angle)
{
	new_value = ((angle/(2*PI))*((MAX_SERVO + 1 + MIN_SERVO + 1)/2)) + MIN_SERVO;
}

void servo_motor::refresh_value()
{
	if((htim_ptr == nullptr) || (Channel == 0))
		return;

	TIM_OC_InitTypeDef sConfigOC = {0};

	sConfigOC.OCMode = TIM_OCMODE_PWM1;
	sConfigOC.Pulse = new_value;
	sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
	sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;

	if (HAL_TIM_PWM_ConfigChannel(htim_ptr, &sConfigOC, Channel) != HAL_OK)
	{
		Error_Handler();
	}
}


