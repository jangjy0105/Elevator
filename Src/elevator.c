/*
 * elevator.c
 *
 *  Created on: Apr 10, 2025
 *      Author: microsoft
 */
#include "elevator.h"
#define MAX_FLOOR 4

void init_priority(void)
{
	elevator.priority[0] = 1;
	elevator.priority[1] = 2;
	elevator.priority[2] = 3;
	elevator.priority[3] = 4;
}

void init_elevator(void)
{
	elevator.direction = STOP;
	elevator.curr_floor = 1;
	elevator.goal = NO_GOAL;
	init_priority();
	state = 0x00;
}

void elevator_main(void)
{
	btn_event();
	floor_event();
	if (pause_flag) resume();
	else
		stepmotor_driver(elevator.direction);
}

void update_priority(void)
{
	if(elevator.direction == FORWARD)
	{
		uint8_t temp = elevator.curr_floor;
		int increament = 1;
		for(int i=0; i<MAX_FLOOR; i++)
		{
			temp += increament;
			if(temp > MAX_FLOOR)
			{
				temp = elevator.curr_floor;
				increament = -1;
			}
			elevator.priority[i] = temp;
		}
	}
	else if(elevator.direction == BACKWARD)
	{
		uint8_t temp = elevator.curr_floor;
		int increament = -1;
		for(int i=0; i<MAX_FLOOR; i++)
		{
			temp += increament;
			if(temp < MAX_FLOOR)
			{
				temp = elevator.curr_floor;
				increament = 1;
			}
			elevator.priority[i] = temp;
		}
	}
	else 	//stop
	{
		init_priority();
	}
}

void update_goal(void)
{
	for (int i=0; i<MAX_FLOOR; i++)
	{
		if (state & 1 << (elevator.priority[i] - 1) )
		{
			elevator.goal = elevator.priority[i];
			update_direction();
			return;
		}
	}
	elevator.goal = NO_GOAL;
	elevator.direction = STOP;
	init_priority();
}

void update_direction(void)
{
	if (elevator.goal == NO_GOAL)
		elevator.direction = STOP;
	else if (elevator.goal > elevator.curr_floor)
		elevator.direction = FORWARD;
	else if (elevator.goal < elevator.curr_floor)
		elevator.direction = BACKWARD;
}


void btn_event(void)
{
	if (btn_event_flag)
	{
		btn_event_flag = 0;
		uint8_t prev_direction = elevator.direction;
		update_priority();
		update_goal();
		if (elevator.goal == NO_GOAL && !pause_flag)
		{
			if (prev_direction == FORWARD) elevator.goal = elevator.curr_floor + 1;
			else if (prev_direction == BACKWARD) elevator.goal = elevator.curr_floor - 1;
			update_direction();
		}
	}
}

void floor_event(void)
{
	if (floor_event_flag)
	{
		floor_event_flag_dotmatrix = 1;
		floor_event_flag = 0;
		if (elevator.curr_floor == elevator.goal)
		{
			state &= ~(1 << (elevator.curr_floor - 1));
			update_priority();
			update_goal();
			pause();
		}
		else update_priority();
	}
}

void pause(void)
{
	pause_flag = 1;
	buzzer_play_flag = 1;
	elevator_pause_counter = 0;
	buzzer_counter = 0;
}

void resume(void)
{
	if (elevator_pause_counter >= 3000)
	{
		elevator_pause_counter = 0;
		pause_flag = 0;
	}
}

void stepmotor_driver(uint8_t direction)
{
	static int step = 0;
	if (direction == STOP) return;
	switch (step)
	{
		case 0:
			stepmotor_all_off();
			GPIOC->ODR |= STEPMOTOR_IN1_Pin;
			break;
		case 1:
			stepmotor_all_off();
			GPIOC->ODR |= STEPMOTOR_IN1_Pin | STEPMOTOR_IN2_Pin;
			break;
		case 2:
			stepmotor_all_off();
			GPIOC->ODR |= STEPMOTOR_IN2_Pin;
			break;
		case 3:
			stepmotor_all_off();
			GPIOC->ODR |= STEPMOTOR_IN2_Pin | STEPMOTOR_IN3_Pin;
			break;
		case 4:
			stepmotor_all_off();
			GPIOC->ODR |= STEPMOTOR_IN3_Pin;
			break;
		case 5:
			stepmotor_all_off();
			GPIOC->ODR |= STEPMOTOR_IN3_Pin | STEPMOTOR_IN4_Pin;
			break;
		case 6:
			stepmotor_all_off();
			GPIOC->ODR |= STEPMOTOR_IN4_Pin;
			break;
		case 7:
			stepmotor_all_off();
			GPIOC->ODR |= STEPMOTOR_IN4_Pin | STEPMOTOR_IN1_Pin;
			break;
	}
	if (direction == FORWARD) 		step = (step + 1) % 8;
	else if (direction == BACKWARD)	if(--step < 0) step = 7;
}

void stepmotor_all_off(void)
{
	GPIOC->ODR &= ~(STEPMOTOR_IN1_Pin | STEPMOTOR_IN2_Pin | STEPMOTOR_IN3_Pin | STEPMOTOR_IN4_Pin);
}

