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

void handleHW_wpm(const MenuItem_t *, int16_t);
void handleHW_band(const MenuItem_t *, int16_t);
void handleHW_rate(const MenuItem_t *, int16_t);

typedef struct
{
    uint8_t selectedSideband;
    uint8_t selectedFilter;
    uint8_t audioState;
    uint8_t wpm;  // current cw speed
    uint16_t freqMultiplier;
    uint8_t  bandIndex;
} RadioState_t;

extern RadioState_t radioState;

#endif /* RADIO_STATE_H_ */
