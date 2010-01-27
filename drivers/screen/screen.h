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
#define CONSOLE     6

//index is zero-based
void switch_console(unsigned int console);

unsigned int current_console(void);

void clear_all(void);
void clear_physical(void);

void set_physical_color(unsigned char color);
void set_cursor(unsigned int x,unsigned int y);

void scroll(int n);

void goto_x(unsigned int x);
void goto_y(unsigned int y);
void goto_xy(unsigned int x,unsigned int y);

int row(void);
int row_physical(void);

int col(void);

void nl(void);
void nl_physical(void);

void write(const char* string);
void write_xy(const char* string,unsigned int x,unsigned int y);

//void write_physical(const char* string);
void write_physical_xy(const char* string, unsigned int x, unsigned int y);

void put(char c);
void put_x(char c, unsigned int x);
void put_xy(char c, unsigned int x, unsigned int y);

void put_physical(char c);
void put_physical_x(char c, unsigned int x);
void put_physical_xy(char c, unsigned int x, unsigned int y);

void put_color_x(unsigned char color,unsigned int x);
void put_color_xy(unsigned char color,unsigned int x,unsigned int y);

void put_physical_color_x(unsigned char color, unsigned int x);
void put_physical_color_xy(unsigned char color, unsigned int x, unsigned int y);

char read_x(unsigned int x);

#endif
