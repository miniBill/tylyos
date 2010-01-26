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
static unsigned int baseline       = 0;
static unsigned int currentConsole = 0;

static unsigned int x = 0;
static unsigned int y = 0;

//static unsigned int py = 0;

//physical implicit
static inline char * addr_xy(unsigned int x,unsigned int y){
  return (char*)(consoleAddr+2*(x+COLUMNS*y));
}

//physical implicit
static inline char * addr_x(unsigned int x){
  return addr_xy(x,y);
}

void switch_console(unsigned int console){
  if(console==currentConsole)
    return;
  currentConsole=console;
  baseline=0;
  for(int y=0;y<ROWS-1;y++)
    for(int x=0;x<COLUMNS;x++){
      put_physical_xy(videoMemory[currentConsole][y+baseline][x],x,y);
      put_physical_color_xy(colorMemory[currentConsole][y+baseline][x],x,y);
    }
}

void update(void){
  for(int ty=0;ty<ROWS-1;ty++)
    for(int tx=0;tx<COLUMNS;tx++){
      put_physical_xy(videoMemory[currentConsole][ty+baseline][tx],tx,ty);
      put_physical_color_xy(colorMemory[currentConsole][ty+baseline][tx],tx,ty);
    }
}

void clear_all(void){
  for(int c=0;c<CONSOLE;c++)
    for(int y=0;y<ROWS*PAGES;y++)
      for(int x=0;x<COLUMNS;x++){
        videoMemory[c][y][x]=' ';
        colorMemory[c][y][x]=consoleColor;
      }
  baseline=0;
  clear_physical();
}

void clear_physical(void){
  for(int y=0;y<ROWS;y++)
    for(int x=0;x<COLUMNS;x++)
      put_physical_xy(' ',x,y);
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

void scroll(int d){
  if(((int)baseline+d)<0)
    baseline=0;
  else
    baseline+=d;
  update();
}

void goto_x(unsigned int nx){
  if(nx>=COLUMNS)
    goto_xy(nx-COLUMNS,baseline+y+1);
  else{
    x=nx;
    set_cursor(x,y);
  }
}

void goto_y(unsigned int ny){
  if(ny<baseline)
    scroll(ny-baseline);
  else if(ny>baseline+ROWS-2)
    scroll(ny-baseline-ROWS+2);
  y=ny-baseline;
  set_cursor(x,y);
}

void goto_xy(unsigned int nx,unsigned int ny){
  goto_x(nx);
  goto_y(ny);
}

int row(void){
  return y+baseline;
}

int col(void){
  return x;
}

void nl(void){
  goto_xy(0,y+1);
}

void write(const char* string){
  for(int k = 0; string[k] != 0; k++)
    if(string[k] != '\n')
      put(string[k]);
    else
      nl();
}

void write_xy(const char* string,unsigned int px,unsigned int py){
  unsigned int ox=x;
  unsigned int oy=y;
  goto_xy(px,py);
  write(string);
  goto_xy(ox,oy);
}

void write_physical(const char* string){
  for(int k = 0; string[k] != 0; k++)
    if(string[k] != '\n')
      put_physical(string[k]);
    else
      nl();
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
  put_xy(c,x,baseline+y);
  put_color_xy(consoleColor,x,baseline+y);
  goto_x(x+1);
}

void put_x(char c, unsigned int x){
  videoMemory[currentConsole][baseline+y][x]=c;
  put_physical_x(c,x);
  put_color_x(consoleColor,x);
}

void put_xy(char c, unsigned int x, unsigned int y){
  videoMemory[currentConsole][y][x]=c;
  put_physical_xy(c,x,y);
  put_color_xy(consoleColor,x,y);
}

void put_physical(char c){
  *addr_xy(x,y)=c;
  goto_x(x+1);
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
  colorMemory[currentConsole][y+baseline][x]=color;
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
  return videoMemory[currentConsole][y+baseline][x];
}