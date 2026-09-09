/*
 * isr.c
 *
 *  Created on: Sep 5, 2026
 *      Author: david
 */
#include "driverlib.h"
#include "isr.h"

uint8_t volatile buttonPressed;

// Port 4 interrupt service routine
#pragma vector=PORT4_VECTOR
__interrupt void Port_4(void)
{
    switch(__even_in_range(P4IV,P4IV_P4IFG7))
    {
      case  P4IV_P4IFG2:
          // P4.2 = Option encoder switch
          buttonPressed = BTN_PRESSED_OPTION_ENCODER_SWITCH;
          break;
      case  P4IV_P4IFG6:
          // P4.6 = menu encoder switch
          buttonPressed = BTN_PRESSED_MENU_ENCODER_SWITCH;
          break;
      default: break;
    }
}
// Port 3 interrupt service routine
#pragma vector=PORT3_VECTOR
__interrupt void Port_3(void)
{
    switch(__even_in_range(P3IV,P3IV_P3IFG7))
    {
      case  P3IV_P3IFG2:
          // P3.2 = Option Encoder A
          buttonPressed = BTN_PRESSED_OPTION_ENCODER;
          // now toggle interrupt edge
          P3IES ^= BIT2;
          break;
      default: break;
    }
}


// Port 2 interrupt service routine
#pragma vector=PORT2_VECTOR
__interrupt void Port_2(void)
{
    switch(__even_in_range(P2IV,P2IV_P2IFG7))
    {
      case  P2IV_P2IFG1:
          // P2.1 = Menu Encoder A
          buttonPressed = BTN_PRESSED_MENU_ENCODER;
          // toggle interrupt edge - for encoder detection
          P2IES ^= BIT1;
          break;
      default: break;
    }
}


