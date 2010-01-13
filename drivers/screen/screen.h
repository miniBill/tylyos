/* Copyright (C) 2008 Leonardo Taglialegne <leonardotaglialegne+clearos@gmail.com>
 * Copyright (C) 2008 Luca Salmin
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
    Black          =0x00,
    Blue           =0x01,
    Green          =0x02,
    Cyan           =0x03,
    Red            =0x04,
    Magenta        =0x05,
    Brown          =0x06,
    Light_Gray     =0x07,
    Dark_Gray      =0x08,
    Light_Blue     =0x09,
    Light_Green    =0x0A,
    Light_Cyan     =0x0B,
    Light_Red      =0x0C,
    Light_Magenta  =0x0D,
    Yellow         =0x0E,
    White          =0x0F
};

enum{
    Back_Black         =0x00,
    Back_Blue          =0x10,
    Back_Green         =0x20,
    Back_Cyan          =0x30,
    Back_Red           =0x40,
    Back_Magenta       =0x50,
    Back_Brown         =0x60,
    Back_Light_Gray    =0x70,
    Back_Dark_Gray     =0x80,
    Back_Light_Blue    =0x90,
    Back_Light_Green   =0xA0,
    Back_Light_Cyan    =0xB0,
    Back_Light_Red     =0xC0,
    Back_Light_Magenta =0xD0,
    Back_Yellow        =0xE0,
    Back_White         =0xF0
};

#define consoleAddr 0xb8000
#define COLUMNS     80
#define PAGES       4
#define ROWS        25

void setConsoleColor(unsigned char color);

void setCursorPos(unsigned int x, unsigned int y);

/* *signed* int, for backscrolling */
void scroll(int lines);

int pos(void);
int row(void);

void gotoi(int pos);
void gotoxy(int x,int y);

char read(void);
char readi(int pos);
char readxy(int x,int y);

void put(char c);
void puti(int pos, char c);
void putxy(int x,int y,char c);

void nl(void);

void write(const char* string);
void writei(int pos,const char * string);
void writexy(int x,int y,const char * string);
void writeline(const char* string);

void clearScreen(void);
void clearScreenAndColor(unsigned char color);

/*This six methods are equal to those without the c, but they read/write the color byte*/
char cread(void);
char creadi(int pos);
char creadxy(int x,int y);

void cput(unsigned char color);
void cputi(int pos,unsigned char color);
void cputxy(int x,int y,unsigned char color);

#endif
