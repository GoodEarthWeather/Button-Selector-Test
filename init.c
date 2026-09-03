/*
 * init.c
 *
 *  Created on: June 26, 2019
 *      Author: dmcneill
 *//////
#include "main.h"

//This file contains the routines to initialize everything
// QEX Amplifier Tester

// initialize the clock system
void initClocks(void)
{

    //Initialize external 32.768kHz clock
    CS_setExternalClockSource(32768);
    CS_turnOnXT1LF(CS_XT1_DRIVE_3);

    //Set DCO frequency to 8MHz
    CS_initClockSignal(CS_FLLREF,CS_XT1CLK_SELECT,CS_CLOCK_DIVIDER_1);
    CS_initFLLSettle(8000,244);  // 244*32.768 is approximately 8000kHz = 8MHz
    //Set ACLK = External 32.768kHz clock with frequency divider of 1
    CS_initClockSignal(CS_ACLK,CS_XT1CLK_SELECT,CS_CLOCK_DIVIDER_1);
    //Set SMCLK = DCO with frequency divider of 1
    CS_initClockSignal(CS_SMCLK,CS_DCOCLKDIV_SELECT,CS_CLOCK_DIVIDER_1);
    //Set MCLK = DCO with frequency divider of 1
    CS_initClockSignal(CS_MCLK,CS_DCOCLKDIV_SELECT,CS_CLOCK_DIVIDER_1);

    //Clear all OSC fault flag
    CS_clearAllOscFlagsWithTimeout(1000);
}

// initialize GPIO
void initGPIO(void)
{
  /*
   * Set Pin 2.0, 2.1 to input Primary Module Function, LFXT.
   * This is for configuration of the external 32.768kHz crystal
   */
   GPIO_setAsPeripheralModuleFunctionInputPin(
       GPIO_PORT_P2,
       GPIO_PIN0 + GPIO_PIN1,
       GPIO_PRIMARY_MODULE_FUNCTION
   );

   // 1. Configure Hardware SPI Pins for eUSCI_B0
   // P1.1 = UCB0CLK (Clock Out)
   // P1.2 = UCB0SIMO (Data Out)
   // GPIO_PRIMARY_MODULE_FUNCTION activates the internal eUSCI hardware multiplexer
   GPIO_setAsPeripheralModuleFunctionOutputPin(
           GPIO_PORT_P1,
           GPIO_PIN1 | GPIO_PIN2,
           GPIO_PRIMARY_MODULE_FUNCTION
   );

   /*
    * Disable the GPIO power-on default high-impedance mode to activate
    * previously configured port settings
    */
   PMM_unlockLPM5();
}

void init_spi_shift_register(void)
{

    // 2. Configure Latch Pin (P1.7) as a standard GPIO Output
    GPIO_setAsOutputPin(LATCH_PORT, LATCH_PIN);
    GPIO_setOutputLowOnPin(LATCH_PORT, LATCH_PIN);

    // 3. Initialize eUSCI_B0 SPI Master Configuration Structure
    EUSCI_B_SPI_initMasterParam param = {0};
    param.selectClockSource = EUSCI_B_SPI_CLOCKSOURCE_SMCLK;             // Use Sub-Main Clock
    param.clockSourceFrequency = CS_getSMCLK();                          // Grab current SMCLK frequency automatically
    param.desiredSpiClock = 1000000;                                     // Drive transmission at 1 MHz
    param.msbFirst = EUSCI_B_SPI_MSB_FIRST;                              // 74HCT595 shifts in MSB first
    param.clockPolarity = EUSCI_B_SPI_CLOCKPOLARITY_INACTIVITY_LOW;      // CPOL = 0 (Clock low when idle)
    param.clockPhase = EUSCI_B_SPI_PHASE_DATA_CAPTURED_ONFIRST_CHANGED_ON_NEXT; // CPHA = 0 (Data changes on falling, latched on rising edge)
    param.spiMode = EUSCI_B_SPI_3PIN;                                    // 3-Wire Mode (Clock, SIMO, ignoring SOMI line)

    // 4. Initialize and Enable the eUSCI_B0 Peripheral Module
    EUSCI_B_SPI_initMaster(EUSCI_B0_BASE, &param);
    EUSCI_B_SPI_enable(EUSCI_B0_BASE);

    // Clear residual eUSCI_B0 RX flag interrupts before starting
    EUSCI_B_SPI_clearInterrupt(EUSCI_B0_BASE, EUSCI_B_SPI_RECEIVE_INTERRUPT);

}

