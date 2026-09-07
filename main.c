#include "driverlib.h"
#include "main.h"

/*
 * This program uses the MSP430FR2476 eval board to test the
 * button selector board.
 * 3 pins for the shifter - shiftClk, shiftData and registerClk
 * 6 pins for two encoders with switches
 * 6 pins for driving LCD
 * P1.1, P1.2 and P1.7 for shifter
 * P4.6, P2.1, P2.0 - for menu encoder
 * P3.2, P3.6, P4.2 for menu option encoder
 * P5.2, P5.1, P5.0, P4.7, P6.0, P3.3 for LCD
 */
// Define all I/O


int8_t menuEncoderCWCount, menuEncoderCCWCount;
int8_t optionEncoderCWCount, optionEncoderCCWCount;


// Track valid button press states globally
volatile uint8_t button_pressed_flag = 0;

int main(void) {

    WDT_A_hold(WDT_A_BASE);
    initClocks();
    initGPIO();
    init_spi_shift_register(); // Hardware setup from the previous step

    while (1)
    {
        switch (buttonPressed)
        {
        case BTN_PRESSED_NONE :
            break;
        case BTN_PRESSED_MENU_ENCODER_SWITCH:
            break;
        case BTN_PRESSED_OPTION_ENCODER_SWITCH :
            break;
        case BTN_PRESSED_MENU_ENCODER :
            handle_menu_encoder();
            break;
        case BTN_PRESSED_OPTION_ENCODER :
            if (GPIO_getInputPinValue(OPTION_ENCODER_A) != GPIO_getInputPinValue(OPTION_ENCODER_B))
            {
                optionEncoderCWCount++;
            } else {
                optionEncoderCCWCount++;
            }
            // now toggle interrupt edge
            P3IES ^= BIT2;
            break;


            // Execute task: Send shifted data byte over eUSCI_B0
            send_byte_to_shift_register(data_to_send);
            delay_ms(12);
            send_byte_to_shift_register(0);

        }
    }
}

/*
 * This routine will handle response to the menu encoder being rotated.
 */
void handle_menu_encoder(void)
{
    uint8_t led_index;

    // toggle interrupt edge - for encoder detection
    P2IES ^= BIT1;
    if (GPIO_getInputPinValue(MENU_ENCODER_A) != GPIO_getInputPinValue(MENU_ENCODER_B))
    {
        menuEncoderCWCount++;
        Menu_SelectMove(menuEncoderCWCount);
    } else {
        menuEncoderCCWCount++;
        Menu_SelectMove(menuEncoderCCWCount);
    }
    menuEncoderCWCount = menuEncoderCCWCount = 0;
    // menuSelectedIndex now contains the index to the new selected menu
    // now get led index associated with the new menu index
    led_index = Menu_GetSelectedLedIndex(void);
    // now send this to the LED shifter to update the LEDs
    updateLEDShifter(led_index);
    // now update LCD status field to show current menu option
    updateLCD_menu



}

// This routine will update menu LEDs
// Sends '3' bytes out to a chain of cascaded 74HCT595s and latches once.
// data[2] = byte for the FIRST chip in the chain (closest to MCU, SER pin)
// data[1] = byte for the SECOND chip
// data[0] = byte for the THIRD chip
// (byte order is reversed internally since the chain shifts "backwards")
void updateLEDShifter( uint8_t index)
{
    uint8_t i;
    uint8_t data[3];
    uint32_t selectedLED;

    selectedLED = (1 << index);  // convert number to bit
    selectedLED = ~selectedLED;  // invert all bits to match HW implementation of turning on LED
    // construct bytes to send
    data[2] = (uint8_t)(selectedLED & 0x000000FF);
    data[1] = (uint8_t)((selectedLED >> 8) & 0x000000FF);
    data[0] = (uint8_t)((selectedLED >> 16) & 0x000000FF);

    // Send last chip's byte first, first chip's byte last
    for (i = 0; i < 3; i++)
    {
        while (!EUSCI_B_SPI_getInterruptStatus(EUSCI_B0_BASE, EUSCI_B_SPI_TRANSMIT_INTERRUPT));
        EUSCI_B_SPI_transmitData(EUSCI_B0_BASE, data[i]);
    }

    // Wait for the last byte to fully clock out before latching
    while (EUSCI_B_SPI_isBusy(EUSCI_B0_BASE));

    // Latch all 24 bits to the outputs simultaneously
    GPIO_setOutputHighOnPin(SHIFTER_LATCH);
    __delay_cycles(10);
    GPIO_setOutputLowOnPin(SHIFTER_LATCH);
}







