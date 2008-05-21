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

#ifndef SCREEN_H_
#define SCREEN_H_

enum{
	Black		=0x00,
	Blue		=0x01,
	Green		=0x02,
	Cyan		=0x03,
	Red		=0x04,
	Magenta		=0x05,
	Brown		=0x06,
	Light_Gray	=0x07,
	Dark_Gray	=0x08,
	Light_Blue	=0x09,
	Light_Green	=0x0A,
	Light_Cyan	=0x0B,
	Light_Red	=0x0C,
	Light_Magenta	=0x0D,
	Yellow		=0x0E,
	White		=0x0F
};

#define consoleAddr	0xb8000
#define COLUMNS		80
#define ROWS		25

int pos();

void gotoi(int pos);
void gotoxy(int x,int y);

char read();
char readi(int pos);
char readxy(int x,int y);

void put(char c);
void puti(int pos, char c);
void putxy(int x,int y,char c);

void nl();

void write(char* string);
void writei(int pos,char * string);
void writexy(int x,int y,char * string);
void writeline(char* string);

void clearScreen();

/*This six methods are equal to those without the c, but they read/write the color byte*/
char cread();
char creadi(int pos);
char creadxy(int x,int y);

void cput(char color);
void cputi(int pos,char color);
void cputxy(int x,int y,char color);

#endif
