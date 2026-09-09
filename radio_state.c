/*
 * radio_state.c
 */

#include "radio_state.h"
#include "lcdLib.h"
#include "main.h"
#include "menu_data.h"

static void selectFilter(uint8_t);
static void selectSideband(uint8_t);
static void selectAudioState(uint8_t);

RadioState_t radioState = { 0 };

/* ------------------------------------------------------------------ */
/* Hardware / state action callbacks -- ONE per menu item, no          */
/* exceptions. Even a "just set a variable" item gets a function here  */
/* so that Menu_OptionMove()/Menu_Init() never need to know what kind  */
/* of thing each item controls -- that knowledge lives ONLY in these   */
/* functions and nowhere else. Signature must match MenuActionFn in    */
/* menu_data.h.                                                        */
/* ------------------------------------------------------------------ */

void handleHW_wpm(const MenuItem_t *item, int16_t value)
{
    (void)item;
    // Since the menu item for CW speed is a range value, it has
    // already been incremented or decremented by the option_move
    // function.
    radioState.wpm = (uint8_t)value;
    //initKeyTimer((uint8_t)value);  // update timer with new speed
}

void handleHW_band(const MenuItem_t *item, int16_t value)
{
    //(void)item; /* unused here, but available if the callback needs
    //             * item->def.list.options[value] etc. */
    radioState.bandIndex = (uint8_t)value;
    selectAudioState(MUTE);
    /*
    switch (value)
    {
    case BAND_40M :
        GPIO_setOutputHighOnPin(BAND_40M_SELECT);
        GPIO_setOutputLowOnPin(BAND_30M_SELECT);
        GPIO_setOutputLowOnPin(BAND_20M_SELECT);
        GPIO_setOutputLowOnPin(BAND_17M_SELECT);
        GPIO_setOutputLowOnPin(BAND_15M_SELECT);
        si5351FreqOut = BAND_40M_LOWER;
        si5351_set_RX_freq(si5351FreqOut);
        si5351_set_TX_freq(si5351FreqOut);
        selectSideband(LOWER_SIDEBAND);
        maxBandFreq = BAND_40M_UPPER;
        minBandFreq = BAND_40M_LOWER;
        break;
    case BAND_30M :
        GPIO_setOutputLowOnPin(BAND_40M_SELECT);
        GPIO_setOutputHighOnPin(BAND_30M_SELECT);
        GPIO_setOutputLowOnPin(BAND_20M_SELECT);
        GPIO_setOutputLowOnPin(BAND_17M_SELECT);
        GPIO_setOutputLowOnPin(BAND_15M_SELECT);
        si5351FreqOut = BAND_30M_LOWER;
        si5351_set_RX_freq(si5351FreqOut);
        si5351_set_TX_freq(si5351FreqOut);
        selectSideband(UPPER_SIDEBAND);
        maxBandFreq = BAND_30M_UPPER;
        minBandFreq = BAND_30M_LOWER;
        break;
    case BAND_20M :
        GPIO_setOutputLowOnPin(BAND_40M_SELECT);
        GPIO_setOutputLowOnPin(BAND_30M_SELECT);
        GPIO_setOutputHighOnPin(BAND_20M_SELECT);
        GPIO_setOutputLowOnPin(BAND_17M_SELECT);
        GPIO_setOutputLowOnPin(BAND_15M_SELECT);
        si5351FreqOut = BAND_20M_LOWER;
        si5351_set_RX_freq(si5351FreqOut);
        si5351_set_TX_freq(si5351FreqOut);
        selectSideband(UPPER_SIDEBAND);
        maxBandFreq = BAND_20M_UPPER;
        minBandFreq = BAND_20M_LOWER;
        break;
    case BAND_17M :
        GPIO_setOutputLowOnPin(BAND_40M_SELECT);
        GPIO_setOutputLowOnPin(BAND_30M_SELECT);
        GPIO_setOutputLowOnPin(BAND_20M_SELECT);
        GPIO_setOutputHighOnPin(BAND_17M_SELECT);
        GPIO_setOutputLowOnPin(BAND_15M_SELECT);
        si5351FreqOut = BAND_17M_LOWER;
        si5351_set_RX_freq(si5351FreqOut);
        si5351_set_TX_freq(si5351FreqOut);
        selectSideband(UPPER_SIDEBAND);
        maxBandFreq = BAND_17M_UPPER;
        minBandFreq = BAND_17M_LOWER;
        break;
    case BAND_15M :
        GPIO_setOutputLowOnPin(BAND_40M_SELECT);
        GPIO_setOutputLowOnPin(BAND_30M_SELECT);
        GPIO_setOutputLowOnPin(BAND_20M_SELECT);
        GPIO_setOutputLowOnPin(BAND_17M_SELECT);
        GPIO_setOutputHighOnPin(BAND_15M_SELECT);
        si5351FreqOut = BAND_15M_LOWER;
        si5351_set_RX_freq(si5351FreqOut);
        si5351_set_TX_freq(si5351FreqOut);
        selectSideband(UPPER_SIDEBAND);
        maxBandFreq = BAND_15M_UPPER;
        minBandFreq = BAND_15M_LOWER;
        break;
    default :
        break;
    }
    */
    /*
    // reset menu function
    ritState = DISABLED;
    ritOffset = 0;
    receiveMode = RXMODE_CW;
    initADC(BATTERY_MEASUREMENT);
    *
}
/*
 * Routine to update rate
 */
void handleHW_rate(const MenuItem_t *item, int16_t value)
{
    // value is the index into the *item list
    switch (value)
    {
    case RATE_10 :
        radioState.freqMultiplier = 10;
        break;
    case RATE_100 :
        radioState.freqMultiplier = 100;
        break;
    case RATE_1K :
        radioState.freqMultiplier = 1000;
        break;
    case RATE_10K :
        radioState.freqMultiplier = 10000;
        break;
    default:
        break;
    }
    moveFreqCursor(void);
}

// routine to select filter
static void selectFilter(uint8_t filter)
{
    radioState.selectedFilter = filter;
    if (filter == CW_FILTER)
        GPIO_setOutputLowOnPin(FILTER_SELECT);  // set low for CW filter
    else
        GPIO_setOutputHighOnPin(FILTER_SELECT); // set high for SSB filter
}

// routine to select sideband
static void selectSideband(uint8_t sideband)
{
    radioState.selectedSideband = sideband;
    if (sideband == UPPER_SIDEBAND)
        GPIO_setOutputHighOnPin(SIDEBAND_SELECT);  // need to check
    else
        GPIO_setOutputLowOnPin(SIDEBAND_SELECT); // need to check
}

// routine to set audio state - mute or unmute
static void selectAudioState(uint8_t state)
{
    radioState.audioState = state;
    if ( state == MUTE )
        GPIO_setOutputHighOnPin(TR_MUTE); // set high for mute pin
    else if (state == UNMUTE)
        GPIO_setOutputLowOnPin(TR_MUTE); // set low for unmute pin
}

