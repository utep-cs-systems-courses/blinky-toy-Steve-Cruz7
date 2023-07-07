//Alternate LEDs from Off, Green, and Red
#include <msp430.h>
#include "libTimer.h"
#include "led.h"

int main(void) {
  P1DIR |= LEDS;
  P1OUT &= ~LED_GREEN;
  P1OUT |= LED_RED;
  // P1OUT &= ~LED_RED;
  
  configureClocks();		/* setup master oscillator, CPU & peripheral clocks */
  enableWDTInterrupts();	/* enable periodic interrupt */
  
  or_sr(0x18);		/* CPU off, GIE on */
}

char greenOn = 0;
int redCount = 0;
int secondCount = 0;
//if green on, then red should be bright
//if green off, then red should be dim


void
__interrupt_vec(WDT_VECTOR) WDT(){	/* 250 interrupts/sec */
  if((++secondCount) == 250){
    secondCount = 0;
    greenOn = !greenOn;
     if(greenOn){
       P1OUT |= LED_GREEN;
       P1OUT &= ~LED_RED;
     }
     else{
       P1OUT &= ~LED_GREEN;
     }
  }
  if(!greenOn){   //Flash red on and off quickly; turning off more often than on
    if(redCount >= 6){
      redCount = 0;
      P1OUT |= LED_RED;
    }
    else{
      P1OUT &= ~LED_RED;
      redCount++;
    }
  }

} 

