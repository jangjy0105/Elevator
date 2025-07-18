/*
 * i2c_lcd.h
 *
 *  Created on: 2019. 9. 4.
 *      Author: k
 */

#ifndef SRC_I2C_LCD_H_
#define SRC_I2C_LCD_H_

#include "main.h"
#include "stm32f4xx_hal.h"
#include <string.h>
#include <stdio.h>
#include "extern.h"


#define I2C_LCD_ADDRESS (0x27<<1)
#define BACKLIGHT_ON 0x08

/* LCD command   */
#define DISPLAY_ON 0x0C
#define DISPLAY_OFF 0x08
#define CLEAR_DISPLAY 0x01  //Delay 2msec
#define RETURN_HOME 0x02

void lcd_command(uint8_t command);
void lcd_data(uint8_t data);
void i2c_lcd_init(void);
void lcd_string(uint8_t *str);
void move_cursor(uint8_t row, uint8_t column);
void display_lcd(uint8_t goal);

extern uint8_t state;
extern volatile int lcd_blink_counter;
#endif /* SRC_I2C_LCD_H_ */
