#include "timer.h"
#include <kernel/stdio.h>
#include <lib/string.h>
#include <drivers/screen/screen.h>
#include <drivers/keyboard/keyboard.h>

static unsigned long int timeCount=0;

void initTimer()
{
    unsigned int divisor=1193;/*interrupt ogni 1ms*/
    asm("cli");
    timeCount=0;
    outb(PIT_COMMREG,0x36);
    outb(PIT_DATAREG0,divisor & 0xFF);
    outb(PIT_DATAREG0,divisor >> 8);
    asm("sti");
}

void tick(void){
    timeCount++;
    if(timeCount%100==0)
    {
    char timestring[9]={0};
    int print=COLUMNS;
    itoa(timeCount,timestring);
    while(timestring[COLUMNS-print]!=0)
        print--;
    writexy(print,ROWS-1,timestring);
    }
}

int time(void){
    return timeCount;
}
