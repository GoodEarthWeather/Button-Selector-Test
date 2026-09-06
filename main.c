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


uint8_t menuEncoderCWCount, menuEncoderCCWCount;
uint8_t optionEncoderCWCount, optionEncoderCCWCount;

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
            if (GPIO_getInputPinValue(MENU_ENCODER_A) != GPIO_getInputPinValue(MENU_ENCODER_B))
            {
                menuEncoderCWCount++;
            } else {
                menuEncoderCCWCount++;
            }
            // now toggle interrupt edge
            P2IES ^= BIT1;
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

void send_byte_to_shift_register(uint8_t data)
{
    // Wait until the hardware transmit buffer is empty and ready
    while (!EUSCI_B_SPI_getInterruptStatus(EUSCI_B0_BASE, EUSCI_B_SPI_TRANSMIT_INTERRUPT));

    // Send the byte into the hardware buffer (it will auto-shift out 8 bits in the background)
    EUSCI_B_SPI_transmitData(EUSCI_B0_BASE, data);

    // Block until the shift register inside the eUSCI_B0 completely finishes serialization
    while (EUSCI_B_SPI_isBusy(EUSCI_B0_BASE));

    // Pulse the P1.7 Latch pin (RCLK) to push data to the 74HCT595 parallel physical pins
    GPIO_setOutputHighOnPin(SHIFTER_LATCH);
    __delay_cycles(10); // Small delay to satisfy 74HCT595 timing minimums
    GPIO_setOutputLowOnPin(SHIFTER_LATCH);
}









