/* Copyright (C) 2008 Leonardo Taglialegne <leonardotaglialegne+clearos@gmail.com>
 * Copyright (C) 2008 Luca Salmin
 *
 * This file is part of ClearOS.
 *
 * ClearOS is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * ClearOS is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with ClearOS.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "kernel.h"
#include "multiboot.h"
#include <lib/string.h>
#include <memory/memory.h>
#include <interrupt/interrupt.h>
#include <drivers/timer/timer.h>

#include <gui/gui.h>

static int on=1;

void halt(){
    on=0;
}

void OK(int i){
    writexy(COLUMNS-6,i,"[ ok ]");
    cputxy(COLUMNS-6,i,Blue);
    cputxy(COLUMNS-4,i,Light_Green);
    cputxy(COLUMNS-3,i,Light_Green);
    cputxy(COLUMNS-1,i,Blue);
}

void NO(int i){
    writexy(COLUMNS-6,i,"[ NO ]");
    cputxy(COLUMNS-6,i,Blue);
    cputxy(COLUMNS-4,i,Light_Red);
    cputxy(COLUMNS-3,i,Light_Red);
    cputxy(COLUMNS-1,i,Blue);
}

void kwrite(char * string){
    static char * kpointer=(char *)0xb8000;
    int k;
    for(k=0;
        string[k]!=0 && (((int)kpointer-0xb8000)/2)<(COLUMNS*ROWS);
        k++){
            *(kpointer++)=string[k];
            *(kpointer++)=7;
        }
}

void logo(){
    int i;
    const int left=22;
    for(i=0;i<left;i++)
        put(' ');
    writeline("   ________                ____      ");
    for(i=0;i<left;i++)
        put(' ');
    writeline("  / ____/ /__  ____  _____/ __ \\____");
    for(i=0;i<left;i++)
        put(' ');
    writeline(" / /   / / _ \\/ __ \\/ ___/ / / /  _/");
    for(i=0;i<left;i++)
        put(' ');
    writeline("/ /___/ /  __/ /_/ / /  / /_/ /\\  \\ ");
    for(i=0;i<left;i++)
        put(' ');
    writeline("\\____/_/\\___/\\____/_/   \\____//___/ ");
}

inline void greendot(void){write(" * ");cputxy(1,row(),Light_Green);}

void _kmain(void){
    int t=0;/*test number*/

    clearScreen();

    NO(t);
    kwrite("Kernel caricato.");
    OK(t++);
    writeline("");

    NO(t);
    writeline("Prova writeline.");
    OK(t++);

    logo();
    t+=5;

    NO(t);
    greendot();
    writeline("Inizializzazione GDT");
    initGdt();
    OK(t++);

    NO(t);
    greendot();
    writeline("Inizializzazione IDT");
    initIdt();
    OK(t++);

    NO(t);
    greendot();
    writeline("Inizializzazione Paging");
    initPaging();
    OK(t++);

    NO(t);
    greendot();
    writeline("Kernel pronto!!!");
    OK(t++);

    drawRectangle(0,t,COLUMNS-1,ROWS-t-2,(char)(Yellow|Back_Blue));
    gotoxy(1,t+1);
    asm("sti");
    setCursorPos(79,24);
    writexy(0,ROWS-1,"[s][c][a] Time:");
    on=1;
    while(on);
    {
        /*wait 3 seconds before halting*/
        int now=time();
        while((time()-now)<3);
    }
}
