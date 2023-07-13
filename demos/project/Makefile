# makefile configuration
COMMON_OBJECTS = 
CPU	       = msp430g2553
CLFAGS         = -mmcu=${CPU} -I../h
LDFLAGS	       = -L/opt/ti/msp430_gcc/include

#switch the compiler (for the internal make rules)
CC	       = msp430-elf-gcc
AS	       = msp430-elf-as

all: led.elf

#additional rules for files
led.elf: ${COMMON_OBJECTS} main.o led.o ../lib/libTimer.a
	${CC} ${CLFAGS} ${LDFLAGS} -o $@ $^ 

load: led.elf
	msp430loader.sh led.elf

clean: $^
	rm -f *.o *.elf

main.c: led.h
led.c: led.h
wdInterruptHandler.o:  led.h
