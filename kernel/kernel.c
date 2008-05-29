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

#include <gui/gui.h>

#define BASIC_TESTS

int on=1;

void halt(){
    on=0;
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


void _kmain(/*multiboot_info_t* mbd, unsigned int magic*/){
#ifdef BASIC_TESTS
    char pointer[17]="Prova puntatore.";
    char conversion[60]={0};
    char *dinamicFirst,*dinamicSecond;
#endif
    short i;/*index*/
    int t=0;/*test number*/

    clearScreen();

    NO(t);
    kwrite("Kernel caricato.");
    OK(t++);
    writeline("");

#ifdef BASIC_TESTS
    NO(t);
    writeline("Prova writeline.");
    OK(t++);
#endif

    logo();
    t+=5;

#ifdef BASIC_TESTS
    NO(t);
    put('P');
    for(i=(COLUMNS*2+1);i<(COLUMNS*2+6);i++)
        put(readi(i));
    writeline("put/read.");
    OK(t++);

    NO(t);
    writeline(pointer);
    OK(t++);

    NO(t);
    write("Prova strapp(output: 101,C,A0.):");
    strapp(conversion,"%b,",/*(void *)*/5);
    strapp(conversion,"%x,",/*(void *)*/12);
    strapp(conversion,"%x.",/*(void *)*/160);
    writeline(conversion);
    OK(t++);

#endif

#ifdef BASIC_TESTS
    NO(t);
    writeline("Prova GDT");
#endif
    initGdt();
#ifdef BASIC_TESTS
    OK(t++);
#endif

#ifdef BASIC_TESTS
    NO(t);
    writeline("Prova IDT");
#endif
    initIdt();
#ifdef BASIC_TESTS
    OK(t++);
#endif

#ifdef BASIC_TESTS
    NO(t);
    writeline("Prova Paging");
#endif
    initPaging();
#ifdef BASIC_TESTS
    OK(t++);
#endif

#ifdef BASIC_TESTS
    addNewPage(PAG_PRESENT|PAG_READWRITE|PAG_SUPERVISOR|PAG_4KPAGE);
    writeline("Test allocazione dinamica: ");
    t++;
    NO(t);
    dinamicFirst=malloc(40);
    conversion[0]='\0';
    strapp(conversion,"Prima allocazione: indirizzo: 0x%x",
           (unsigned int)dinamicFirst);
    writeline(conversion);
    OK(t++);

    NO(t);
    /*deletePage((unsigned int)dinamicTest);*/
    dinamicSecond=malloc(40);
    conversion[0]='\0';
    strapp(conversion,"Seconda allocazione: indirizzo: 0x%x",
           (unsigned int)dinamicSecond);
    writeline(conversion);
    if(dinamicFirst+40==dinamicSecond)
        OK(t++); 

#endif
    drawRectangle(5,17,10,5,(char)(Yellow|Back_Blue));
    asm("sti");
    setCursorPos(79,24);
    on=1;
    while(on);
}
