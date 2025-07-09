/*
 * my_main.h
 *
 *  Created on: Apr 10, 2025
 *      Author: microsoft
 */

#ifndef INC_MY_MAIN_H_
#define INC_MY_MAIN_H_

typedef struct
{
	uint8_t direction;
	uint8_t curr_floor;
	uint8_t goal;
	uint8_t priority[4];
}t_elevator;

t_elevator elevator;
uint8_t state = 0x00;

#define STEPMOTOR_DELAY 1126;

volatile int elevator_pause_counter = 0;
volatile int dotmatrix_scroll_counter = 0;
volatile int buzzer_counter = 0;

#endif /* INC_MY_MAIN_H_ */
