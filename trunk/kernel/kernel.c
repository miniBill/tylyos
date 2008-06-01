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
#define FAST_TESTS

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

int putreadtest(){
    char output[16]="Prova put/read.";
    int i,check=1;
    put('P');
    for(i=COLUMNS+1;i<COLUMNS+6;i++)
        put(readi(i));
    writeline("put/read.");
    for(i=0;i<15;i++)
        if(readxy(i,row())!=output[i]){
        check=0;
        {
            char number[13]={0};
            itoa(row(),number);
            write("|");
            write(number);
            write("|");
        }
        cputxy(i,row(),Light_Red);
        }
    return check;
}

int pointertest(){
    char pointer[17]="Prova puntatore.";
    int i,check=1;
    writeline(pointer);
    for(i=0;i<16;i++)
        if(readxy(i,row())!=pointer[i]){
            check=0;
            cputxy(i,row(),Light_Red);
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
    writeline(".");
    for(i=0;i<6;i++)
        if(readxy(i+11,row())!=output[i]){
            check=0;
            cputxy(i+11,row(),Light_Red);
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
    writeline(conversion);
    for(i=0;i<8;i++)
        if(readxy(i+13,row())!=output[i]){
            check=0;
            cputxy(i+13,row(),Light_Red);
        }
    return check;
}

int magictest(int magic){
    char magicString[13]={0};
    write("Test magic number:");
    itobase(magic,16,magicString);
    writeline(magicString);
    return magic==0x2BADB002;
}

int mbdtest(multiboot_info_t * mbd){
    char lower[13]={0};
    char upper[10]={0};
    char totalM[10]={0};
    char totalK[5]={0};
    if (mbd->flags & 1){
        itoa(mbd->mem_lower,lower);
        itoa(mbd->mem_upper/1024,upper);
        itoa((mbd->mem_lower+mbd->mem_upper)/1024,totalM);
        itoa((mbd->mem_lower+mbd->mem_upper)%1024,totalK);
        write("Lower memory:");
        write(lower);
        write("Kb, Upper memory:");
        write(upper);
        write("Mb, Total memory:");
        write(totalM);
        write("Mb and ");
        write(totalK);
        writeline("Kb.");
    }
    return mbd->mem_lower>0;
}

int dinamictestOne(){
    char *dinamicFirst,*dinamicSecond;
    char conversion[60]={0};
    char number[3]={0};
    int c=0,i,check=1;
    char buff[5]="-/|\\";
    write("Test allocazione dinamica: fase1 ");

    putxy(36,row(),'%');

#ifdef FAST_TESTS
    for(i=0;i<1500;i++){
        if(!(i%15)){
#else
    for(i=0;i<4000;i++){
        if(!(i%40)){
#endif
            itoa(c,number);
            writexy(34+(c<10),row(),number);
            putxy(37,row(),buff[c%4]);
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
    writexy(33,row(),"100");
    putxy(37,row(),' ');
    writeline("");
    return check;
}

int dinamictestTwo(){
    char conversion[33]={0};
    char *dinamicFirst,*dinamicSecond;
    put(' ');/*HACK*/
    writexy(27,row(),"fase2 ");
    dinamicFirst=(char*)malloc(4);
    conversion[0]='\0';
    strapp(conversion,"0x%x=",(unsigned int)dinamicFirst);

    dinamicSecond=dinamicFirst;
    free(dinamicFirst,4);

    dinamicFirst=(char*)malloc(4);

    strapp(conversion,"=0x%x.",(unsigned int)dinamicFirst);
    writexy(33,row(),conversion);
    writeline("");

    return dinamicFirst==dinamicSecond;
}

void _kmain(multiboot_info_t* mbd, unsigned int magic){
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

    /*here start the true tests*/

#ifdef BASIC_TESTS
    NO(t);
    if(putreadtest())
        OK(t++);
    else
        t++;
    NO(t);
    if(pointertest())
        OK(t++);
    else
        t++;
    NO(t);
    if(itoatest())
        OK(t++);
    else
        t++;
    NO(t);
    if(strapptest())
        OK(t++);
    else
        t++;
    NO(t);
    if(magictest(magic))
        OK(t++);
    else
        t++;
    NO(t);
    if(mbdtest(mbd))
        OK(t++);
    else
        t++;
#endif

    NO(t);
    writeline("Inizializzazione GDT");
    initGdt();
    OK(t++);

    NO(t);
    writeline("Inizializzazione IDT");
    initIdt();
    OK(t++);

    NO(t);
    writeline("Inizializzazione Paging");
    initPaging();
    OK(t++);

#ifdef BASIC_TESTS
    NO(t);
    if(dinamictestOne())
        OK(t++);
    else
        t++;
    NO(t);
    if(dinamictestTwo())
        OK(t++);
    else
        t++;
#endif

    writeline("Kernel pronto!!!");
    OK(t++);

    drawRectangle(0,t,COLUMNS-1,ROWS-t-2,(char)(Yellow|Back_Blue));
    asm("sti");
    setCursorPos(79,24);
    writexy(0,ROWS-1,"Time:");
    on=1;
    while(on);
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
