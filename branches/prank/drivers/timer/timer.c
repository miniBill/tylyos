#include "timer.h"
#include <kernel/stdio.h>
#include <lib/string.h>
#include <drivers/screen/screen.h>
#include <drivers/keyboard/keyboard.h>

static unsigned long int timeCount=0;

void initTimer()
{
    unsigned int divisor=1193;/*interrupt ogni 1ms*/
    asm ( "cli" );
    timeCount=0;
    outb ( PIT_COMMREG,0x36 );
    outb ( PIT_DATAREG0,divisor & 0xFF );
    outb ( PIT_DATAREG0,divisor >> 8 );
    asm ( "sti" );
}

extern char gui_background;

void tick ( void )
{
    timeCount++;
    if ( timeCount%1000==0 )
    {
        char timestring[9]={0};
        int print=COLUMNS;
        itoa ( timeCount/1000,timestring );
        while ( timestring[COLUMNS-print]!=0 )
            print--;
    
        int tc=timeCount/1000;
        if(tc>3){
            gui_background=tc%256;
            void VGA_clear_screen(void);
            VGA_clear_screen();
            write_xy(timestring,0,0,0);
        }
    }
}

unsigned long int time ( void )
{
    return timeCount;
}

void sleep ( unsigned int i )
{
    unsigned long int temp=time();
    while ( time()-temp<i );
}
