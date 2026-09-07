/*
 * radio_state.h
 *
 * Live operating parameters for the radio, derived from menu settings.
 * This is the decoupling point between the menu system and everything
 * else: the keyer, TX chain, etc. read from here and don't need to
 * know anything about menus, encoders, or LEDs. Menu action callbacks
 * are the ONLY code that writes to this struct.
 */

#ifndef RADIO_STATE_H_
#define RADIO_STATE_H_

#include <stdint.h>

typedef struct
{
    uint16_t keyerDotTimeMs;   /* CW dot duration, derived from WPM     */
    uint8_t  bandIndex;        /* mirrors menuCurrentValue[BAND], kept
                                 * here too so non-menu code (e.g. a
                                 * band-edge frequency lookup) doesn't
                                 * need to touch menu_data.h at all    */
    uint8_t  txPowerWatts;
    /* add more fields as menu items are fleshed out */
} RadioState_t;

extern RadioState_t radioState;

#endif /* RADIO_STATE_H_ */
