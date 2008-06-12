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

#define FAST_TESTS

static int on=1;

static int magicNumber=0;

static multiboot_info_t * multiBootInfo;

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

int putreadtest(){
    char output[16]="Prova put/read.";
    int i,check=1;
    put('P');
    for(i=COLUMNS+1;i<COLUMNS+6;i++)
        put(readi(i));
    write("put/read.");
    for(i=0;i<15;i++)
        if(readxy(i+3,row())!=output[i]){
            check=0;
            cputxy(i+3,row(),Light_Red);
        }
    return check;
}

int pointertest(){
    char pointer[17]="Prova puntatore.";
    int i,check=1;
    write(pointer);
    for(i=0;i<16;i++)
        if(readxy(i+3,row())!=pointer[i]){
            check=0;
            cputxy(i+3,row(),Light_Red);
        }
    return check;
}

int itoatest(){
    char conversion[4]={0};
    char output[7]="123,A0";
    int i,check=1;
    write("Prova itoa:");
    for(i=0;i<4;i++)
        conversion[i]=0;
    itoa(123,conversion);
    write(conversion);
    write(",");
    itobase(160,16,conversion);
    write(conversion);
    write(".");
    for(i=0;i<6;i++)
        if(readxy(i+14,row())!=output[i]){
            check=0;
            cputxy(i+14,row(),Light_Red);
        }
    return check;
}

int strapptest(){
    char conversion[10]={0};
    char output[9]="101,C,A0";
    int i,check=1;
    write("Prova strapp:");
    strapp(conversion,"%b,",/*(void *)*/5);
    strapp(conversion,"%x,",/*(void *)*/12);
    strapp(conversion,"%x.",/*(void *)*/160);
    write(conversion);
    for(i=0;i<8;i++)
        if(readxy(i+16,row())!=output[i]){
            check=0;
            cputxy(i+16,row(),Light_Red);
        }
    return check;
}

int magictest(){
    char magicString[13]={0};
    write("Test magic number:");
    itobase(magicNumber,16,magicString);
    writeline(magicString);
    return magicNumber==0x2BADB002;
}

int mbdtest(){
    char lower[13]={0};
    char upper[10]={0};
    char totalM[10]={0};
    char totalK[5]={0};
    int l=multiBootInfo->mem_lower;
    int u=multiBootInfo->mem_upper;
    if (multiBootInfo->flags & 1){
        itoa(l,lower);
        itoa(u/1024,upper);
        itoa((l+u)/1024,totalM);
        itoa((l+u)%1024,totalK);
        write("Lower memory:");
        write(lower);
        write("Kb, Upper memory:");
        write(upper);
        write("Mb, Total memory:");
        write(totalM);
        write("Mb and ");
        write(totalK);
        write("Kb.");
    }
    return l>0;
}

int dinamictestOne(){
    char *dinamicFirst,*dinamicSecond;
    char conversion[60]={0};
    char number[3]={0};
    int c=0,i,check=1;
    char buff[5]="-/|\\";
    write("Test allocazione dinamica: fase1 ");

    putxy(39,row(),'%');

#ifdef FAST_TESTS
    for(i=0;i<1500;i++){
        if(!(i%15)){
#else
    for(i=0;i<4000;i++){
        if(!(i%40)){
#endif
            itoa(c,number);
            writexy(37+(c<10),row(),number);
            putxy(40,row(),buff[c%4]);
            c++;
        }

        dinamicSecond=dinamicFirst;
        dinamicFirst=(char*)malloc(4);

        *dinamicFirst=0;

        if((dinamicSecond+4)!=dinamicFirst &&
            getPageFromVirtualAdress((unsigned int)dinamicFirst)==
            getPageFromVirtualAdress((unsigned int)dinamicSecond)){
            check=0;
            conversion[0]='\0';
            strapp(conversion,"%d:",(unsigned int)i);
            strapp(conversion,"0x%x >> ",(unsigned int)dinamicSecond);
            strapp(conversion,"0x%x ",(unsigned int)dinamicFirst);
            write(conversion);
        }
    }
#ifdef DONTDEFINETHIS
    }}
#endif
    writexy(36,row(),"100");
    putxy(40,row(),' ');
    return check;
}

int dinamictestTwo(){
    char conversion[33]={0};
    char *dinamicFirst,*dinamicSecond;
    writexy(30,row(),"fase2 ");
    dinamicFirst=(char*)malloc(4);
    conversion[0]='\0';
    strapp(conversion,"0x%x=",(unsigned int)dinamicFirst);

    dinamicSecond=dinamicFirst;
    free(dinamicFirst,4);

    dinamicFirst=(char*)malloc(4);

    strapp(conversion,"=0x%x.",(unsigned int)dinamicFirst);
    writexy(36,row(),conversion);

    return dinamicFirst==dinamicSecond;
}

typedef int (*test)(void);

inline void greendot(void){write(" * ");cputxy(1,row(),Light_Green);}

int doTests(test tests[]){
    int i;
    int t=row()+1;
    for(i=0;tests[i]!=0;i++){
        NO(t);
        greendot();
        if((*tests[i])())
            OK(t++);
        else
            t++;
        writeline("");
    }
    return i;
}

void _kmain(multiboot_info_t* mbd, unsigned int magic){
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

    /*here start the true tests*/
    magicNumber=magic;
    multiBootInfo=mbd;

    {
        /*REMEMBER TO KEEP SIZE=ITEMS+1!!!*/
        test tests[7]={
            putreadtest,
            pointertest,
            itoatest,
            strapptest,
            magictest,
            mbdtest
        };
        t+=doTests(tests);
    }

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

    {
        /*REMEMBER TO KEEP SIZE=ITEMS+1!!!*/
        test tests[3]={
            dinamictestOne,
            dinamictestTwo
        };
        t+=doTests(tests);
    }

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
