/*
 * hal_output_real.c
 *
 * "Real hardware attached" implementation of hal_output.h.
 * Fill in once the button/LED/LCD board comes back from fab.
 * Same build-configuration note as hal_output_stub.c applies --
 * exclude ONE of these two files per configuration, never both.
 */

#include "hal_output.h"
/* #include "driverlib.h" */
/* #include "lcd_driver.h" -- your existing/likely-off-the-shelf HD44780-style driver */

void HAL_Output_Init(void)
{
    /* GPIO_setAsOutputPin() for the 3 shift-register control lines
     * (DATA/CLOCK/LATCH), LCD_init(), etc. */
}

void HAL_LED_SetPattern(uint32_t pattern)
{
    /* Shift 24 bits out MSB-first (or LSB-first -- match your wiring)
     * across the 3x 74HCT595s, then pulse latch.
     *
     * uint8_t i;
     * for (i = 0; i < 24; i++)
     * {
     *     GPIO_setOutputLowOnPin/HighOnPin(DATA_PIN,
     *         (pattern & (1UL << (23 - i))) ? 1 : 0);
     *     pulse CLOCK_PIN;
     * }
     * pulse LATCH_PIN;
     */
}

void HAL_LCD_WriteLine(uint8_t line, const char *text)
{
    /* LCD_setCursor(0, line); LCD_print(text); pad remainder with
     * spaces to LCD_LINE_LEN so leftover characters from a longer
     * previous string don't linger on screen. */
}
