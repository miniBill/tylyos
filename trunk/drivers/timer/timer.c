#include "timer.h"
#include <lib/string.h>
#include <drivers/screen/screen.h>

void tick(void){
    static short ticks=0;
    static int time=0;
    ticks++;
    if(ticks==0x40){
        char timestring[9]={0};
        int print=COLUMNS;
        ticks=0;
        time++;
        if(time==3600)
            time=0;
        strapp(timestring,"%d",time);
        while(timestring[COLUMNS-print]!=0)
            print--;
        writexy(print,24,timestring);
    }
}
