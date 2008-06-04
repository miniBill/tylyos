#include "timer.h"
#include <lib/string.h>
#include <drivers/screen/screen.h>

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
        strapp(timestring,"%d",timeS);
        while(timestring[COLUMNS-print]!=0)
            print--;
        writexy(print,24,timestring);
    }
}

int time(void){
    return timeS;
}
