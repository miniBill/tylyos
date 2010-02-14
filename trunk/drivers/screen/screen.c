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
#include <memory/memory.h>
#include <lib/string.h>
#include <gui/gui.h>

static char consoleColor  = White;

static char videoMemory[CONSOLE][ROWS*PAGES][COLUMNS];
static char colorMemory[CONSOLE][ROWS*PAGES][COLUMNS];

//index of first displayed line
static unsigned int baseline[CONSOLE];
static unsigned int currentConsole = 0;

static unsigned int x[CONSOLE];
static unsigned int y[CONSOLE];

static unsigned char vga_mode = 0;

//physical implicit
static inline char * addr_xy(unsigned int x, unsigned int y) {
  return (char*)(consoleAddr + 2*(x + COLUMNS*y));
}

static const int ulx=5;
static const int uly=5;
static const int drx=315;
static const int dry=195;
static const int dx=5;//220/COLUMNS;
static const int dy=5;//100/ROWS;

static inline void update(void) {
  for (int ty = 0;ty < ROWS - 1;ty++)
    for (int tx = 0;tx < COLUMNS;tx++)
      if(!vga_mode){
        put_physical_xy(videoMemory[currentConsole][ty+baseline[currentConsole]][tx], tx, ty);
        put_physical_color_xy(colorMemory[currentConsole][ty+baseline[currentConsole]][tx], tx, ty);
      }
      else
        VGA_writeChar(videoMemory[currentConsole][ty+baseline[currentConsole]][tx],tx*dx+ulx,ty*dy+uly,
                      colorMemory[currentConsole][ty+baseline[currentConsole]][tx]);
}

void go_graphic(void){
  vga_mode=1;
  update();
}

void switch_console(unsigned int console) {
  if (console == currentConsole)
    return;
  if (console >= CONSOLE)
    return;
  currentConsole = console;
  update();
  set_cursor(x[console],y[console]);
  if(!vga_mode)
    put_physical_xy(console+'1', 26, ROWS - 1);
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

void clear_physical(void) {
  for (int y = 0;y < ROWS;y++)
    for (int x = 0;x < COLUMNS;x++)
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
    goto_xy(console, nx - COLUMNS, baseline[console] + y[console] + 1);
  else {
      x[console] = nx;
      if(console==currentConsole)
        set_cursor(nx, y[console]);
    }
}

void goto_y(unsigned int console, unsigned int ny) {
  if (ny < baseline[console])
    scroll(console, ny - baseline[console]);
  else if (ny > baseline[console] + ROWS - 2)
    scroll(console, ny - baseline[console] - ROWS + 2);
  y[console] = ny - baseline[console];
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

int row(unsigned int console) {
  return y[console] + baseline[console];
}

int col(unsigned int console) {
  return x[console];
}

void nl(unsigned int console) {
  goto_xy(console, 0, y[console] + 1);
}

void write(const char* string, unsigned int console) {
  for (int k = 0; string[k] != 0; k++)
    if (string[k] != '\n')
      put(string[k], console);
    else
      nl(console);
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
      if (x == COLUMNS) {
          x = 0;
          y++;
        }
    }
}

void put(char c, unsigned int console) {
  put_xy(c, console, x[console], baseline[console] + y[console]);
  put_color_xy(consoleColor, console, x[console], baseline[console] + y[console]);
  goto_x(console, x[console] + 1);
}

void put_x(char c, unsigned int console, unsigned int x) {
  videoMemory[console][baseline[console] + y[console]][x] = c;
  if(console==currentConsole)
    put_physical_xy(c, x, y[console] + baseline[console]);
  put_color_x(consoleColor, console, x);
}

void put_xy(char c, unsigned int console, unsigned int x, unsigned int y) {
  videoMemory[console][y][x] = c;
  if(console==currentConsole)
    put_physical_xy(c, x, y);
  put_color_xy(consoleColor, console, x, y);
}

void reboot(void);

void put_physical_xy(char c, unsigned int x, unsigned int y) {
  if(!vga_mode){
    *addr_xy(x, y) = c;
    put_physical_color_xy(consoleColor, x, y);
  }
  else
    VGA_writeChar(c,x*dx+ulx,y*dy+uly,consoleColor);
}

void put_color_x(unsigned char color, unsigned int console, unsigned int x) {
  colorMemory[console][y[console] + baseline[console]][x] = color;
  if(console==currentConsole)
    put_physical_color_xy(color, x, y[console]);
}

void put_color_xy(unsigned char color, unsigned int console, unsigned int x, unsigned int y) {
  colorMemory[console][y][x] = color;
  if(console==currentConsole)
    put_physical_color_xy(color, x, y);
}

void put_physical_color_xy(unsigned char color, unsigned int x, unsigned int y) {
  if(!vga_mode)
    *(addr_xy(x, y) + 1) = color;
  else
    VGA_writeChar(videoMemory[currentConsole][y + baseline[currentConsole]][x],x*dx+ulx,y*dy+uly,color);
}

char read_x(unsigned int x, unsigned int console) {
  return videoMemory[console][y[console] + baseline[console]][x];
}
