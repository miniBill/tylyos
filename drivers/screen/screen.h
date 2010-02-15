/* Copyright (C) 2008 Leonardo Taglialegne <leonardotaglialegne+tylyos@gmail.com>
 * Copyright (C) 2008 Luca Salmin
 * Copyright (C) 2008 Roberto Domenella
 *
 * This file is part of TylyOS.
 *
 * TylyOS is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * TylyOS is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with TylyOS.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef SCREEN_H_
#define SCREEN_H_

enum {
  Black         = 0x00,
  Blue          = 0x01,
  Green         = 0x02,
  Cyan          = 0x03,
  Red           = 0x04,
  Magenta       = 0x05,
  Brown         = 0x06,
  Light_Gray    = 0x07,
  Dark_Gray     = 0x08,
  Light_Blue    = 0x09,
  Light_Green   = 0x0A,
  Light_Cyan    = 0x0B,
  Light_Red     = 0x0C,
  Light_Magenta = 0x0D,
  Yellow        = 0x0E,
  White         = 0x0F
};

enum {
  Back_Black         = 0x00,
  Back_Blue          = 0x10,
  Back_Green         = 0x20,
  Back_Cyan          = 0x30,
  Back_Red           = 0x40,
  Back_Magenta       = 0x50,
  Back_Brown         = 0x60,
  Back_Light_Gray    = 0x70,
  Back_Dark_Gray     = 0x80,
  Back_Light_Blue    = 0x90,
  Back_Light_Green   = 0xA0,
  Back_Light_Cyan    = 0xB0,
  Back_Light_Red     = 0xC0,
  Back_Light_Magenta = 0xD0,
  Back_Yellow        = 0xE0,
  Back_White         = 0xF0
};

extern unsigned char vga_mode;

enum{
  consoleAddr = 0xB8000,
  COLUMNS     = 64,//HACK for vga mode
  PAGES       =  4,
  ROWS        = 30,//HACK for vga mode
  CONSOLE     =  6,
  VGA_dx      =  5
};

//activate graphical console
void go_graphic(void);

//index is zero-based
void switch_console(unsigned int console);

unsigned int current_console(void);

void clear_all(void);
void clear_physical(void);

void set_physical_color(unsigned char color);
///Set cursor physical position
void set_cursor(unsigned int x, unsigned int y);

void scroll(unsigned int console, int n);

void goto_x(unsigned int console, unsigned int x);
void goto_y(unsigned int console, unsigned int y);
void goto_current_xy(unsigned int x, unsigned int y);
void goto_xy(unsigned int console, unsigned int x, unsigned int y);

int row(unsigned int console);
int col(unsigned int console);
void nl(unsigned int console);

void write(const char* string, unsigned int console);
void write_current(const char* string);
void write_xy(const char* string, unsigned int console, unsigned int x, unsigned int y);

void write_physical_xy(const char* string, unsigned int x, unsigned int y);

void put(char c, unsigned int console);
void put_x(char c, unsigned int console, unsigned int x);
void put_xy(char c, unsigned int console, unsigned int x, unsigned int y);

void put_physical(char c);
void put_physical_xy(char c, unsigned int x, unsigned int y);

void put_color_x(unsigned char color, unsigned int console, unsigned int x);
void put_color_xy(unsigned char color, unsigned int console, unsigned int x, unsigned int y);

void put_physical_color_xy(unsigned char color, unsigned int x, unsigned int y);

char read_x(unsigned int x, unsigned int console);

#endif
