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

#include keymaps.in

unsigned short escape = 0;
unsigned short shift = 0;
unsigned short ctrl = 0;
unsigned short alt = 0;
unsigned short numlock = 0;
unsigned short capslock = 0;

static char * buffer;
static int pointer = 0;

#define KEYBUFSIZE COLUMNS

int modifier(char c, int released) {
  if (c == 0x36 || c == 0x2A) {
      if (released)
        shift = 0;
      else
        shift = 1;
      putxy(1, ROWS - 1, shift ? 'S' : 's');
      return 1;
    }
  if (c == 0x1D) {
      if (released)
        ctrl = 0;
      else
        ctrl = 1;
      putxy(4, ROWS - 1, ctrl ? 'C' : 'c');
      return 1;
    }
  if (c == 0x38) {
      if (released)
        alt = 0;
      else
        alt = 1;
      putxy(7, ROWS - 1, alt ? 'A' : 'a');
      return 1;
    }
  if (c == 0x45) {
      if (!released) {
          numlock = 1 - numlock;
          putxy(10, ROWS - 1, numlock ? 'N' : 'n');
        }
      return 1;
    }
  if (c == 0x3A) {
      if (!released) {
          capslock = 1 - capslock;
          putxy(13, ROWS - 1, capslock ? 'K' : 'k');
        }
      return 1;
    }
  return 0;
}

void backspace(void) {
  gotoi(pos() - 1);
  puti(pos(), ' ');
}

void input(char ch, int released) {
#ifdef FREEROAMING
  if (ch == '\b') {
      backspace();
      return;
    }
#endif
  if (!released) {
      buffer[pointer++] = ch;
      pointer %= KEYBUFSIZE;
    }
  if (ch == '\n') {
#ifdef FREEROAMING
      nl();
#endif
      return;
    }
#ifdef FREEROAMING
  put(ch);
#endif
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
          if (c == 0x13) {
              ch = 0;
              writexy((COLUMNS - 9) / 2, 0, "Rebooting");
              reboot();
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
      else
        printf("(%d)", c);
    }
#ifdef PUT_ON_KEY_RELEASE
  else
    printf("(R%d)", ch);
#endif
#ifndef FREEROAMING
  /*int i;
  for (i = 0;i < COLUMNS;i++)
    putxy(0, i, ' ');*/
  int p=pos();
  gotoxy(0, ROWS - 2);
  printf("%s", buffer);
  gotoi(p);
#endif
}

char getch() {
  pointer--;
  return buffer[pointer];
}

char fetchch() {
  return buffer[pointer-1];
}
