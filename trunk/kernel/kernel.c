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
#include "screen.h"
#include "stdio.h"
#include "../memory/memory.h"

const char life='#';
#define rows	18

int X(int x){
	if(x==-1)
		return COLUMNS-1;
	if(x==COLUMNS)
		return 0;
	return x;
}

int Y(int y){
	if(y==-1)
		return rows-1;
	if(y==rows)
		return 0;
	return y;
}

void lifenext(char prev[]){
	int x,y,n;
	char new[rows*COLUMNS];
	for(x=0;x<COLUMNS;x++){
		for(y=0;y<rows;y++){
			n=0;
			n+=(prev[X(x-1)+Y(y-1)*COLUMNS]==life)+
			   (prev[x+Y(y-1)*COLUMNS]==life)+
			   (prev[X(x+1)+Y(y-1)*COLUMNS]==life)+
			   (prev[X(x-1)+Y(y+1)*COLUMNS]==life)+
			   (prev[x+Y(y+1)*COLUMNS]==life)+
			   (prev[X(x+1)+Y(y+1)*COLUMNS]==life);
			n+=(prev[X(x-1)+Y(y)*COLUMNS]==life)+
			   (prev[X(x+1)+Y(y)*COLUMNS]==life);
			if(prev[x+y*COLUMNS]==life){
				if(n<2||n>3){
					putxy(x,y+7,' ');
					new[x+y*COLUMNS]=' ';
				}
				else{
					putxy(x,y+7,life);
					new[x+y*COLUMNS]=life;
				}
			}
			else{
				if(n==3){
					putxy(x,y+7,life);
					new[x+y*COLUMNS]=life;
				}
				else{
					putxy(x,y+7,' ');
					new[x+y*COLUMNS]=' ';
				}
			}
		}
	}
	for(x=0;x<rows*COLUMNS;x++)
		prev[x]=new[x];
}
void OK(int i){
	putxy(COLUMNS-2,i,'O');
	putxy(COLUMNS-1,i,'K');
	cwritexy(COLUMNS-2,i,Light_Green);
	cwritexy(COLUMNS-1,i,Light_Green);
}
short abs(short s){return s<0?-s:s;}

void _kmain(multiboot_info_t* mbd, unsigned int magic){
	char next[COLUMNS*rows];
	char * parameters="Parametri: \0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0";
	char * pointer="Prova puntatore";
	char * conversion="\0\0\0\0\0\0\0\0\0";
	short i;/*index*/
	int t=0;/*test number*/
	clearScreen();
	OK(t++);
	writeline("Prova writeline");
	OK(t++);
	writeline(pointer);
	OK(t++);
	write("Prova strapp(output: 101|C|A0):");
	strapp(conversion,"%b|",(void *)5);
	strapp(conversion,"%x|",(void *)12);
	strapp(conversion,"%x",(void *)160);
	writeline(conversion);
	OK(t++);
	strapp(parameters,"mbd.flags:%b|",(void *)mbd->flags);
	strapp(parameters,"magic:%d",(void *)magic);
	writeline(parameters);
	for(i=0;i<6;i++)
		put(read(i));
	writeline("put/read");
	OK(t++);
	writeline("Prova GDT");
	init_gdt();
	OK(t++);
	writeline("Ed ora, diamo il via alle danze!");
	OK(t++);
	i=0;
	/*LWSS*/
	next[0*COLUMNS+0]=life;
	next[0*COLUMNS+3]=life;
	next[1*COLUMNS+4]=life;
	next[2*COLUMNS+0]=life;
	next[2*COLUMNS+4]=life;
	next[3*COLUMNS+1]=life;
	next[3*COLUMNS+2]=life;
	next[3*COLUMNS+3]=life;
	next[3*COLUMNS+4]=life;
	/*glider*/
	next[6*COLUMNS+1]=life;
	next[7*COLUMNS+2]=life;
	next[8*COLUMNS+0]=life;
	next[8*COLUMNS+1]=life;
	next[8*COLUMNS+2]=life;
	while(1)
		lifenext(next);
}

