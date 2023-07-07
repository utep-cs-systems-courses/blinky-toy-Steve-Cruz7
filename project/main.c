#include <msp430.h>
#include "libTimer.h"
#include "led.h"

int main(void) {
  P1DIR |= LEDS;         //Opens Transmission gate for Bits 0 and 6 (Red and Green)
  P1OUT |= GREEN;
  P1OUT &= ~RED;         //Code won't work until we add libTimer stuff to project directory

  configureClocks();
  enableWDTInterrupts();

  or_sr(0x18);
}

int state = 0;
int interrupts = 0;
int secondCount = 0;

void statePicker(int state){
  if(state == 0){
    quarter(interrupts);
  }
  else if(state == 1){
    eighth(interrupts);
  }
  else if(state == 2){
    triplet(interrupts);
  }
}

void //250 interrupts per second
__interrupt_vec(WDT_VECTOR) WDT() {   
  if(++interrupts == 250){
    interrupts = 0;
    if(secondCount == 4){
      secondCount = 1;
    }else{
      secondCount++;
    }
    
    if(state < 2 && secondCount == 4){
      state++;
    }else if(state >=  2  && secondCount == 4){
      state = 0;
    }
  }
  statePicker(state);
}
