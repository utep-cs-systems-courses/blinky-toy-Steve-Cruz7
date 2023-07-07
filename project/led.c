#inlcude "led.h"
#include "libTimer.h"
#include <msp430.h>

void quarter(int interrupts){//I want the light to blink at a quarter note's speed once per second
  if(interrupts == 250){     //Every second turn it on
    P1OUT |= GREEN;
  }
  else if(interrupts == 125){ //Every half second turn it off 
    P1OUT &= ~GREEN;
  }
}

void eighth(int interrupts){ //Blinking at eighth note speed
  interrupts *= 2;
  if(interrupts == 250 || interrupts == 500 ){
    P1OUT |= GREEN;
  }
  else if(interrupts == 126 || interrupts == 376){
    P1OUT &= ~GREEN;
  }
}

void triplet(int interrupts){ //Blinking at triplet speed
  interrupts *= 3;
  if(interrupts == 250 || interrupts == 500 || interrupts == 750){
    P1OUT |= GREEN;
  }
  else if(interrupts == 126 || interrupts == 375 || interrupts == 627){
    P1OUT &= ~GREEN;
  }
}
