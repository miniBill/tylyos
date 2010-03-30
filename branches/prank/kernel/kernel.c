/* Copyright (C) 2008 Leonardo Taglialegne <leonardotaglialegne+tylyos@gmail.com>
 * Copyright (C) 2008 Luca Salmin
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

#include "kernel.h"
//#include <lib/string.h>
#include <memory/memory.h>
#include <drivers/keyboard/keyboard.h>
#include <interrupt/interrupt.h>
#include <drivers/timer/timer.h>
#include <drivers/sound/sound.h>
#include <drivers/screen/vga.h>

#include <gui/gui.h>

unsigned char flash;

void spinWait(unsigned long long t){
  unsigned long long a=0;
  for(unsigned long long wait=0;wait<t;wait++)
    a+=wait;
}

int fishing;

void _kmain(multiboot_info_t* mbd, unsigned int magicN) {
  set_physical_color(White|Back_Black);

  VGA_init(320,200,8);

  initGdt();
  initIdt();
  initTimer();

  asm("sti");
  
  gui_background=0;
  void VGA_clear_screen(void);
  VGA_clear_screen();

  flash=0;

  while(!flash);
  
  asm("cli");

  goto_current_xy(0,3);
  write_current("BLACK SCREEN OF DOOM");
  goto_current_xy(0, 7);
  write_current("Dear user,\n");
  write_current("I am very sorry I haven't written for so long.\n");
  write_current("I am writing to tell you that an invalid operation has happened at unknown address! PEBKAC!!!\n");
  write_current("Much love, your computer.\n");
  set_cursor(100, 100);
  spinWait(20000000);
  write_current("AYEEEEEEE! EXPLODING!!!");
  spinWait(60000000);
  sound(1000);
  int coll=0;
  while(1){
    coll++;
    spinWait(100000);
    gui_background=coll%255;
    void VGA_clear_screen(void);
    VGA_clear_screen();
    if(coll>400)
      fishing=1;
  }
}