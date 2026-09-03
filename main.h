/*
 * main.h
 *
 *  Created on: May 16, 2018
 *      Author: dmcneill
 */


#ifndef MYINCLUDE_MAIN_H_
#define MYINCLUDE_MAIN_H_

#include "driverlib.h"

void initClocks(void);
void init_spi_shift_register(void);
void initGPIO(void);
void send_byte_to_shift_register(uint8_t);

// Define all I/O
#define delay_ms(x)     __delay_cycles((long) x* 1000 * 8)
#define delay_us(x)     __delay_cycles((long) x * 8)

// Define the GPIO Pin used for the 74HCT595 Latch Signal (RCLK)
#define LATCH_PORT   GPIO_PORT_P1
#define LATCH_PIN    GPIO_PIN7

// Define encoders
#define MENU_ENCODER_A GPIO_PORT_P2, GPIO_PIN1
#define MENU_ENCODER_B GPIO_PORT_P2, GPIO_PIN0
#define MENU_ENCODER_SWITCH GPIO_PORT_P4, GPIO_PIN6

#define OPTION_ENCODER_A GPIO_PORT_P3, GPIO_PIN2
#define OPTION_ENCODER_B GPIO_PORT_P3, GPIO_PIN6
#define OPTION_ENCODER_SWITCH GPIO_PORT_P4, GPIO_PIN2

#define LCD_D4 GPIO_PORT_P5, GPIO_PIN2
#define LCD_D5 GPIO_PORT_P5, GPIO_PIN1
#define LCD_D6 GPIO_PORT_P5, GPIO_PIN0
#define LCD_D7 GPIO_PORT_P4, GPIO_PIN7
#define LCD_RS GPIO_PORT_P6, GPIO_PIN0
#define LCD_CLK GPIO_PORT_P3, GPIO_PIN3


#endif /* MYINCLUDE_MAIN_H_ */
