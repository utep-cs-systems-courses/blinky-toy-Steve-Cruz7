#ifndef led_included
#define led_included

#define RED BIT0       //P1.0
#define GREEN BIT6    //P1.6
#define LEDS (BIT0 | BIT6)


void quarter(int interrupts);

void eighth(int interrupts);

void triplet(int interrupts);

#endif
