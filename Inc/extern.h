/*
 * extern.h
 *
 *  Created on: Apr 9, 2025
 *      Author: microsoft
 */

#ifndef INC_EXTERN_H_
#define INC_EXTERN_H_

#include "main.h"
#include "ds1302.h"
#include "button.h"

extern volatile int TIM11_1ms_counter;

extern int get_button(GPIO_TypeDef *GPIO, int GPIO_Pin, int button_num);

//extern void init_gpio_ds1302(void);
//extern void init_flash(void);
//extern void ds1302_main(void);

extern uint8_t rx_data;
extern UART_HandleTypeDef huart2;
extern TIM_HandleTypeDef htim2;
extern void pc_command_processing(void);
extern void set_rtc(char* date_time);
extern void delay_us(int us);

extern HAL_StatusTypeDef flash_write (uint32_t *data32, int size);
extern HAL_StatusTypeDef flash_read(uint32_t *addr32, int size);
extern HAL_StatusTypeDef flash_erase(void);

extern void i2c_lcd_init(void);
extern void i2c_lcd_main(void);
extern void display_lcd(uint8_t goal);


extern void init_elevator(void);
extern void elevator_main(void);
extern void stepmotor_driver(uint8_t direction);

extern void display_dotmatrix_floor(uint8_t floor, uint8_t direction);
extern void buzzer_play(void);

#endif /* INC_EXTERN_H_ */
