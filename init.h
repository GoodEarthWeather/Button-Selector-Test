/*
 * init.h
 *
 *  Created on: Sep 8, 2026
 *      Author: david
 */

#ifndef INIT_H_
#define INIT_H_


void initClocks(void);
void initGPIO(void);
void init_spi_shift_register(void);

#define SHIFTER_LATCH  GPIO_PORT_P1, GPIO_PIN7
#define MENU_ENCODER_A GPIO_PORT_P2, GPIO_PIN1
#define MENU_ENCODER_B GPIO_PORT_P2, GPIO_PIN0
#define MENU_ENCODER_SWITCH GPIO_PORT_P4, GPIO_PIN6

#define OPTION_ENCODER_A GPIO_PORT_P3, GPIO_PIN2
#define OPTION_ENCODER_B GPIO_PORT_P3, GPIO_PIN6
#define OPTION_ENCODER_SWITCH GPIO_PORT_P4, GPIO_PIN2


#endif /* INIT_H_ */
