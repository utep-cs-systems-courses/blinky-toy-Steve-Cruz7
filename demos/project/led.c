#include <msp430.h>
#include "libTimer.h"
#include "led.h"

void quarter(int interrupts){
  if(interrupts == 250){
    P1OUT |= GREEN;
  }
  else if(interrupts == 125){
    P1OUT &= ~GREEN;
  }
}


void eighth(int interrupts){
  interrupts *=2;
  if(interrupts == 250 || interrupts == 500){
    P1OUT |= GREEN;
  }
  else if(interrupts == 126 || interrupts == 376){
    P1OUT &= ~GREEN;
  }
}

void triplet(int interrupts){
  interrupts *= 3;
  if(interrupts == 250 || interrupts == 500 || interrupts == 750){
    P1OUT |= GREEN;
  }
  else if(interrupts == 126 || interrupts == 375 || interrupts == 627){
    P1OUT &= ~GREEN;
  }
}
