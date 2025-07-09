///*
// * failed_i2c_lcd.c
// *
// *  Created on: Apr 11, 2025
// *      Author: microsoft
// */
//
//
///*
// * i2c_lcd.c
// *
// *  Created on: 2019. 9. 4.
// *      Author: k
// */
//
//#include "i2c_lcd.h"
//#include "main.h"
//
////extern I2C_HandleTypeDef hi2c1;
//
//void i2c_lcd_main(void);
//void i2c_lcd_init(void);
//void lcd_string(uint8_t *str);
//uint8_t transmit_8bit(uint8_t data);
//uint8_t transmit_lcd(uint8_t data);
//void sda_mode_output(void);
//void sda_mode_input(void);
//
//void display_lcd(uint8_t goal)
//{
//	uint8_t static goal_floor_blink_flag = 0;
//	char* line1;
////	char line1[17] = "                ";
//	char* line2;
//	line1 = (char *)malloc(sizeof(char)*17);
//	line2 = (char *)malloc(sizeof(char)*17);
//	for (int i = 0; i < 16; i++)
//		line1[i] = ' ';
//	line1[16] = '\0';
//
//	for (int i=0; i<4; i++)
//	{
//		if ( state & 1<<i )
//		{
//			line1[i*4+1] = (i+1) + '0';
//			line1[i*4+2] = 'F';
//		}
//	}
//	goal_floor_blink_flag = !goal_floor_blink_flag;
//	if (goal != 10)
//	{
//		if (goal_floor_blink_flag)
//		{
//			line1[(goal-1)*4+1] = goal + '0';
//			line1[(goal-1)*4+2] = 'F';
//		}
//		else
//		{
//			line1[(goal-1)*4+1] = ' ';
//			line1[(goal-1)*4+2] = ' ';
//		}
//	}
//
//	sprintf(line2, "TIME: %02d:%02d:%02d", ds1302.hours , ds1302.minutes, ds1302.seconds);
//	move_cursor(0,0);
//	lcd_string(line1);
//	move_cursor(1,0);
//	lcd_string(line2);
//	free(line1);
//	free(line2);
//}
//
//void lcd_data(uint8_t data){
//
//	uint8_t high_nibble, low_nibble;
//	uint8_t i2c_buffer[4];
//	high_nibble = data & 0xf0;
//	low_nibble = (data<<4) & 0xf0;
//	i2c_buffer[0] = high_nibble | 0x05 | 0x08; //en=1, rs=1, rw=0, backlight=1
//	i2c_buffer[1] = high_nibble | 0x01 | 0x08; //en=0, rs=1, rw=0, backlight=1
//	i2c_buffer[2] = low_nibble  | 0x05 | 0x08; //en=1, rs=1, rw=0, backlight=1
//	i2c_buffer[3] = low_nibble  | 0x01 | 0x08; //en=0, rs=1, rw=0, backlight=1
//	while(HAL_I2C_Master_Transmit(&hi2c1, I2C_LCD_ADDRESS,
//			i2c_buffer, 4, 100)!=HAL_OK){
//		//HAL_Delay(1);
//	}
//	return;
//}
//
//void lcd_command(uint8_t command){
//
//	uint8_t high_nibble, low_nibble;
//	uint8_t i2c_buffer[4];
//	high_nibble = command & 0xf0;
//	low_nibble = (command<<4) & 0xf0;
//	i2c_buffer[0] = high_nibble | 0x04 | 0x08; //en=1, rs=0, rw=0, backlight=1
//	i2c_buffer[1] = high_nibble | 0x00 | 0x08; //en=0, rs=0, rw=0, backlight=1
//	i2c_buffer[2] = low_nibble  | 0x04 | 0x08; //en=1, rs=0, rw=0, backlight=1
//	i2c_buffer[3] = low_nibble  | 0x00 | 0x08; //en=0, rs=0, rw=0, backlight=1
//	while(HAL_I2C_Master_Transmit(&hi2c1, I2C_LCD_ADDRESS,
//			i2c_buffer, 4, 100)!=HAL_OK){
//		//HAL_Delay(1);
//	}
//	return;
//}
////void lcd_command(uint8_t command){
////
////	uint8_t high_nibble, low_nibble;
////	uint8_t i2c_buffer[4];
////	high_nibble = command & 0xf0;
////	low_nibble = (command<<4) & 0xf0;
////	i2c_buffer[0] = high_nibble | 0x04 | 0x08; //en=1, rs=0, rw=0, backlight=1
////	i2c_buffer[1] = high_nibble | 0x00 | 0x08; //en=0, rs=0, rw=0, backlight=1
////	i2c_buffer[2] = low_nibble  | 0x04 | 0x08; //en=1, rs=0, rw=0, backlight=1
////	i2c_buffer[3] = low_nibble  | 0x00 | 0x08; //en=0, rs=0, rw=0, backlight=1
////	for (int i=0; i<4; i++)
////	{
////		transmit_lcd(i2c_buffer[i]);
////		sda_mode_output();
////		GPIOB->ODR |= LCD_SDA_Pin;
////		GPIOB->ODR |= LCD_SCL_Pin;
////	}
////	return;
////}
////void lcd_data(uint8_t data){
////
////	uint8_t high_nibble, low_nibble;
////	uint8_t i2c_buffer[4];
////	high_nibble = data & 0xf0;
////	low_nibble = (data<<4) & 0xf0;
////	i2c_buffer[0] = high_nibble | 0x05 | 0x08; //en=1, rs=1, rw=0, backlight=1
////	i2c_buffer[1] = high_nibble | 0x01 | 0x08; //en=0, rs=1, rw=0, backlight=1
////	i2c_buffer[2] = low_nibble  | 0x05 | 0x08; //en=1, rs=1, rw=0, backlight=1
////	i2c_buffer[3] = low_nibble  | 0x01 | 0x08; //en=0, rs=1, rw=0, backlight=1
////	for (int i=0; i<4; i++)
////	{
////		transmit_lcd(i2c_buffer[i]);
////		sda_mode_output();
////		GPIOB->ODR |= LCD_SDA_Pin;
////		GPIOB->ODR |= LCD_SCL_Pin;
////	}
////	return;
////}
//
//void i2c_lcd_init(void){
//	lcd_command(0x33);
//	lcd_command(0x32);
//	lcd_command(0x28);	//Function Set 4-bit mode
//	lcd_command(DISPLAY_ON);
//	lcd_command(0x06);	//Entry mode set
//	lcd_command(CLEAR_DISPLAY);
//	HAL_Delay(2);
//}
//
//void lcd_string(uint8_t *str){
//	while(*str)lcd_data(*str++);
//}
//void move_cursor(uint8_t row, uint8_t column){
//	lcd_command(0x80 | row<<6 | column);
//	return;
//}
//
////void sda_mode_output(void)
////{
////	GPIOB->MODER |= 1 << 2*9;
////	GPIOB->MODER &= ~(1 << (2*9 + 1));
//////	GPIOB->OTYPER |= 1<<9;
//////	GPIOB->PUPDR &= ~(0b11 << (2 * 9));
//////	GPIOB->PUPDR |=  (0b01 << (2 * 9));  // Pull-up
////}
////
////void sda_mode_input(void)
////{
////	GPIOB->MODER &= ~(0b11 << 2*9);
//////	GPIOB->PUPDR &= ~(0b11 << (2 * 9));             // PUPDR 클리어
//////	GPIOB->PUPDR |=  (0b01 << (2 * 9));             // 풀업 활성화
////}
////
////uint8_t transmit_lcd(uint8_t data)
////{
////	sda_mode_output();
////	GPIOB->ODR &= ~LCD_SDA_Pin;
////	delay_us(5);
//
//
////	GPIOB->ODR &= ~LCD_SCL_Pin;
////	delay_us(5);
////	if(!transmit_8bit(0x27<<1)) return 0;
////	GPIOB->ODR &= ~LCD_SCL_Pin;
////	if(!transmit_8bit(data)) return 0;
////	GPIOB->ODR |= LCD_SDA_Pin;
////	return 1;
////}
////
////uint8_t transmit_8bit(uint8_t data)
////{
////	for (int i=7; i>=0; i--)
////	{
////		if(data & (1 << i))
////			GPIOB->ODR |= LCD_SDA_Pin;
////		else
////			GPIOB->ODR &= ~LCD_SDA_Pin;
////		delay_us(5);
////		clk_lcd();
////		delay_us(5);
////	}
////	sda_mode_input();
////	delay_us(2);
////	GPIOB->ODR |= LCD_SCL_Pin;
////	delay_us(5);
////	uint8_t ack = (GPIOB->IDR & LCD_SDA_Pin) ? 1 : 0;
//////	return (GPIOB->IDR & LCD_SDA_Pin) ? 1 : 0;
//////	GPIOB->ODR &= ~LCD_SCL_Pin;
////
////	sda_mode_output();
////	return ack;
////}
////
////void clk_lcd(void)
////{
////	GPIOB->ODR |= LCD_SCL_Pin;
////	delay_us(5);
////	GPIOB->ODR &= ~LCD_SCL_Pin;
////	delay_us(5);
////}
////
////
////
////
////
