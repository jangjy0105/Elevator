/*
 * ds1302.h
 *
 *  Created on: Mar 28, 2025
 *      Author: microsoft
 */

#ifndef INC_DS1302_H_
#define INC_DS1302_H_

#include <stdio.h>
#include "main.h"
#include "extern.h"
#include "string.h"
#include "stdlib.h"

#define ADDR_SECONDS	0x80		// wirte addr만 define 하기로 하자, read addr은 write addr에 +1
#define ADDR_MINUTES	0x82
#define ADDR_HOURS		0x84
#define ADDR_DATE		0x86
#define ADDR_MONTH		0x88
#define ADDR_DAYOFWEEK	0x8a
#define ADDR_YEAR		0x8c
#define ADDR_WRITEPROTECTED	0x8e

#define ADDR_FLASH_SECTOR7      ((uint32_t) 0x8060000)
#define FLASH_USER_START_ADDR   ((uint32_t) 0x8060000)
#define USER_DATA_ADDRESS        0x8060000
#define FLASH_USER_END_ADDR     ((uint32_t) 0x807FFFF)
#define FLASH_INIT_STATUS       0xFFFFFFFF        // flash 초기 상태
#define FLASH_NOT_INIT_STATUS   0xAAAAAAAA        // flash 초기 상태가 아니다
#define DATA_32                 ((uint32_t) 0x00000001)

typedef struct ds1302
{
	uint32_t magic;		// 4byte
	uint8_t seconds;
	uint8_t minutes;
	uint8_t hours;
	uint8_t date;		// 8byte
	uint8_t month;
	uint8_t dayofweek;	// 1:sun 2:mon
	uint8_t year;
	uint8_t ampm;	// 1:pm, 0:am		// 12byte
	uint8_t hourmode;	// 0:24 1:12	// 13byte
	uint8_t	dummy[3];					// 16byte	, 4byte 단위로 맞춰줘야함

} t_ds1302;


void update_ds1302(void);
void init_gpio_ds1302(void);
void init_date_time(void);
void init_ds1302_flash(void);
void init_ds1302(void);
void write_ds1302(uint8_t addr, uint8_t data);
void tx_ds1302(uint8_t value);
void read_time_ds1302(void);
void read_date_ds1302(void);
uint8_t read_ds1302(uint8_t addr);
void rx_ds1302(uint8_t* p_data);
void output_dataline_ds1302(void);
void input_dataline_ds1302(void);
void clock_ds1302(void);
uint8_t dec2bcd(uint8_t dec);
uint8_t bcd2dec(uint8_t bcd);

t_ds1302 ds1302;

#endif /* INC_DS1302_H_ */
