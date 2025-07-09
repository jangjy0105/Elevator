/*
 * uart.c
 *
 *  Created on: Mar 31, 2025
 *      Author: microsoft
 */
#include "uart.h"

/**
  * @brief  Rx Transfer completed callbacks.
  * @param  huart  Pointer to a UART_HandleTypeDef structure that contains
  *                the configuration information for the specified UART module.
  * @retval None
  * move from Drivers/STM32F4xx_HAL_driver/Src/stm324xx_hal_uart.c to here
  * ex) comportmaster로부터 1char를 수신하면 HAL_UART_RxCpltCallback으로 진입
  * 9600bps의 경우 HAL_UART_RxCpltCallback를 1ms 이내에는 빠져나가야 함.
  * 115200bps의 경우						   86us
  */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	static volatile int i = 0;
	if (huart == &huart2)
	{
		if (rx_data == '\n')
		{
			rx_buff[rear++][i] = '\0';			// 문장의 끝을 NULL로 함
			rear %= COMMAND_NUMBER;		// rear -> 0 ~ 9
			i = 0;
			// !!!! rx_buff queue full check 하는 logic 추가
		}
		else
		{
			rx_buff[rear][i++] = rx_data;
			// COMMAND_LENGTH를 check하는 logic 추가
		}
		HAL_UART_Receive_IT(&huart2, &rx_data, 1);	// 반드시 집어넣어야 다음 INT가 발생
	}
}

void pc_command_processing()
{
	if(front != rear)	// rx_buff에 data가 존재
	{
		if( strncmp((char *)rx_buff[front], (const char *)"setrtc", strlen("setrtc")) == 0 )
		{
			set_rtc(rx_buff[front]+strlen("setrtc"));
		}
		front++;
		front %= COMMAND_NUMBER;
	}
}
