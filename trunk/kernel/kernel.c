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

void putreadtest(int t){
    char output[16]="Prova put/read.";
    int i,check=1;
    NO(t);
    put('P');
    for(i=COLUMNS+1;i<COLUMNS+6;i++)
        put(readi(i));
    writeline("put/read.");
    for(i=0;i<15;i++)
        if(readxy(i,t)!=output[i]){
        check=0;
        cputxy(i,t,Light_Red);
        }
    if(check)
        OK(t);
}

void pointertest(int t){
    char pointer[17]="Prova puntatore.";
    int i,check=1;
    NO(t);
    writeline(pointer);
    for(i=0;i<16;i++)
        if(readxy(i,t)!=pointer[i]){
            check=0;
            cputxy(i,t,Light_Red);
        }
    if(check)
        OK(t);
}

void itoatest(int t){
    char conversion[4]={0};
    char output[7]="123,A0";
    int i,check=1;
    NO(t);
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
        if(readxy(i+11,t)!=output[i]){
            check=0;
            cputxy(i+11,t,Light_Red);
        }
    if(check)
        OK(t);
}

void strapptest(int t){
    char conversion[10]={0};
    char output[9]="101,C,A0";
    int i,check=1;
    NO(t);
    write("Prova strapp:");
    strapp(conversion,"%b,",/*(void *)*/5);
    strapp(conversion,"%x,",/*(void *)*/12);
    strapp(conversion,"%x.",/*(void *)*/160);
    writeline(conversion);
    for(i=0;i<8;i++)
        if(readxy(i+13,t)!=output[i]){
            check=0;
            cputxy(i+13,t,Light_Red);
        }
    if(check)
        OK(t);
}

void magictest(int t,int magic,multiboot_info_t * mbd){
    char magicString[13]={0};
    NO(t);
    write("Test magic number:");
    itobase(magic,16,magicString);
    writeline(magicString);
    if(magic==0x2BADB002)
        OK(t);
    itobase(mbd->flags,16,magicString);
}

void dinamictest(int *t){
    char *dinamicFirst,*dinamicSecond;
    char conversion[60]={0};
    char number[3]={0};
    int c=0,i,check=1;
    char buff[5]="-/|\\";
    NO(*t);
    write("Test allocazione dinamica: fase1 ");

    putxy(33,*t,'%');

#ifdef FAST_TESTS
    for(i=0;i<1500;i++){
        if(!(i%15)){
#else
    for(i=0;i<4000;i++){
        if(!(i%40)){
#endif
            itoa(c,number);
            writexy(34+(c<10),*t,number);
            putxy(37,*t,buff[c%4]);
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
    writexy(34+(c<10),*t,"100");
    if(check)
        OK(*t);
    *t+=1;
    NO(*t);
    writeline("");
    write("                           fase2 ");
    dinamicFirst=(char*)malloc(4);
    conversion[0]='\0';
    strapp(conversion,"0x%x ",(unsigned int)dinamicFirst);
   
    dinamicSecond=dinamicFirst;
    free(dinamicFirst,4);
   
    dinamicFirst=(char*)malloc(4);
   
    strapp(conversion,"0x%x ",(unsigned int)dinamicFirst);
    writeline(conversion);

     if(dinamicFirst==dinamicSecond)
        OK(*t);

    
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
    putreadtest(t++);
    pointertest(t++);
    itoatest(t++);
    strapptest(t++);
    magictest(t++,magic,mbd);
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
    
    dinamictest(&t);
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
