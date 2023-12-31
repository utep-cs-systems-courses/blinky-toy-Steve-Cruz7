//Alternate LEDs from Off, Green, and Red
#include <msp430.h>
#include "libTimer.h"
#include "led.h"

int main(void) {
  P1DIR |= LEDS;       //Opens transmission gate for bits 6 and 0 i.e Green and Red led.
  P1OUT &= ~LED_GREEN; //Turns off the green with & and its inverse
  P1OUT &= ~LED_RED;
  // P1OUT |= LED_RED;    //Turns on just bit 0. LED_RED's value here is 1 or 0000 0001
  // P1OUT |= LED_GREEN;
  or_sr(0x18);		/* CPU off, GIE on */
}
