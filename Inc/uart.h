/*
 * uart.h
 *
 *  Created on: Mar 31, 2025
 *      Author: microsoft
 */

#ifndef INC_UART_H_
#define INC_UART_H_

#include "main.h"
#include "ds1302.h"
#include "extern.h"
#include <string.h>
#include <stdio.h>
#define COMMAND_NUMBER 	20
#define COMMAND_LENGTH 	40
volatile uint8_t rx_buff[COMMAND_NUMBER][COMMAND_LENGTH];		// URAT0로부터 들어온 문자를 저장하는 버퍼(변수)
volatile int rear = 0;		// input index: UART0_RX_vect에서 넣어주는 index
volatile int front = 0;		// output index
extern uint8_t rx_data;

#endif /* INC_UART_H_ */
