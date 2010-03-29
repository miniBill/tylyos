#include "timer.h"
#include <kernel/stdio.h>

static unsigned long int timeCount=0;

void initTimer(){
    unsigned int divisor=1193;/*interrupt ogni 1ms*/
    asm ( "cli" );
    timeCount=0;
    outb ( PIT_COMMREG,0x36 );
    outb ( PIT_DATAREG0,divisor & 0xFF );
    outb ( PIT_DATAREG0,divisor >> 8 );
    asm ( "sti" );
}

extern char gui_background;

void tick(void){

}