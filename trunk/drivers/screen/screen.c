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

#include "screen.h"
#include <memory/memory.h>
#include <lib/string.h>

static char consoleColor  = White;

static char videoMemory[CONSOLE][ROWS*PAGES][COLUMNS];
static char colorMemory[CONSOLE][ROWS*PAGES][COLUMNS];

//index of first displayed line
//static int baseline       = 0;
static int currentConsole = 0;

static unsigned int x = 0;
static unsigned int y = 0;

static unsigned int py = 0;

//physical implicit
static inline char * addr_xy(unsigned int x,unsigned int y){
  return (char*)(consoleAddr+2*(x+COLUMNS*y));
}

//physical implicit
static inline char * addr_x(unsigned int x){
  return addr_xy(x,py);
}

static inline void inc(unsigned int * x,unsigned int * y){
  *x=(*x)+1;
  if(*x==COLUMNS){
    *x=0;
    *y=(*y)+1;
  }
}

void switch_console(int console){
  if(console==currentConsole)
    return;
  currentConsole=console;
  for(py=0;py<ROWS-1;py++)
    for(x=0;x<COLUMNS;x++){
      put_physical_xy(videoMemory[currentConsole][py][x],x,py);
      put_physical_color_xy(colorMemory[currentConsole][py][x],x,py);
    }
}

void clear_physical(void){
  for(py=0;py<ROWS;py++)
    for(x=0;x<COLUMNS;x++)
      put_physical_xy(' ',x,py);
}

void set_physical_color(unsigned char color){
  consoleColor=color;
}

void set_cursor(unsigned int x,unsigned int y){
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
  : "g"(y), "g"(x)
  );
}

void goto_physical_x(unsigned int nx){
  x=nx;
}

void goto_physical_y(unsigned int ny){
  py=ny;
}

void goto_physical_xy(unsigned int nx,unsigned int ny){
  x=nx;
  py=ny;
}

int row(void){
  return y;
}

int row_physical(void){
  return py;
}

int col(void){
  return x;
}

void nl(void){
  x=0;
  y++;
  nl_physical();
}

void nl_physical(void){
  x=0;
  py++;
}

void write(const char* string){
  for(int k = 0; string[k] != 0; k++)
    if(string[k] != '\n')
      put(string[k]);
    else
      nl();
}

void write_xy(const char* string,unsigned int x,unsigned int y){
  int k;
  for (k = 0;string[k] != 0;k++) {
    put_xy(string[k],x,y);
    inc(&x,&y);
  }
}

void write_physical(const char* string){
  for(int k = 0; string[k] != 0; k++)
    if(string[k] != '\n')
      put_physical(string[k]);
    else
      nl_physical();
}
void write_physical_xy(const char* string, unsigned int x, unsigned int y){
  int k;
  for (k = 0;string[k] != 0;k++) {
    put_physical_xy(string[k],x,y);
    x++;
    if(x==COLUMNS){
      x=0;
      y++;
    }
  }
}

void put(char c){
  put_xy(c,x,y);
  put_color_xy(consoleColor,x,y);
  inc(&x,&y);
}

void put_xy(char c, unsigned int x, unsigned int y){
  videoMemory[currentConsole][y][x]=c;
  put_physical_xy(c,x,y);
  put_color_xy(consoleColor,x,y);
}

void put_physical(char c){
  *addr_xy(x,py)=c;
  inc(&x,&py);
}

void put_physical_x(char c, unsigned int x){
  *addr_x(x)=c;
  put_physical_color_x(consoleColor,x);
}

void put_physical_xy(char c, unsigned int x, unsigned int y){
  *addr_xy(x,y)=c;
  put_physical_color_xy(consoleColor,x,y);
}

void put_color_x(unsigned char color,unsigned int x){
  colorMemory[currentConsole][y][x]=color;
  put_physical_color_x(color,x);
}

void put_color_xy(unsigned char color,unsigned int x,unsigned int y){
  colorMemory[currentConsole][y][x]=color;
  put_physical_color_xy(color,x,y);
}

void put_physical_color_x(unsigned char color, unsigned int x){
  *(addr_x(x)+1)=color;
}

void put_physical_color_xy(unsigned char color, unsigned int x, unsigned int y){
  *(addr_xy(x,y)+1)=color;
}

char read_x(unsigned int x){
  return videoMemory[currentConsole][y][x];
}