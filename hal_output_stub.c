/*
 * hal_output_stub.c
 *
 * "No hardware attached" implementation of hal_output.h.
 * Prints what WOULD have been sent to the shift registers / LCD
 * to the CCS debug console (CIO) instead.
 *
 * BUILD CONFIG NOTE:
 * Put this file in a separate CCS Build Configuration (e.g. "Debug_NoHW")
 * from hal_output_real.c (e.g. "Debug_HW"). Right-click each file ->
 * "Exclude from Build..." for whichever configuration doesn't need it.
 * That way switching between "test on the bench" and "test with the
 * real button board" is a one-click build-configuration change --
 * menu_data.c and main.c never need to be touched or #ifdef'd.
 *
 * Requires: Project Properties -> Debug -> "Enable CIO function use"
 * And on FRAM parts: make sure .cio / .sysmem are linked into a RAM
 * region in the linker .cmd file, not FRAM, or console output can
 * behave oddly.
 */

#include <stdio.h>
#include "hal_output.h"

void HAL_Output_Init(void)
{
    printf("HAL (STUB): output initialized -- no hardware attached.\n");
}

void HAL_LED_SetPattern(uint32_t pattern)
{
    /* Print as 24 individual bit positions, MSB..LSB, so it visually
     * matches "which LED would be lit" -- and also print which bit(s)
     * are set by name-friendly index, since that's what you'll want
     * to eyeball against menuTable[].ledIndex. */
    int8_t i;

    printf("LED PATTERN: ");
    for (i = 23; i >= 0; i--)
    {
        printf("%c", (pattern & ((uint32_t)1 << i)) ? '1' : '0');
        if ((i % 8) == 0 && i != 0)
        {
            printf(" ");  /* group into bytes, matches the 3 shift registers */
        }
    }
    printf("  (lit bit(s): ");
    for (i = 0; i < 24; i++)
    {
        if (pattern & ((uint32_t)1 << i))
        {
            printf("%d ", i);
        }
    }
    printf(")\n");
}

void HAL_LCD_WriteLine(uint8_t line, const char *text)
{
    printf("LCD[%u]: \"%s\"\n", line, text);
}
