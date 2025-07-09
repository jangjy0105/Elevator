

#include "dotmatrix.h"

void display_dotmatrix_floor(uint8_t floor, uint8_t direction)
{
	static int i = 0;
	static uint8_t display_data[8];

	uint8_t col[2];

	if (floor_event_flag_dotmatrix) init_dotmatrix_floor(display_data, floor);
	if ((dotmatrix_scroll_counter >= 200) && (direction != STOP) && ! pause_flag)
	{
		dotmatrix_scroll_counter = 0;
		scroll(display_data, direction);
	}
	col[0] = ~(1 << i);
	col[1] = display_data[i];
	for (int j=0; j<2; j++)
	{
		for (int k=0; k<8; k++)
		{
			if (col[j] & (1 << k)) 	HAL_GPIO_WritePin(GPIOB, SER_74HC595_Pin, 1);
			else					HAL_GPIO_WritePin(GPIOB, SER_74HC595_Pin, 0);

			HAL_GPIO_WritePin(GPIOB, CLK_74HC595_Pin, 1);
			HAL_GPIO_WritePin(GPIOB, CLK_74HC595_Pin, 0);
		}
	}
	GPIOB->ODR &= ~GPIO_PIN_13;   // latch핀을 pull-down
	GPIOB->ODR |= GPIO_PIN_13;   // latch핀을 pull-up
	i = (i + 1) % 8;
}

void scroll(uint8_t *display_data, uint8_t direction)
{
	uint8_t temp;
	if(direction == FORWARD)
	{
		temp = display_data[0];
		for (int i=0; i<7; i++)
			display_data[i] = display_data[i+1];
		display_data[7] = temp;
	}
	else if (direction == BACKWARD)
	{
		temp = display_data[7];
		for (int i=7; i>0; i--)
			display_data[i] = display_data[i-1];
		display_data[0] = temp;
	}
}


void init_dotmatrix_floor(uint8_t* display_data, uint8_t floor)
{
	floor_event_flag_dotmatrix = 0;
	for (int i=0; i < 8; i++)
	{
		display_data[i] = num_data[floor-1][i];
	}
}


