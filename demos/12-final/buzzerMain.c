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
  }
  else{
    P1OUT |= RED;
    state = 1;
  }
}

void
tswitch_interrupt_handler(){
  char p2val = P2IN;

  P2IES |= (p2val & TSWITCHES);  //If the switch isnt pressed, sense down
  P2IES &= (p2val | ~TSWITCHES); //If the switch is pressed, sense up

  //Different cases for each top switch (press == 0, unpress == 1 )
  if(p2val ^ TSW1){  //change p2val & TSW1 to P2IFG & TSW1 
    buzzer_set_period(5000);
  }
  else if(p2val ^ TSW2){
    buzzer_set_period(5100);
  }
  else if(p2val ^ TSW3){               //If a button is pressed, a tone will be played
    buzzer_set_period(5200);           //Otherwise, turn off tone
  }
  else if(p2val ^ TSW4){
    buzzer_set_period(5300);
  }
  else{
    buzzer_set_period(0);
  }
}

//Switch on P1 (S2)
void
__interrupt_vec(PORT1_VECTOR) Port_1(){
  if(P1IFG & BSWITCHES){
    P1IFG &= ~BSWITCHES;
    bswitch_interrupt_handler();
  }
}

//Switches on P2
void
__interrupt_vec(PORT2_VECTOR) Port_2(){
  if(P2IFG & TSWITCHES){
    P2IFG &= ~TSWITCHES;
    tswitch_interrupt_handler();
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
  //buzzer_set_period(interrupts);	/* start buzzing!!! 2MHz/interrupts = 2mHz happens x times
  
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
