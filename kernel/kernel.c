/* Copyright (C) 2008 Leonardo Taglialegne <leonardotaglialegne+clearos@gmail.com>
 * Copyright (C) 2008 Luca Salmin
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

#include "kernel.h"
#include "tests.h"
#include "multiboot.h"
#include <lib/string.h>
#include <memory/memory.h>
#include <task/task.h>
#include <interrupt/interrupt.h>
#include <drivers/timer/timer.h>
#include <fs/fs.h>

#include <gui/gui.h>

static int on = 1;

static int magicNumber = 0;

static multiboot_info_t * multiBootInfo;

inline void greendot(void) {
  write(" * ");
  put_color_xy(Light_Green,1,row());
}

void reboot(void) {
  on = 0;
}

void OK(int i) {
  write_xy("[ ok ]",COLUMNS - 6, i);
  put_color_xy(Blue,COLUMNS - 6, i);
  put_color_xy(Light_Green,COLUMNS - 4, i);
  put_color_xy(Light_Green,COLUMNS - 3, i);
  put_color_xy(Blue,COLUMNS - 1, i);
}

void NO(int i) {
  write_xy("[ NO ]",COLUMNS - 6, i);
  put_color_xy(Blue,COLUMNS - 6, i);
  put_color_xy(Light_Red,COLUMNS - 4, i);
  put_color_xy(Light_Red,COLUMNS - 3, i);
  put_color_xy(Blue,COLUMNS - 1, i);
}

void kwrite(const char * string) {
  static char * kpointer = (char *) 0xb8000;
  int k;
  for (k = 0;
       string[k] != 0 && k < (COLUMNS*ROWS);
       k++) {
      * (kpointer++) = string[k];
      * (kpointer++) = 7;
    }
}

void logo(void) {
  write("                       ________                ____\n");
  write("                      / ____/ /__  ____  _____/ __ \\____\n");
  write("                     / /   / / _ \\/ __ \\/ ___/ / / /  _/\n");
  write("                    / /___/ /  __/ /_/ / /  / /_/ /\\  \\\n");
  write("                    \\____/_/\\___/\\__._/_/   \\____//___/\n");

}

int check(const char * output, int offset) {
  unsigned int retval = 1, i;
  for (i = 0;i < strlen(output);i++)
    if (read_x(i + 3 + offset) != output[i]) {
        retval = 0;
        put_color_x(i + 3 + offset, Light_Red);
      }
  return retval;
}

int magictest(void) {
  printf("Test magic number: %x", magicNumber);
  return magicNumber == 0x2BADB002;
}

int mbdtest(void) {
  int l = multiBootInfo->mem_lower;
  int u = multiBootInfo->mem_upper;
  if (multiBootInfo->flags & 1){
      printf("Lower memory:%dKb. ",l);
      printf("Upper memory:%dMb. ",u/1024);
      printf("Total memory:%dMb and %dKb.",(l + u) / 1024,(l + u) % 1024);
  }
  return l > 0;
}

void magic(void) {
  int t = row() + 1;
  NO(t);
  greendot();
  if (magictest())
    OK(t++);
  else
    t++;
  printf("\n");
  NO(t);
  greendot();
  if (mbdtest())
    OK(t++);
  else
    t++;
  printf("\n");
}

void _kmain(multiboot_info_t* mbd, unsigned int magicN) {
  int t = 0;/*test number*/


  magicNumber = magicN;
  multiBootInfo = mbd;

  memoriaFisica = multiBootInfo->mem_lower + multiBootInfo->mem_upper;
  memoriaFisica *= 1024;

  clear_physical();

  NO(t);
  kwrite("Kernel caricato.");
  OK(t++);
  write("\n");

  NO(t);
  write("Prova writeline.\n");
  OK(t++);

  logo();
  t += 4;

  NO(t);
  greendot();
  write("Inizializzazione:");

  write(" GDT");
  initGdt();

  write(" IDT");
  initIdt();

  write(" PIT");
  initTimer();

  write(" Paging\n");
  initPaging();
  OK(t++);

  /*here start the true tests*/

  doTests(); //from now on printf is ok

  magic();

  t = row() + 1;

  NO(t);
  greendot();
  printf("Inizializzazione gestore devices\n");
  initDeviceFsManager();
  OK(t++);

  NO(t);
  greendot();
  printf("Kernel pronto!!!\n");
  OK(t++);

  /*drawRectangle(0,t,COLUMNS-1,ROWS-t-2,(char)(Yellow|Back_Blue));*/
  //gotoxy(1, t);
  asm("sti");
  write_physical_xy("[s][c][a][n][k] Time:",0, ROWS - 1);
  on = 1;

  /* test kernel panic
   while(1)
   {
      kmalloc(1024*1024);
   }*/
  /*kernelPanic("_kmain()","this is a test message");*/



  while (on);
  {
    /*wait 3 seconds before halting*/
    sleep(3000);

    asm("cli");
    clearIdt();
    asm("int $1");
  }
}


void kernelPanic(char *sender, char *message) {

  asm("cli");
  set_physical_color(Yellow | Back_Red);
  clear_physical();
  write_physical_xy("RED SCREEN OF DOOM",0,3);
  goto_physical_xy(0, 7);
  write_physical("Dear user,\n");
  write_physical("I am very sorry I haven't written for so long. ");
  write_physical("I am writing to tell you that \n");
  write_physical(message);
  write_physical("\nMuch love, ");
  write_physical(sender);
  set_cursor(100, 100);
  while (1);
}
