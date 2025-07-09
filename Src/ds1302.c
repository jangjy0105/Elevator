/*
 * p_ds1302->c
 *
 *  Created on: Mar 28, 2025
 *      Author: microsoft
 */

#include "ds1302.h"
#include <stdio.h>
#include "extern.h"

extern uint32_t flash_read_value;


void update_ds1302(void)
{
	if (ds1302.seconds == 59)
	{
		flash_erase();
		flash_write((uint32_t *)&ds1302, sizeof(ds1302));
	}
	read_time_ds1302();
	read_date_ds1302();
	pc_command_processing();
}


void init_gpio_ds1302(void)
{
	HAL_GPIO_WritePin(GPIOA, CE_DS1302_Pin, 0);
	HAL_GPIO_WritePin(GPIOA, IO_DS1302_Pin, 0);
	HAL_GPIO_WritePin(GPIOA, CLK_DS1302_Pin, 0);
}

void init_date_time()
{
	ds1302.year = 25;
	ds1302.month = 4;
	ds1302.date = 11;
	ds1302.dayofweek = 6;
	ds1302.hours = 11;
	ds1302.minutes = 20;
	ds1302.seconds = 00;
}

void init_ds1302_flash(void)
{
	flash_read_value = *(__IO uint32_t *) USER_DATA_ADDRESS;	// sector 7의 시작주소

		if (flash_read_value == FLASH_INIT_STATUS)  // 초기에 아무런 데이터도 존재 하지 않을 경우 0xff
		{
			flash_erase();
			printf("flash EMPTY !!!!\n");
			ds1302.magic=0x55555555;	// FLASH MEMORY가 초기상태가 아닌 data가 write 되었다는 뜻
			init_date_time();
			flash_write((uint32_t *)&ds1302, sizeof(ds1302));
		}
		else   // 1번 이상 flash memory에 데이터를 write 한 경우
		{
			flash_read((uint32_t *)&ds1302, sizeof(ds1302));
		}

		init_ds1302();
}

void init_ds1302(void)
{
	write_ds1302(ADDR_SECONDS, ds1302.seconds);
	write_ds1302(ADDR_MINUTES, ds1302.minutes);
	write_ds1302(ADDR_HOURS, ds1302.hours);
	write_ds1302(ADDR_DATE, ds1302.date);
	write_ds1302(ADDR_MONTH, ds1302.month);
	write_ds1302(ADDR_DAYOFWEEK, ds1302.dayofweek);
	write_ds1302(ADDR_YEAR, ds1302.year);
}

void write_ds1302(uint8_t addr, uint8_t data)
{
	// 1. CE low --> high
	HAL_GPIO_WritePin(GPIOA, CE_DS1302_Pin, 1);

	// 2. addr 전송
	tx_ds1302(addr);

	// 3. data 전송
	tx_ds1302(dec2bcd(data));

	// 4. CE high --? low
	HAL_GPIO_WritePin(GPIOA, CE_DS1302_Pin, 0);
}

void tx_ds1302(uint8_t value)
{
	output_dataline_ds1302();

	for (int i=0; i<8; i++)
	{
		uint8_t tmp = (value & (1 << i)) ? 1 : 0;
		HAL_GPIO_WritePin(GPIOA, IO_DS1302_Pin, tmp);
		clock_ds1302();
	}
}

void read_time_ds1302(void)
{
	ds1302.seconds = read_ds1302(ADDR_SECONDS);
	ds1302.minutes = read_ds1302(ADDR_MINUTES);
	ds1302.hours = read_ds1302(ADDR_HOURS);
}

void read_date_ds1302(void)
{
	ds1302.date = read_ds1302(ADDR_DATE);
	ds1302.month = read_ds1302(ADDR_MONTH);
	ds1302.dayofweek = read_ds1302(ADDR_DAYOFWEEK);
	ds1302.year = read_ds1302(ADDR_YEAR);
}

uint8_t read_ds1302(uint8_t addr)
{
	uint8_t data8bits = 0;

	// 1. CE high
	HAL_GPIO_WritePin(GPIOA, CE_DS1302_Pin, 1);
	// 2. addr 전송
	tx_ds1302(addr+1);
	// 3. data 읽기
	rx_ds1302(&data8bits);
	// 4. CE low
	HAL_GPIO_WritePin(GPIOA, CE_DS1302_Pin, 0);
	// 5. return (bcd to dec)
	return (bcd2dec(data8bits));		// bcd to dec를 이런식으로도 사용
}

void rx_ds1302(uint8_t* p_data)
{
	uint8_t temp = 0;
	input_dataline_ds1302();
	for (int i=0; i<8; i++)
	{
		if ( HAL_GPIO_ReadPin(GPIOA, IO_DS1302_Pin) )
			temp |= 1 << i;
		else
			temp &= ~(1 << i);

		if (i != 7 ) clock_ds1302();
	}
	*p_data = temp;
}

void output_dataline_ds1302(void)
{
	GPIO_InitTypeDef GPIO_init = {0,};
	GPIO_init.Pin = IO_DS1302_Pin;
	GPIO_init.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_init.Pull = GPIO_NOPULL;
	GPIO_init.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(GPIOA, &GPIO_init);
}

void input_dataline_ds1302(void)
{
	GPIO_InitTypeDef GPIO_init = {0,};
	GPIO_init.Pin = IO_DS1302_Pin;
	GPIO_init.Mode = GPIO_MODE_INPUT;
	GPIO_init.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOA, &GPIO_init);
}

void clock_ds1302(void)
{
	HAL_GPIO_WritePin(GPIOA, CLK_DS1302_Pin, 1);
	HAL_GPIO_WritePin(GPIOA, CLK_DS1302_Pin, 0);
}

uint8_t dec2bcd(uint8_t dec)
{
	uint8_t high, low;
	low = dec % 10;
	high = (dec / 10) << 4;

	return (high | low);
}

uint8_t bcd2dec(uint8_t bcd)
{
	uint8_t high, low;
	low = bcd & 0x0f;
	high = 10*((bcd & 0xf0) >> 4);

	return (high + low);
}

void set_rtc(char* date_time)
{
	char* tmp;
	tmp = (char *)malloc(sizeof(char)*4);
	strncpy(tmp, date_time, 2);
	ds1302.year = atoi(tmp);
	date_time+=2;

	strncpy(tmp, date_time, 2);
	ds1302.month = atoi(tmp);
	date_time+=2;

	strncpy(tmp, date_time, 2);
	ds1302.date = atoi(tmp);
	date_time+=2;

	strncpy(tmp, date_time, 2);
	ds1302.hours = atoi(tmp);
	date_time+=2;

	strncpy(tmp, date_time, 2);
	ds1302.minutes = atoi(tmp);
	date_time+=2;

	strncpy(tmp, date_time, 2);
	ds1302.seconds = atoi(tmp);

	free(tmp);

	init_ds1302();
	flash_erase();
	flash_write((uint32_t *)&ds1302, sizeof(ds1302));
}

