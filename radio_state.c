/*
 * radio_state.c
 */

#include "radio_state.h"

RadioState_t radioState = { 0 };

/* ------------------------------------------------------------------ */
/* Hardware / state action callbacks -- ONE per menu item, no          */
/* exceptions. Even a "just set a variable" item gets a function here  */
/* so that Menu_OptionMove()/Menu_Init() never need to know what kind  */
/* of thing each item controls -- that knowledge lives ONLY in these   */
/* functions and nowhere else. Signature must match MenuActionFn in    */
/* menu_data.h.                                                        */
/* ------------------------------------------------------------------ */

static void handleHW_wpm(const MenuItem_t *item, int16_t value)
{
    (void)item;
    /* Standard PARIS timing: dot length (ms) = 1200 / WPM */
    radioState.keyerDotTimeMs = (uint16_t)(1200 / value);
}

static void handleHW_band(const MenuItem_t *item, int16_t value)
{
    (void)item; /* unused here, but available if the callback needs
                 * item->def.list.options[value] etc. */
    radioState.bandIndex = (uint8_t)value;
    HAL_HW_SelectBand((uint8_t)value);
}
