/* Copyright (C) 2008 Leonardo Taglialegne <leonardotaglialegne+clearos@gmail.com>
 * Copyright (C) 2008 Roberto Domenella
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

#include "screen.h"
#include <memory/memory.h>
#include <lib/string.h>

#define addr(pos)   (consoleAddr+pos*2)
#define vaddr(pos)  (2*(pos+baseline*COLUMNS))
#define vpos()      vaddr(pos())
#define xy(x,y)     (COLUMNS*y+x)
#define total()     (COLUMNS*ROWS)

static char consoleColor    =White;
static char * pointer       =(char *)consoleAddr;
static char videoMemory[PAGES*ROWS*COLUMNS*2]={0};
static int baseline         =0;

void setCursorPos(unsigned int x, unsigned int y){
   asm(
       "movl  %0, %%eax     \n"
       "movl  %1, %%ebx     \n"
       "movl  $0x50, %%ecx  \n"
       "mul   %%ecx         \n"
       "addl  %%ebx, %%eax  \n"
       "movw  $0x03d4, %%dx \n"
       "pushl %%eax         \n"
       "movb  $0x0f, %%al   \n"
       "out   %%al, %%dx    \n"
       "popl  %%eax         \n"
       "movw  $0x03d5, %%dx \n"
       "out   %%al, %%dx    \n"
       "shr   $0x08,%%eax   \n"
       "pushl %%eax         \n"
       "movw  $0x03d4, %%dx \n"
       "movb  $0x0e, %%al   \n"
       "out   %%al, %%dx    \n"
       "pop   %%eax         \n"
       "movw  $0x03d5, %%dx \n"
       "out   %%al, %%dx    \n"
      :
      : "g" (y), "g" (x)
      );
}

int pos(void){
    return ((int)pointer-consoleAddr)/2;
}

int row(void){
    return (pos()-1)/COLUMNS;
}

void setCursor(void){
    setCursorPos(pos()%COLUMNS,pos()/COLUMNS);
}

void pageUp(void){
    scroll(-20);
}

void pageDown(void){
    scroll(20);
}

void scroll(int lines){
    int i;
    setCursor();
    baseline+=lines;
    if(baseline<0)
        baseline=0;
    if(baseline>(PAGES-1)*ROWS){
        int c=baseline-(PAGES-1)*ROWS;/*righe da tagliare*/
        for(i=0;i<((2*PAGES-1)*ROWS-baseline)*COLUMNS;i++){
            videoMemory[i*2]=videoMemory[(i+c*COLUMNS)*2];
            videoMemory[i*2+1]=videoMemory[(i+c*COLUMNS)*2+1];
        }
        for(;i<PAGES*ROWS*COLUMNS;i++){
            videoMemory[i*2]=' ';
            videoMemory[i*2+1]=consoleColor;
        }
        baseline=(PAGES-1)*ROWS;
    }
    for(i=0;i<total();i++){
        if(videoMemory[vaddr(i)]){
            *(char *)addr(i)=videoMemory[vaddr(i)];
            *(char *)(addr(i)+1)=videoMemory[vaddr(i)+1];
        }
        else{
            *(char *)addr(i)=' ';
            *(char *)(addr(i)+1)=consoleColor;
        }
    }
}

int checkscroll(int i){
    int lines=0;
    if(i<0){
        lines=(i+1)/COLUMNS-1;
    }
    else{
        if(i<total())
            return i;
        lines=(i-total())/COLUMNS+1;
    }
    scroll(lines);
    return i-lines*COLUMNS;
}

void gotoi(int i){
    i=checkscroll(i);
    pointer=(char *)addr(i);
    setCursor();
}

void gotoxy(int x,int y){
    gotoi(xy(x,y));
}

char read(){
    return *(pointer);
}

char readi(int i){
    return *(int* )addr(i);
}

char readxy(int x,int y){
    return readi(xy(x,y));
}

void put(char c){
    if(pos()>total())
        nl();
    videoMemory[vpos()]=c;
    videoMemory[vpos()+1]=consoleColor;
    *(pointer++)=c;
    *(pointer++)=consoleColor;
    setCursor();
}

void puti(int i, char c){
    i=checkscroll(i);
    *(char *)addr(i)=c;
    *(char *)(addr(i)+1)=consoleColor;
    videoMemory[vaddr(i)]=c;
    videoMemory[vaddr(i)+1]=consoleColor;
}

void putxy(int x,int y,char c){
    puti(xy(x,y),c);
}

void nl(){
    gotoi(pos()+COLUMNS-pos()%COLUMNS);
}

void write(const char* string){
    int k;
    for(k=0;string[k]!=0;k++)
        if(string[k]!='\n')
            put(string[k]);
        else
            nl();
}

void writei(int i,const char * string){
    int k;
    for(k=0;string[k]!=0;k++)
        if(string[k]!='\n')
            puti(i+k,string[k]);
    else
        i+=COLUMNS-i%COLUMNS;
}

void writexy(int x,int y,const char * string){
    writei(xy(x,y),string);
}

void writeline(const char* string){
    write(string);
    nl();
}

void clearScreen(){
    gotoi(0);
    for(;pos()<total();)
        put(' ');
    gotoi(0);
}

/*This three methods are equal to those without the c, but they read the color*/
char cread(){
    return *(pointer+1);
}

char creadi(int i){
    return *(int* )(addr(i)+1);
}

char creadxy(int x,int y){
    return creadi(xy(x,y));
}

void cput(unsigned char color){
    videoMemory[vpos()+1]=color;
    *(pointer+1)=color;
}

void cputi(int i,unsigned char color){
    i=checkscroll(i);
    *(char* )(addr(i)+1)=color;
    videoMemory[vaddr(i)+1]=color;
}

void cputxy(int x,int y,unsigned char color){
    cputi(xy(x,y),color);
}
