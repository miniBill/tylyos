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
#include <drivers/hdd/ata.h>

#include <gui/gui.h>

/*#define FAST_TESTS*/

static int on=1;

static int magicNumber=0;

static multiboot_info_t * multiBootInfo;

void reboot(void){
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

void kwrite(const char * string){
    static char * kpointer=(char *)0xb8000;
    int k;
    for(k=0;
        string[k]!=0 && (((int)kpointer-0xb8000)/2)<(COLUMNS*ROWS);
        k++){
            *(kpointer++)=string[k];
            *(kpointer++)=7;
        }
}

void logo(void){
    writeline("                         ________                ____      ");
    writeline("                        / ____/ /__  ____  _____/ __ \\____");
    writeline("                       / /   / / _ \\/ __ \\/ ___/ / / /  _/");
    writeline("                      / /___/ /  __/ /_/ / /  / /_/ /\\  \\ ");
    writeline("                      \\____/_/\\___/\\____/_/   \\____//___/ ");
}

int check(const char * output,int offset){
    int retval=1,i;
    for(i=0;i<strlen(output);i++)
        if(readxy(i+3+offset,row())!=output[i]){
            retval=0;
            cputxy(i+3+offset,row(),Light_Red);
        }
    return retval;
}

int putreadtest(void){
    char output[16]="Prova put/read.";
    int i;
    put('P');
    for(i=COLUMNS+1;i<COLUMNS+6;i++)
        put(readi(i));
    write("put/read.");
    return check(output,0);
}

int pointertest(void){
    char pointer[17]="Prova puntatore.";
    write(pointer);
    return check(pointer,0);
}

int itoatest(void){
    char conversion[4]={0};
    char output[8]="123,-A0";
    int i;
    write("Prova itoa:");
    for(i=0;i<4;i++)
        conversion[i]=0;
    itoa(123,conversion);
    write(conversion);
    write(",");
    itobase(-160,16,conversion);
    write(conversion);
    write(".");
    return check(output,11);
}

int printftest(void){
    char output[13]="10,CA,a,ciao";
    write("Prova printf:");
    printf("%d,%x,%c,%s",10,0xCA,'a',"ciao");
    printf(" base stack: %x end: %x",getEBP(),getESP());
    return check(output,13);
}

int magictest(void){
    char magicString[13]={0};
    write("Test magic number:");
    itobase(magicNumber,16,magicString);
    write(magicString);
    return magicNumber==0x2BADB002;
}

int mbdtest(void){
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

int dinamictestOne(void){
    unsigned int pointera,pointerb,pointerc;
    int ret=1;
    write("Test allocazione dinamica: fase1 ");

    printf("heap start %d",mallocMemoryStart);
    pointera=(unsigned int)kmalloc(4);
    pointerb=(unsigned int)kmalloc(4);
    pointerc=(unsigned int)kmalloc(4);


    if(!(pointerc-pointerb == 12 &&
       pointerb-pointera == 12))
	   ret=0;

    kfree((void*)pointera);
    if(!(pointera==(unsigned int)kmalloc(4)))
    {
       printf("a\n");
       ret=0;
    }
    kfree((void*)pointerb);
    if(!(pointerb==(unsigned int)kmalloc(4)))
    {
       printf("b\n");
       ret=0;
    }
    kfree((void*)pointerc);
    if(!(pointerc==(unsigned int)kmalloc(4)))
    {
       printf("c\n");
       ret=0;
    }
    return ret;
}

int dinamictestTwo(void){
    char *dinamicFirst,*dinamicSecond;
    int i;
    for(i=0;i<27;i++)
        put(' ');
    write("fase2 ");
    dinamicFirst=(char*)kmalloc(4);
    printf("0x%x=",(unsigned int)dinamicFirst);

    dinamicSecond=dinamicFirst;
    kfree(dinamicFirst);

    dinamicFirst=(char*)kmalloc(4);

    printf("=0x%x.",(unsigned int)dinamicFirst);

    return dinamicFirst==dinamicSecond;
}

inline void greendot(void){write(" * ");cputxy(1,row(),Light_Green);}

int checkHdd(int t,int controller,int disk){
	int present=0;
	NO(t);
	greendot();
	present=isHddPresent(controller,disk);
	printf("Rilevamento presenza   hdd: %s %s\n",disk?"slave     ":"master    ",present?"presente":"assente");
	if(present)
		OK(t);
	return t+1;
}

int checkController(int t,int controller){
	int present=0;
	NO(t);
	greendot();
	present=isControllerPresent(controller);
	printf("Rilevamento controller hdd: %s %s\n",controller?"secondario":"primario  ",present?"presente":"assente");
	if(present)
		OK(t);
	else
		return t+1;
	t++;
	
	t=checkHdd(t,controller,0);
	t=checkHdd(t,controller,1);
	
	return t;
}

int checkHdds(int t){
	t=checkController(t,0);
	t=checkController(t,1);
	return t;
}

typedef int (*test)(void);

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

    /*here start the true tests*/
    magicNumber=magic;
    multiBootInfo=mbd;

    {
        /*REMEMBER TO KEEP SIZE=ITEMS+1!!!*/
        test tests[8]={
            putreadtest,
            pointertest,
            itoatest,
            printftest,
            magictest,
            mbdtest,
            dinamictestOne,
            /*dinamictestTwo*/
        };
        t+=doTests(tests);
    }

    NO(t);
    greendot();
    writeline("Kernel pronto!!!");
    OK(t++);

    t=checkHdds(t);

    drawRectangle(0,t,COLUMNS-1,ROWS-t-2,(char)(Yellow|Back_Blue));
    gotoxy(1,t+1);
    setCursorPos(79,24);
    asm("sti");
    writexy(0,ROWS-1,"[s][c][a][n][k] Time:");
    on=1;
    while(on);
    {
        /*wait 3 seconds before halting*/
        int now=time();
        while((time()-now)<3);

        asm("cli");
        clearIdt();
        asm("int $1");
    }
}

void sleep(int i){
    int c;
    for(c=0;c<100*i;c++)
        ;
}
