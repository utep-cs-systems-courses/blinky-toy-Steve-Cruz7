#include <msp430.h>
#include "libTimer.h"
#include "buzzer.h"
#include "led.h"
#include "switch.h"

int main() {
    configureClocks();
    enableWDTInterrupts();  //Enables the 250/s interrupts
    
    //led stuff
    P1DIR |= LEDS;         //Sets outputs 
    P1OUT &= ~LEDS;        //Turns off leds
    P1OUT |= GREEN;        //Turning on the Green LED
    
    //bottom switches stuff
    P1REN |= BSWITCHES;     //enables resistors for switches
    P1IE |= BSWITCHES;      //enables interrupts switches
    P1OUT |= BSWITCHES;     //pull-ups for switches
    P1DIR &= ~BSWITCHES;    //set switches bits for input

    //top switches stuff
    P2REN |= TSWITCHES;    //Same stuff but for Port 2 and top switches
    P2IE |= TSWITCHES;
    P2OUT |= TSWITCHES;
    P2DIR &= ~TSWITCHES;
    
    buzzer_init();        //Start buzzer
  


    or_sr(0x18);          // CPU off, GIE on
}

int state = 0;
int interrupts = 1;
int secondCount = 0;

void
bswitch_interrupt_handler()
{
  char p1val = P1IN;

//update switch interrupt sense to detect changes from current buttons
  P1IES |= (p1val & BSWITCHES);  //If switch up, sense down
  P1IES &= (p1val | ~BSWITCHES); //If switch down, sense up

// up = red off, down = red on
  if(p1val & SW1){
    P1OUT &= ~RED;
    state = 0;
    secondCount = 0;
    interrupts = 1;
  }
  else{
    P1OUT |= RED;
    state = 1;
    secondCount = 0;
    interrupts = 1;
  }
}

//Switch on P1 (S2)
void
__interrupt_vec(PORT1_VECTOR) Port_1(){
  if(P1IFG & BSWITCHES){
    P1IFG &= ~BSWITCHES;
    buzzer_set_period(0);
    bswitch_interrupt_handler();
  }
}

//Switches on P2
void
__interrupt_vec(PORT2_VECTOR) Port_2(){
  if(P2IFG & TSW1){
    P2IFG &= ~TSW1;
    playFsharp();
    //call on switch 1 state;
    
  }
  else if(P2IFG & TSW2){
    P2IFG &= ~TSW2;
    playG();
    //call on switch 2 state;
  }
  else if(P2IFG & TSW3){               //If an input is detected from buttons, do something
    P2IFG &= ~TSW3;                    //Eventually need to just change states for WDT interrupt
    playA();
    //call on switch 3 state;          //In order to play songs
  }
  else if(P2IFG & TSW4){
    P2IFG &= ~TSW4;
    playCsharp();
    //call on switch 4 state;
  }
}




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
  // buzzer_set_period(0);	 start buzzing!!! 2MHz/interrupts = 2mHz happens x times
  
  if((interrupts++) == 250){
    interrupts = 0;
    if(secondCount == 3){
      if(state < 1){
	state ++;
      }else{
	state = 0;
      }
      secondCount = 0;
    }else{
      secondCount++;
    }
  }
}
