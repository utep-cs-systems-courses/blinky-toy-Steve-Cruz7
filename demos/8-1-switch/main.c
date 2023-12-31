#include <msp430.h>
#include "libTimer.h"

void quarter(int interrupts);
void eighth(int interrupts);
void triplet(int interrupts);


#define LED_RED BIT0               // P1.0
#define LED_GREEN BIT6             // P1.6
#define LEDS (BIT0 | BIT6)

#define SW1 BIT3		/* switch1 is p1.3 */
#define SWITCHES SW1		/* only 1 switch on this board */

void main(void) 
{  
  configureClocks();
  enableWDTInterrupts();

  P1DIR |= LEDS;
  P1OUT &= ~LEDS;		/* leds initially off */
  
  P1REN |= SWITCHES;		/* enables resistors for switches */
  P1IE |= SWITCHES;		/* enable interrupts from switches */
  P1OUT |= SWITCHES;		/* pull-ups for switches */
  P1DIR &= ~SWITCHES;           /* set switches' bits for input */

  P1OUT |= LED_GREEN;

  or_sr(0x18);  // CPU off, GIE on
} 

int state = 0;

void
switch_interrupt_handler()
{
  char p1val = P1IN;		/* switch is in P1 */

/* update switch interrupt sense to detect changes from current buttons */
  P1IES |= (p1val & SWITCHES);	/* if switch up, sense down */
  P1IES &= (p1val | ~SWITCHES);	/* if switch down, sense up */

/* up=red, down=green */
  if (p1val & SW1) {
     P1OUT &= ~LED_RED;                //Everytime switch is pressed, switch states;
     state = 0;
  }
  else {
    P1OUT |= LED_RED;
    state = 1;
  }
}


/* Switch on P1 (S2) */
void
__interrupt_vec(PORT1_VECTOR) Port_1(){
  if (P1IFG & SWITCHES) {	      /* did a button cause this interrupt? */
    P1IFG &= ~SWITCHES;		      /* clear pending sw interrupts */
    switch_interrupt_handler();	/* single handler for all switches */
  }
}

int interrupts = 1;
int secondCount = 0;

void statePicker(int state){
  if(state == 0){
    quarter(interrupts);
  }
  else if(state == 1){
    eighth(interrupts); 
  }
}




void
__interrupt_vec(WDT_VECTOR) WDT(){
  statePicker(state);
  if((interrupts++) == 250){
    interrupts = 0;
    if(secondCount == 3){
      if(state < 1){
	state++;
      }else{
	state = 0;
      }
      secondCount = 0;
    }else{
      secondCount++;
      
    }   
  }
 
}

void quarter(int interrupts){
  if(interrupts == 250){
    P1OUT |= LED_GREEN;
  }
  else if(interrupts == 125){
    P1OUT &= ~LED_GREEN;
  }
}

void eighth(int interrupts){
  interrupts *=2;
  if(interrupts == 250 || interrupts == 500){
    P1OUT |= LED_GREEN;
  }
  else if(interrupts == 126 || interrupts == 376){
    P1OUT &= ~LED_GREEN;
  }
}

