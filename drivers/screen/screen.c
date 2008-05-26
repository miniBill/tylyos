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

#define addr(pos)   (consoleAddr+pos*2)
#define xy          (COLUMNS*y+x)
#define total       (COLUMNS*ROWS)

char consoleColor=0x07;
char * pointer=(char *)consoleAddr;

void SetCursorPos(unsigned int x, unsigned int y)
{
   asm(
       "movl  %0, %%eax   \n"
       "movl  %1, %%ebx   \n"
       "movl  $0x50, %%ecx   \n"
       "mul   %%ecx         \n"
       "addl  %%ebx, %%eax   \n"
       "movw  $0x03d4, %%dx  \n"
       "pushl %%eax         \n"
       "movb  $0x0f, %%al    \n"
       "out   %%al, %%dx     \n"
       "popl  %%eax         \n"
       "movw  $0x03d5, %%dx  \n"
       "out   %%al, %%dx     \n"
       "shr   $0x08,%%eax      \n"
       "pushl %%eax         \n"
       "movw  $0x03d4, %%dx  \n"
       "movb  $0x0e, %%al    \n"
       "out   %%al, %%dx     \n"
       "pop   %%eax         \n"
       "movw  $0x03d5, %%dx  \n"
       "out   %%al, %%dx     \n"
      :
      : "g" (x), "g" (y)
      );
}

int pos(){
    return ((int)pointer-consoleAddr)/2;
}

void gotoi(int pos){
    pointer=(char *)addr(pos);
}

void gotoxy(int x,int y){
    gotoi(xy);
}

char read(){
    return *(pointer);
}

char readi(int pos){
    return *(int* )addr(pos);
}

char readxy(int x,int y){
    return readi(xy);
}

void put(char c){
    *(pointer++)=c;
    *(pointer++)=consoleColor;
}

void puti(int pos, char c){
    *(char *)addr(pos)=c;
    *(char *)(addr(pos)+1)=consoleColor;
}

void putxy(int x,int y,char c){
    puti(xy,c);
}

void nl(){
    gotoi(pos()+COLUMNS-pos()%COLUMNS);
}

void write(char* string){
    int k;
    for(k=0;string[k]!=0 && pos()<total;k++)
        if(string[k]!='\n')
            put(string[k]);
        else
            nl();
}

void writei(int i,char * string){
    int s=pos();
    gotoi(i);
    write(string);
    gotoi(s);
}

void writexy(int x,int y,char * string){
    writei(xy,string);
}

void writeline(char* string){
    write(string);
    nl();
}

void clearScreen(){
    gotoi(0);
    for(;pos()<total;)
        put(' ');
    gotoi(0);
}

/*This three methods are equal to those without the c, but they read the color*/
char cread(){
    return *(pointer+1);
}

char creadi(int pos){
    return *(int* )(addr(pos)+1);
}

char creadxy(int x,int y){
    return creadi(xy);
}

void cput(char color){
    *(pointer+1)=color;
}

void cputi(int pos,char color){
    *(char* )(addr(pos)+1)=color;
}

void cputxy(int x,int y,char color){
    cputi(xy,color);
}
