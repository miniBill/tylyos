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

char consoleColor=0x07;
char * pointer=(char *)consoleAddr;

int pos(){
	return ((int)pointer-consoleAddr)/2;
}

void setpointer(int pos){
	pointer=(char *)(consoleAddr+pos*2);
}

void clearScreen(){
	setpointer(0);
	for(;pos()<(COLUMNS*ROWS);)
		put(' ');
	setpointer(0);
}

void gotoxy(int x,int y){
	setpointer(COLUMNS*y+x);
}

void put(char c){
	*(pointer++)=c;
	*(pointer++)=consoleColor;
}

void putxy(int x,int y,char c){
	*(char *)(consoleAddr+(COLUMNS*y+x)*2)=c;
	*(char *)(consoleAddr+(COLUMNS*y+x)*2+1)=consoleColor;
}

void write(char* string){
	int k;
	for(k=0;string[k]!=0 && pos()<(COLUMNS*ROWS);k++)
		put(string[k]);
}

char read(int index){
	return *(int* )(consoleAddr+index*2);
}

char readcurr(){
	return *(pointer);
}

char readxy(int x,int y){
	return read(COLUMNS*y+x);
}

/*This three methods are equal to those without the c, but they read the color*/
char cread(int index){
	return *(int* )(consoleAddr+index*2+1);
}

char creadcurr(){
	return *(pointer+1);
}

char creadxy(int x,int y){
	return cread(COLUMNS*y+x);
}

void cwrite(int index,char color){
	*(char* )(consoleAddr+index*2+1)=color;
}

void cwritecurr(char color){
	*(pointer+1)=color;
}

void cwritexy(int x,int y,char color){
	cwrite(COLUMNS*y+x,color);
}

void writeline(char* string){
	write(string);
	setpointer(pos()+COLUMNS-pos()%COLUMNS);
}
