/*
 * extint.c
 *
 *  Created on: 2025. 4. 9.
 *      Author: microsoft
 */

/**
  * @brief  EXTI line detection callbacks.
  * @param  GPIO_Pin Specifies the pins connected EXTI line
  * @retval None
  */
#include "main.h"

extern uint8_t state;
extern uint8_t floor_event_flag;
extern uint8_t btn_event_flag;
extern uint8_t floor_event_flag;
typedef struct
{
	uint8_t direction;
	uint8_t curr_floor;
	uint8_t goal;
	uint8_t priority[4];
}t_elevator;
extern t_elevator elevator;

extern uint32_t HAL_GetTick(void); // 시스템 시간(ms) 반환

volatile uint32_t last_btn_time[4] = {0}; // BTN0~BTN3용
#define DEBOUNCE_TIME_MS 50

__weak void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	uint32_t now = HAL_GetTick();

	switch(GPIO_Pin) {
	case BTN0_Pin:
		if (now - last_btn_time[0] < DEBOUNCE_TIME_MS) break;
		last_btn_time[0] = now;
		state ^= 0x01;
		btn_event_flag = 1;
		break;

	case BTN1_Pin:
		if (now - last_btn_time[1] < DEBOUNCE_TIME_MS) break;
		last_btn_time[1] = now;
		state ^= 0x02;
		btn_event_flag = 1;
		break;

	case BTN2_Pin:
		if (now - last_btn_time[2] < DEBOUNCE_TIME_MS) break;
		last_btn_time[2] = now;
		state ^= 0x04;
		btn_event_flag = 1;
		break;
	case BTN3_Pin:
		if (now - last_btn_time[3] < DEBOUNCE_TIME_MS) break;
		last_btn_time[3] = now;
		state ^= 0x08;
		btn_event_flag = 1;
		break;
	case FLOOR_1_Pin:
		elevator.curr_floor = 1;
		floor_event_flag = 1;
		break;
	case FLOOR_2_Pin:
		elevator.curr_floor = 2;
		floor_event_flag = 1;
		break;
	case FLOOR_3_Pin:
		elevator.curr_floor = 3;
		floor_event_flag = 1;
		break;
	case FLOOR_4_Pin:
		elevator.curr_floor = 4;
		floor_event_flag = 1;
		break;
	}
}
