/*
 * elevator.h
 *
 *  Created on: Apr 10, 2025
 *      Author: microsoft
 */

#ifndef INC_ELEVATOR_H_
#define INC_ELEVATOR_H_

#include "main.h"

void init_priority(void);
void init_elevator(void);
void elevator_main(void);
void update_priority(void);
void update_goal(void);
void btn_event(void);
void floor_event(void);
void stepmotor_driver(uint8_t direction);
void stepmotor_all_off(void);
void resume(void);
void pause(void);
void update_direction(void);


extern uint8_t state;
uint8_t btn_event_flag = 0;
uint8_t floor_event_flag = 0;

extern volatile int elevator_pause_counter;
extern volatile int buzzer_counter;

extern uint8_t floor_event_flag_dotmatrix;

#define STOP		0
#define FORWARD 	1
#define BACKWARD 	2
#define NO_GOAL		10

typedef struct
{
	uint8_t direction;
	uint8_t curr_floor;
	uint8_t goal;
	uint8_t priority[4];
}t_elevator;

extern t_elevator elevator;
uint8_t pause_flag;

extern uint8_t buzzer_play_flag;

#endif /* INC_ELEVATOR_H_ */
