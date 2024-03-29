/*
 * servo.h
 *
 *  Created on: May 4, 2023
 *      Author: nikita
 */

#ifndef SERVO_H_
#define SERVO_H_

#define MAX_SERVO 1999
#define MIN_SERVO 999
#define PI 		  3.1415

#define MIN_DELL 10

class servo_motor
{
	TIM_HandleTypeDef* htim_ptr = nullptr;

	uint16_t new_value = 1499;

	uint32_t Channel = 0;

public:
	servo_motor() = default;
	servo_motor(TIM_HandleTypeDef* _htim, uint32_t _channel): htim_ptr(_htim), Channel(_channel) {};
	~servo_motor() = default;

	void change_value(double angle);

	void refresh_value();

};



#endif /* SERVO_H_ */
