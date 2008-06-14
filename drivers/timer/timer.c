#include "timer.h"
#include <lib/string.h>
#include <drivers/screen/screen.h>
#include <drivers/keyboard/keyboard.h>

static int timeS=0;
void tick(void){
    static short ticks=0;
    ticks++;
    if(ticks==0x40){
        char timestring[9]={0};
        int print=COLUMNS;
        ticks=0;
        timeS++;
        if(timeS==3600)
            timeS=0;
        itoa(timeS,timestring);
        while(timestring[COLUMNS-print]!=0)
            print--;
        writexy(print-1,ROWS-1,timestring);
        putxy(COLUMNS-1,ROWS-1,fetchch());
    }
}

int time(void){
    return timeS;
}
