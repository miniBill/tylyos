#include "timer.h"
#include <kernel/stdio.h>
#include <lib/string.h>
#include <drivers/screen/screen.h>
#include <drivers/keyboard/keyboard.h>

static int timeCount=0;

void initTimer()
{
    int divisor=65535;
    asm("cli");
    timeCount=0;
    outb(0x36,PIT_COMMREG);
    outb(divisor & 0xFF,PIT_DATAREG0);
    outb(divisor >> 8,PIT_DATAREG0);
    asm("sti");
}

void tick(void){
    static short ticks=0;
    ticks++;
    if(ticks==0x40){
        char timestring[9]={0};
        int print=COLUMNS;
        ticks=0;
        timeCount++;
        if(timeCount==3600)
            timeCount=0;
        itoa(timeCount,timestring);
        while(timestring[COLUMNS-print]!=0)
            print--;
        writexy(print,ROWS-1,timestring);
    }
}

int time(void){
    return timeCount;
}
