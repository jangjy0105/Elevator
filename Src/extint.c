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
	/* 버튼에 의한 인터럽트 */
	case BTN0_Pin:
		if (now - last_btn_time[0] < DEBOUNCE_TIME_MS) break; /* 버튼 디바운싱 */
		/* 버튼 디바운싱을 위한 last_btn_time 업데이트 */
		last_btn_time[0] = now;
		/*
		 *	state의 0번 비트 toggle
		 *	state는 예약된 층들을 나타내는 변수
		 *	state의 n번 비트1가 set 되었다면 (n+1)층이 예약되어 언젠가는(n+1)층에 도착해야 한다는 의미
		 */
		state ^= 0x01;
		/* 버튼 이벤트 플래그 set, 해당 플래그가 set 되었다면 엘리베이터 제어 태스크에서 버튼 이벤트 처리 */
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

	/* 포토센서에 의한 인터럽트(각 층 도착 시 발생) */
	case FLOOR_1_Pin:
		/* 현재 층 업데이트 */
		elevator.curr_floor = 1;
		/* 층 도착 이벤트 플래그 set, 해당 플래그가 set 되었다면 엘리베이터 제어 태스크에서 층 도착 이벤트 처리 */
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
