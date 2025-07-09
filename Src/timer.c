/*
 * timer.c
 *
 *  Created on: Mar 31, 2025
 *      Author: microsoft
 */
#include "main.h"

extern TIM_HandleTypeDef htim2;
void delay_us(int us)
{
	__HAL_TIM_SET_COUNTER(&htim2, 0);
	while(__HAL_TIM_GET_COUNTER(&htim2) < us)
		;
}


