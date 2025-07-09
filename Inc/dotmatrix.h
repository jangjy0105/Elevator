/*
 * dotmatrix.h
 *
 *  Created on: Apr 3, 2025
 *      Author: microsoft
 */

#ifndef INC_DOTMATRIX_H_
#define INC_DOTMATRIX_H_

#include "main.h"

const uint8_t num_data[4][8] = {
	{0x00,0x08,0x18,0x08,0x08,0x08,0x1c,0x00},
	{0x00,0x18,0x24,0x04,0x08,0x10,0x3c,0x00},
	{0x00,0x18,0x24,0x04,0x18,0x04,0x24,0x18},
	{0x00,0x08,0x18,0x28,0x3c,0x08,0x08,0x00}
};


extern volatile int dotmatrix_scroll_counter;
extern uint8_t pause_flag;

#define STOP		0
#define FORWARD 	1
#define BACKWARD 	2

uint8_t floor_event_flag_dotmatrix = 1;

unsigned char display_data[8];  // 최종 8x8 출력할 데이터

void display_dotmatrix_floor(uint8_t floor, uint8_t direction);
void scroll(uint8_t *display_data, uint8_t direction);
void init_dotmatrix_floor(uint8_t* display_data, uint8_t floor);


#endif /* INC_DOTMATRIX_H_ */
