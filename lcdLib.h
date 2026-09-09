
#ifndef LCDLIB_H_
#define LCDLIB_H_

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

typedef struct
{
    uint8_t baseAddr;    /* DDRAM address, e.g. 0x00 for FREQ, 0x40 for STATUS */
    uint8_t fieldWidth;  /* characters, e.g. 12 for STATUS */
} LcdField_t;




#endif /* LCDLIB_H_ */
