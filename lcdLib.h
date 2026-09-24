
#ifndef LCDLIB_H_
#define LCDLIB_H_

#include "driverlib.h"
#include <string.h>

// Functions
void lcdInit();                                 // Initialize LCD
void moveFreqCursor(void);
void updateLCD_menu(void);

// Delay Functions
// Modified for an 8MHz clock
#define delay_ms(x)		__delay_cycles((long) x* 1000 * 8)
#define delay_us(x)		__delay_cycles((long) x * 8)

// Pins

#define MOVE_CURSOR 0x80   // add address to this for move cursor command
#define LCD_D4 GPIO_PORT_P5, GPIO_PIN2
#define LCD_D5 GPIO_PORT_P5, GPIO_PIN1
#define LCD_D6 GPIO_PORT_P5, GPIO_PIN0
#define LCD_D7 GPIO_PORT_P4, GPIO_PIN7
#define LCD_RS GPIO_PORT_P6, GPIO_PIN0
#define LCD_CLK GPIO_PORT_P3, GPIO_PIN3


typedef struct
{
    uint8_t baseAddr;    /* DDRAM address, e.g. 0x00 for FREQ, 0x40 for STATUS */
    uint8_t fieldWidth;  /* characters, e.g. 12 for STATUS */
} LcdField_t;




#endif /* LCDLIB_H_ */
