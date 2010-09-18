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

#include "screen.h"
#include "vga.h"
#include <memory/memory.h>
#include <lib/string.h>
#include <gui/gui.h>

static char consoleColor  = White;

static char videoMemory[CONSOLE][ROWS*PAGES][COLUMNS];
static char colorMemory[CONSOLE][ROWS*PAGES][COLUMNS];

//index of first displayed line
static unsigned int baseline[CONSOLE];
static unsigned int currentConsole = 0;

///physical coordinate
static unsigned int x[CONSOLE];
///physical coordinate
static unsigned int y[CONSOLE];

unsigned char vga_mode = 0;

//physical implicit
static inline char * addr_xy(unsigned int x, unsigned int y) {
  return (char*)consoleAddr + 2*(x + 80*y);//COLUMNS hardcoded, because of yes
}

static const int ulx=0;
static const int uly=30;
static const int drx=315;
static const int dry=195;
static const int dx=VGA_dx;//~=220/COLUMNS;
static const int dy=VGA_dy;//~=100/ROWS;

static inline unsigned char convert(unsigned char color){
  switch(color){
    case White:
      return 0xFF;
    case Light_Green:
      return 0x3D;
    case Light_Red:
      return 0xE0;
    case Blue:
      return 0x03;
    default:
      return 0;
  }
}

inline void update(void) {
  for (int ty = 0;ty < ROWS - 1;ty++)
    for (int tx = 0;tx < COLUMNS;tx++)
      if(!vga_mode){
        put_physical_xy(videoMemory[currentConsole][ty+baseline[currentConsole]][tx], tx, ty);
        put_physical_color_xy(colorMemory[currentConsole][ty+baseline[currentConsole]][tx], tx, ty);
      }
      else
        gui_writeChar(videoMemory[currentConsole][ty+baseline[currentConsole]][tx],tx*dx+ulx,ty*dy+uly,vga_mode?
              convert(colorMemory[currentConsole][ty+baseline[currentConsole]][tx])
                     :colorMemory[currentConsole][ty+baseline[currentConsole]][tx]);
  blit();
}

void go_graphic(void){
  vga_mode=1;
  consoleColor=White;
  update();
}

void switch_console(unsigned int console) {
  if (console == currentConsole)
    return;
  if (console >= CONSOLE)
    return;
  currentConsole = console;
  set_cursor(x[console],y[console]);
  put_physical_xy(console+'1', 26, ROWS - 1);
  update();
}

unsigned int current_console(void) {
  return currentConsole;
}

void clear_all(void) {
  for (int c = 0;c < CONSOLE;c++) {
    for (int ty = 0;ty < ROWS*PAGES;ty++)
      for (int tx = 0;tx < COLUMNS;tx++) {
        videoMemory[c][ty][tx] = ' ';
        colorMemory[c][ty][tx] = consoleColor;
      }
      baseline[c] = 0;
      x[c] = 0;
      y[c] = 0;
    }
  clear_physical();
}

void clear(unsigned int console) {
    
        for (int ty = 0;ty < ROWS*PAGES;ty++)
            for (int tx = 0;tx < COLUMNS;tx++) {
                videoMemory[console][ty][tx] = ' ';
                colorMemory[console][ty][tx] = consoleColor;
            }
            baseline[console] = 0;
        x[console] = 0;
        y[console] = 0;
    if(console==currentConsole)
        clear_physical();
}

void clear_physical(void) {
  for (int y = 0;y < 25;y++)
    for (int x = 0;x < 80;x++)
      put_physical_xy(' ', x, y);
}

void set_physical_color(unsigned char color) {
  consoleColor = color;
}

void set_cursor(unsigned int x, unsigned int y) {
  if(vga_mode)
    return;
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

void scroll(unsigned int console, int d) {
  if (((int)baseline[console] + d) < 0)
    baseline[console] = 0;
  else
    baseline[console] += d;
 
  if(console==currentConsole)
    update();
}

void goto_x(unsigned int console, unsigned int nx) {
  if (nx >= COLUMNS)
    goto_xy(console, nx - COLUMNS, row(console) + 1);
  else{
    x[console] = nx;
    if(console==currentConsole)
      set_cursor(nx, y[console]);
  }
}

void goto_y(unsigned int console, unsigned int ny) {
 
    if(baseline[console]+ny>=ROWS*PAGES)
        clear(console);/*HACK*/
    
    if (ny > ROWS - 2)
      scroll(console, ny - ROWS+2);
    else
      y[console] = ny;
  
  if(console==currentConsole)
    set_cursor(x[console], y[console]);
}

void goto_current_xy(unsigned int nx, unsigned int ny) {
  goto_x(currentConsole, nx);
  goto_y(currentConsole, ny);
}

void goto_xy(unsigned int console, unsigned int nx, unsigned int ny) {
  goto_x(console, nx);
  goto_y(console, ny);
}

inline int row(unsigned int console) {
  return y[console] + baseline[console];
}

int col(unsigned int console) {
  return x[console];
}

void nl(unsigned int console) {
  if(console==currentConsole)
    blit();
  goto_xy(console, 0, y[console] + 1);
}

void write(const char* string, unsigned int console)
{
  for (int k = 0; string[k] != 0; k++)
  {
    switch(string[k])
    {
    case '\n':
        nl(console);
        break;
    case '\b':
        goto_x(console, col(console) - 1);
        put_x(' ', console, col(console));
        break;
    default:
        put(string[k], console);
        break;
    }
  }
    if(console==currentConsole)
      blit();
}

void write_current(const char* string){
  write(string,currentConsole);
}

void write_xy(const char* string, unsigned int console, unsigned int px, unsigned int py) {
  unsigned int ox = x[console];
  unsigned int oy = y[console];
  goto_xy(console, px, py);
  write(string, console);
  goto_xy(console, ox, oy);
}

void write_physical_xy(const char* string, unsigned int x, unsigned int y) {
  int k;
  for (k = 0;string[k] != 0;k++) {
    put_physical_xy(string[k], x, y);
    x++;
    if (x == COLUMNS){
      x = 0;
      y++;
    }
  }
}

void put(char c, unsigned int console) {
  put_xy(c, console, x[console], row(console));
  put_color_xy(consoleColor, console, x[console], row(console));
  goto_x(console, x[console] + 1);
}

void put_x(char c, unsigned int console, unsigned int x) {
  videoMemory[console][(baseline[console] + y[console])][x] = c;
  if(console==currentConsole)
    put_physical_xy(c, x, row(console));
  put_color_x(consoleColor, console, x);
}

void put_xy(char c, unsigned int console, unsigned int x, unsigned int y) {
  videoMemory[console][y][x] = c;
  if(console==currentConsole)
    put_physical_xy(c, x, y-baseline[console]);
  put_color_xy(consoleColor, console, x, y);
}

void reboot(void);

void put_physical_xy(char c, unsigned int x, unsigned int y) {
  if(!vga_mode){
    *addr_xy(x, y) = c;
    put_physical_color_xy(consoleColor, x, y);
  }
  else
    gui_writeChar(c,x*dx+ulx,y*dy+uly,convert(consoleColor));
}

void put_color_x(unsigned char color, unsigned int console, unsigned int x) {
  colorMemory[console][y[console] + baseline[console]][x] = color;
  if(console==currentConsole)
    put_physical_color_xy(color, x, y[console]-baseline[console]);
}

void put_color_xy(unsigned char color, unsigned int console, unsigned int x, unsigned int y) {
  colorMemory[console][y][x] = color;
  if(console==currentConsole)
    put_physical_color_xy(color, x, y-baseline[console]);
}

void put_physical_color_xy(unsigned char color, unsigned int x, unsigned int y) {
  if(!vga_mode)
    *(addr_xy(x, y) + 1) = color;
  else
    gui_writeChar(videoMemory[currentConsole][y + baseline[currentConsole]][x],x*dx+ulx,y*dy+uly,vga_mode?convert(color):color);
}

char read_x(unsigned int x, unsigned int console) {
  return videoMemory[console][row(console)][x];
}
