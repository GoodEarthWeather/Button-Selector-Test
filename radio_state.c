/*
 * radio_state.c
 */

#include "radio_state.h"
#include "lcdLib.h"
#include "menu_data.h"
//#include "main.h"


//static void selectFilter(uint8_t);
//static void selectSideband(uint8_t);
//static void selectAudioState(uint8_t);

// define rates - these numbers must match the order in the MenuItem_t definition
#define RATE_10 10
#define RATE_100 100
#define RATE_1K 1000
#define RATE_10K 10000
// define bands - these numbers must match the order in the MenuItem_t definition
//#define BAND_40M 0 => this is defined in radio_state.h because it is used by lcdLib.c
#define BAND_30M 1
#define BAND_20M 2
#define BAND_17M 3
#define BAND_15M 4

#define MUTE 0x1
#define UNMUTE 0x0

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
    //selectAudioState(MUTE);
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
    */
}
/*
 * Routine to update rate
 */
void handleHW_rate(const MenuItem_t *item, int16_t value)
{
    const uint32_t rateValues[] = {10,100,1000,10000};
    // value is the index into the *item list
    radioState.freqMultiplier = rateValues[value];
    moveFreqCursor();
}

/*******************
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
***************/
