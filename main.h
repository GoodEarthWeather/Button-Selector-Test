/*
 * main.h
 *
 *  Created on: May 16, 2018
 *      Author: dmcneill
 */


#ifndef MYINCLUDE_MAIN_H_
#define MYINCLUDE_MAIN_H_

#include "driverlib.h"


// Define all I/O
#define delay_ms(x)     __delay_cycles((long) x* 1000 * 8)
#define delay_us(x)     __delay_cycles((long) x * 8)

// Define shifter ports
#define SHIFTER_CLOCK   GPIO_PORT_P1, GPIO_PIN1
#define SHIFTER_DATA   GPIO_PORT_P1, GPIO_PIN2
#define SHIFTER_LATCH  GPIO_PORT_P1, GPIO_PIN7

// Define encoders
#define MENU_ENCODER_A GPIO_PORT_P2, GPIO_PIN1
#define MENU_ENCODER_B GPIO_PORT_P2, GPIO_PIN0
#define MENU_ENCODER_SWITCH GPIO_PORT_P4, GPIO_PIN6

#define OPTION_ENCODER_A GPIO_PORT_P3, GPIO_PIN2
#define OPTION_ENCODER_B GPIO_PORT_P3, GPIO_PIN6
#define OPTION_ENCODER_SWITCH GPIO_PORT_P4, GPIO_PIN2

// Define LCD interface
#define LCD_D4 GPIO_PORT_P5, GPIO_PIN2
#define LCD_D5 GPIO_PORT_P5, GPIO_PIN1
#define LCD_D6 GPIO_PORT_P5, GPIO_PIN0
#define LCD_D7 GPIO_PORT_P4, GPIO_PIN7
#define LCD_RS GPIO_PORT_P6, GPIO_PIN0
#define LCD_CLK GPIO_PORT_P3, GPIO_PIN3


// define bands - these numbers must match the order in the MenuItem_t definition
#define BAND_40M 0
#define BAND_30M 1
#define BAND_20M 2
#define BAND_17M 3
#define BAND_15M 4

// define rates - these numbers must match the order in the MenuItem_t definition
#define RATE_10 10
#define RATE_100 100
#define RATE_1K 1000
#define RATE_10K 10000

// define frequency bands
#define BAND_40M_LOWER 7000000
#define BAND_40M_UPPER 7300000
#define BAND_30M_LOWER 10100000
#define BAND_30M_UPPER 10150000
#define BAND_20M_LOWER 14000000
#define BAND_20M_UPPER 14350000
#define BAND_17M_LOWER 18068000
#define BAND_17M_UPPER 18168000
#define BAND_15M_LOWER 21000000
#define BAND_15M_UPPER 21450000
#define BAND_40M_SELECT GPIO_PORT_P6, GPIO_PIN1
#define BAND_30M_SELECT GPIO_PORT_P6, GPIO_PIN2
#define BAND_20M_SELECT GPIO_PORT_P4, GPIO_PIN7
#define BAND_17M_SELECT GPIO_PORT_P5, GPIO_PIN0
#define BAND_15M_SELECT GPIO_PORT_P5, GPIO_PIN1


#endif /* MYINCLUDE_MAIN_H_ */
