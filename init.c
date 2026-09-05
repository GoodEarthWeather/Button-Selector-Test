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
   GPIO_setAsPeripheralModuleFunctionOutputPin(SHIFTER_CLOCK,GPIO_PRIMARY_MODULE_FUNCTION);
   GPIO_setAsPeripheralModuleFunctionOutputPin(SHIFTER_DATA,GPIO_PRIMARY_MODULE_FUNCTION);
   GPIO_setAsOutputPin(SHIFTER_LATCH);
   GPIO_setOutputLowOnPin(SHIFTER_LATCH);

   //Initialize rotary encoder inputs and rotary encoder switches
   // P4.6(switch), P2.1(A), P2.0(B) - for menu encoder
   // P3.2(A), P3.6(B), P4.2(switch) for menu option encoder
   GPIO_setAsInputPin(MENU_ENCODER_A);
   GPIO_setAsInputPin(MENU_ENCODER_B);
   GPIO_setAsInputPin(MENU_ENCODER_SWITCH);
   GPIO_setAsInputPin(OPTION_ENCODER_A);
   GPIO_setAsInputPin(OPTION_ENCODER_B);
   GPIO_setAsInputPin(OPTION_ENCODER_SWITCH);

   GPIO_selectInterruptEdge(MENU_ENCODER_A, GPIO_HIGH_TO_LOW_TRANSITION);  // interrupt on falling edge of menu encoder pin A
   GPIO_selectInterruptEdge(MENU_ENCODER_SWITCH, GPIO_HIGH_TO_LOW_TRANSITION);  // interrupt on falling edge of menu encoder switch
   GPIO_selectInterruptEdge(OPTION_ENCODER_A, GPIO_HIGH_TO_LOW_TRANSITION);  // interrupt on falling edge of menu option encoder pin A
   GPIO_selectInterruptEdge(OPTION_ENCODER_SWITCH, GPIO_HIGH_TO_LOW_TRANSITION);  // interrupt on falling edge of menu option encoder switch

   // Configure interrupts for encoder
   GPIO_enableInterrupt(MENU_ENCODER_A);
   GPIO_clearInterrupt(MENU_ENCODER_A);
   GPIO_enableInterrupt(MENU_ENCODER_SWITCH);
   GPIO_clearInterrupt(MENU_ENCODER_SWITCH);
   GPIO_enableInterrupt(OPTION_ENCODER_A);
   GPIO_clearInterrupt(OPTION_ENCODER_A);
   GPIO_enableInterrupt(OPTION_ENCODER_SWITCH);
   GPIO_clearInterrupt(OPTION_ENCODER_SWITCH);


   /*
    * Disable the GPIO power-on default high-impedance mode to activate
    * previously configured port settings
    */
   PMM_unlockLPM5();
}

void init_switch(void)
{
    // 1. Configure S1 (P4.0) as Input with an Internal Pull-Up Resistor
    GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P4, GPIO_PIN0);

    // 2. Configure P4.0 Interrupts: Trigger on a High-to-Low transition (falling edge)
    GPIO_selectInterruptEdge(GPIO_PORT_P4, GPIO_PIN0, GPIO_HIGH_TO_LOW_TRANSITION);
    GPIO_clearInterrupt(GPIO_PORT_P4, GPIO_PIN0);
    GPIO_enableInterrupt(GPIO_PORT_P4, GPIO_PIN0);

    // 1. Configure S2 (P2.3) as Input with an Internal Pull-Up Resistor
    GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P2, GPIO_PIN3);

    // 2. Configure P2.3 Interrupts: Trigger on a High-to-Low transition (falling edge)
    GPIO_selectInterruptEdge(GPIO_PORT_P2, GPIO_PIN3, GPIO_HIGH_TO_LOW_TRANSITION);
    GPIO_clearInterrupt(GPIO_PORT_P2, GPIO_PIN3);
    GPIO_enableInterrupt(GPIO_PORT_P2, GPIO_PIN3);

    // 3. Configure Timer_A3 for the Debounce Timeout Period
    // Using SMCLK (assumed ~1MHz) in Up Mode. 15,000 counts at 1MHz = 15ms delay.
    Timer_A_initUpModeParam timerParam = {0};
    timerParam.clockSource = TIMER_A_CLOCKSOURCE_SMCLK;
    timerParam.clockSourceDivider = TIMER_A_CLOCKSOURCE_DIVIDER_1;
    timerParam.timerPeriod = 15000;
    timerParam.timerInterruptEnable_TAIE = TIMER_A_TAIE_INTERRUPT_DISABLE;
    timerParam.captureCompareInterruptEnable_CCR0_CCIE = TIMER_A_CCIE_CCR0_INTERRUPT_ENABLE; // Enable CCR0 Interrupt
    timerParam.timerClear = TIMER_A_DO_CLEAR;
    timerParam.startTimer = false; // Do not start running yet

    Timer_A_initUpMode(TIMER_A3_BASE, &timerParam);

    // Disable FRAM power/IO lock (Required step for MSP430 FR2xx line)
    //PMM_unlockLPM5();

    // Enable Global Interrupts (GIE bit in Status Register)
    __enable_interrupt();
}

void init_spi_shift_register(void)
{


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

