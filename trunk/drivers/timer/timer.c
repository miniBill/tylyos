#include "timer.h"
#include <kernel/stdio.h>
#include <lib/string.h>
#include <drivers/screen/screen.h>
#include <drivers/keyboard/keyboard.h>

static unsigned long int timeCount=0;

void initTimer()
{
    unsigned int divisor=1000;/*TODO:ricontrollare l'inizializzazione in modo da garantire tick ad intervalli conosciuti*/
    asm("cli");
    timeCount=0;
    outb(PIT_COMMREG,0x34);
    outb(PIT_DATAREG0,divisor & 0xFF);
    outb(PIT_DATAREG0,divisor >> 8);
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
