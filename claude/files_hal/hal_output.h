/*
 * hal_output.h
 *
 * Hardware abstraction for the two physical outputs the menu system
 * drives: the 24-LED "which item is selected" indicator, and the
 * 16x2 LCD text.
 *
 * menu_data.c / main.c should ONLY call these two functions -- never
 * touch the 74HCT595 shift registers or the LCD driver directly.
 * That way the exact same menu logic runs unmodified whether it's
 * linked against the STUB implementation (prints to CCS console) or
 * the REAL implementation (drives actual GPIO/SPI).
 */

#ifndef HAL_OUTPUT_H_
#define HAL_OUTPUT_H_

#include <stdint.h>

/* Set the 24-LED pattern. Bit N (0..23) = 1 means LED N is lit.
 * In normal operation exactly one bit is set at a time, but the HAL
 * doesn't need to assume that -- it just shifts out whatever pattern
 * it's given. */
void HAL_LED_SetPattern(uint32_t pattern);

/* Write a null-terminated string to LCD line (0 or 1). Caller is
 * responsible for keeping it <= LCD_LINE_LEN characters; the HAL may
 * truncate/pad as needed for the physical display. */
void HAL_LCD_WriteLine(uint8_t line, const char *text);

/* Call once at startup before using the above. */
void HAL_Output_Init(void);

#endif /* HAL_OUTPUT_H_ */
