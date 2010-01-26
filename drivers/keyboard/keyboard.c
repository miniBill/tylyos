/* Copyright (C) 2008 Luca Salmin
 * Copyright (C) 2008 Leonardo Taglialegne <leonardotaglialegne+clearos@gmail.com>
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

#include "keyboard.h"
#include <memory/memory.h>
#include <kernel/stdio.h>
#include <kernel/kernel.h>
#include <drivers/screen/screen.h>
#include <lib/string.h>

unsigned char kmode = 0;

#include "keymaps.in"

unsigned short escape = 0;
unsigned short shift = 0;
unsigned short ctrl = 0;
unsigned short alt = 0;
unsigned short numlock = 0;
unsigned short capslock = 0;

static char * buffer;
static int pointer = 0;

#define KEYBUFSIZE COLUMNS

//#define FREEROAMING

static int stop_output=0;

int modifier(char c, int released) {
  if (c == 0x36 || c == 0x2A) {
      if (released)
        shift = 0;
      else
        shift = 1;
      if(!stop_output)
	put_physical_xy(1, ROWS - 1, shift ? 'S' : 's');
      return 1;
    }
  if (c == 0x1D) {
      if (released)
        ctrl = 0;
      else
        ctrl = 1;
      if(!stop_output)
        put_physical_xy(4, ROWS - 1, ctrl ? 'C' : 'c');
      return 1;
    }
  if (c == 0x38) {
      if (released)
        alt = 0;
      else
        alt = 1;
      if(!stop_output)
       put_physical_xy(7, ROWS - 1, alt ? 'A' : 'a');
      return 1;
    }
  if (c == 0x45) {
      if (!released) {
          numlock = 1 - numlock;
	  if(!stop_output)
           put_physical_xy(10, ROWS - 1, numlock ? 'N' : 'n');
        }
      return 1;
    }
  if (c == 0x3A) {
      if (!released) {
          capslock = 1 - capslock;
	  if(!stop_output)
            put_physical_xy(13, ROWS - 1, capslock ? 'K' : 'k');
        }
      return 1;
    }
  return 0;
}

/*void backspace(void) {
  gotoi(pos() - 1);
  puti(pos(), ' ');
}*/

void input(char ch, int released) {
#ifdef FREEROAMING
  if (ch == '\b') {
      backspace();
      return;
    }
  if (ch == '\n') {
      nl();
      return;
    }
#endif

  if (!released) {
    #ifndef FREEROAMING
       if(pointer==KEYBUFSIZE)
	return;
    #endif
      buffer[pointer++] = ch;
      #ifdef FREEROAMING
      pointer %= KEYBUFSIZE;
      #endif
    }
#ifdef FREEROAMING
  put(ch);
#endif
}

#define inc() x++;if(x==COLUMNS){x=0;y++;}
#define slashwrite(a) write_xy("\\",x,y);inc();write_xy(a,x,y);inc();

//Write a string, with escaped characters
static void Swrite_xy(const char* string,unsigned int x, unsigned int y) {
  int k;
  if(stop_output)
    return;
  for (k = 0;string[k] != 0;k++) {
    switch (string[k]) {
      case '\n':
        slashwrite("n");
        break;
      case '\b':
        slashwrite("b");
        break;
      case '\\':
        slashwrite("\\");
        break;
      default:
        put_xy(string[k],x,y);
        inc();
    }
  }
}

void keypress(void) {
  unsigned char c = inb(0x60);
  unsigned char ch = 0;
  unsigned char released = 0;
  static int init = 0;
  if (!init) {
      buffer = (char *) kmalloc(KEYBUFSIZE * sizeof(buffer) + 1);/*always reserve a byte for \0 */
      for (init = 0;init <= KEYBUFSIZE;init++)
        buffer[init] = 0;
    }
  /*identifica l'inizio di uno scancode a due byte*/
  if (c == 0xE0) {
      escape = 1;
      return;
    }
  /*se e' un evento: tasto rilasciato*/
  if (c&0x80) {
      c &= ~0x80;
      released = 1;
      ch = 0;
    }
  if (c > 0x80)
    printf("(!)|%d|", c);   /*should NEVER happen*/
  /*se e' un tasto modificatore es: shitf*/
  if (modifier(c, released)) {
      escape = 0;/*HACK: in modo da gestire ugualemente sia alt destro che sinistro*/
      return;
    }
  /*
    se e' stato notificato una scancode a due byte precedentemente
    viene letto il secondo scancode ed interpretato
  */
  if (escape) {
      escape = 0;
      if (released)
        return;
      ch = key_map[c];
      switch (ch) {
        case '7':
#ifdef FREEROAMING
          gotoxy(0, row());
#endif
          return;
        case '8': /* freccia su */
#ifdef FREEROAMING
          gotoi(pos() - COLUMNS);
#endif
          return;
        case '9': /* pag su */
#ifdef FREEROAMING
          if (alt)
            scroll(-20);
#endif
          return;
        case '4': /* freccia sx */
#ifdef FREEROAMING
          gotoi(pos() - 1);
#endif
          return;
        case '5':
          return;
        case '6': /* freccia dx */
#ifdef FREEROAMING
          gotoi(pos() + 1);
#endif
          return;
        case '1':
#ifdef FREEROAMING
          gotoxy(COLUMNS - 1, row());
#endif
          return;
        case '2': /* freccia giu' */
#ifdef FREEROAMING
          gotoi(pos() + COLUMNS);
#endif
          return;
        case '3': /* pag giu' */
#ifdef FREEROAMING
          if (alt)
            scroll(20);
#endif
          return;
        case '0':
          /*TODO*/
          return;
        case '.': /* canc */
#ifdef FREEROAMING
          put(' ');
          gotoi(pos() - 1);
#endif
          return;
        }
    }
  else {
      if (ctrl && alt) {
          ch = altgr_map[c];
          switch (c) {
            case 0x13: /* R for restart */
              ch = 0;
              write_physical_xy("Rebooting", (COLUMNS - 9) / 2, 0);
              reboot();
              break;
            case 0x23: /* H for halt */
              ch = 0;
              write_physical_xy("Halting", (COLUMNS - 7) / 2, 0);
              reboot();//TODO: implement halt
              break;
            case 0x20:/*D for doom */
              ch = 0;
              write_physical_xy("Dooming", (COLUMNS - 7) / 2, 0);
              kernelPanic("your system","an invalid operation has happened at unknown address!!! PEBKAC!!!");
              break;
	    case 0x38:/*L for life*/
	      ch = 0;
	      write_physical_xy("r for \"randomize\", p for pause",0,ROWS-1);
	      stop_output=1;
	      do_life();
	      break;
            }
        }
      else {
          if (alt) {
              ch = altgr_map[c];
            }
          else {
              /* se e' premuto shift */
              if (shift) {
                  ch = shift_map[c];
                  if (capslock && ch >= 'A' && ch <= 'Z')
                    ch -= 'A' -'a';
                }
              else {
                  /*se non e' attivo nessun modificatore*/
                  ch = key_map[c];
                  if (capslock && ch >= 'a' && ch <= 'z')
                    ch += 'A' -'a';
                }
            }
        }
    }
  if (!released) {
      if (ch != 0)
        input(ch, released);
#ifdef FREEROAMING
      else {
          if (escape)
            printf("(E%d)", c);
          else
            printf("(%d)", c);
        }
#endif
    }
#ifdef PUT_ON_KEY_RELEASE
  else
    printf("(R%d)", ch);
#endif
#ifndef FREEROAMING
  /*int i;
  for (i = 0;i < COLUMNS;i++)
    putxy(0, i, ' ');*/
  Swrite_xy(buffer,0,ROWS-3);
#endif
}

char getch() {
  if(pointer==0)
    return '\0';
  pointer--;
  char toret=buffer[pointer];
  buffer[pointer]=0;
  return toret;
}

char fetchch() {
  return buffer[pointer-1];
}
