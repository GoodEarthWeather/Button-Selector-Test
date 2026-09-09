/*
 * isr.h
 *
 *  Created on: Sep 8, 2026
 *      Author: david
 */

#ifndef ISR_H_
#define ISR_H_

extern uint8_t volatile buttonPressed;

// Define buttons
#define BTN_PRESSED_NONE 0x0
#define BTN_PRESSED_OPTION_ENCODER 0x1
#define BTN_PRESSED_MENU_ENCODER 0x2
#define BTN_PRESSED_OPTION_ENCODER_SWITCH 0x3
#define BTN_PRESSED_MENU_ENCODER_SWITCH 0x4

#define ENCODER_CW   1
#define ENCODER_CCW  -1

#endif /* ISR_H_ */
