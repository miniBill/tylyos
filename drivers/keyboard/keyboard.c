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
#include <drivers/screen/screen.h>
#include <kernel/stdio.h>

unsigned char kmode   = 0;



#ifdef KBD_US

char key_map[] = {
       0,   27,  '1',  '2',  '3',  '4',  '5',  '6',
     '7',  '8',  '9',  '0',  '-',  '=',  127,    9,
     'q',  'w',  'e',  'r',  't',  'y',  'u',  'i',
     'o',  'p',  '[',  ']', '\n',    0,  'a',  's',
     'd',  'f',  'g',  'h',  'j',  'k',  'l',  ';',
    '\'',  '`',    0, '\\',  'z',  'x',  'c',  'v',
     'b',  'n',  'm',  ',',  '.',  '/',    0,  '*',
       0,   32,    0,    0,    0,    0,    0,    0,
       0,    0,    0,    0,    0,    0,    0,    0,
       0,    0,  '-',    0,    0,    0,  '+',    0,
       0,    0,    0,    0,    0,    0,  '<',    0,
       0,    0,    0,    0,    0,    0,    0,    0,
       0 };

char shift_map[] = {
       0,   27,  '!',  '@',  '#',  '$',  '%',  '^',
     '&',  '*',  '(',  ')',  '_',  '+',  127,    9,
     'Q',  'W',  'E',  'R',  'T',  'Y',  'U',  'I',
     'O',  'P',  '{',  '}',   13,    0,  'A',  'S',
     'D',  'F',  'G',  'H',  'J',  'K',  'L',  ':',
     '"',  '~',  '0',  '|',  'Z',  'X',  'C',  'V',
     'B',  'N',  'M',  '<',  '>',  '?',    0,  '*',
       0,   32,    0,    0,    0,    0,    0,    0,
       0,    0,    0,    0,    0,    0,    0,    0,
       0,    0,  '-',    0,    0,    0,  '+',    0,
       0,    0,    0,    0,    0,    0,  '>',    0,
       0,    0,    0,    0,    0,    0,    0,    0,
       0 };

char alt_map[] = {
       0,    0,    0,  '@',    0,  '$',    0,    0,
     '{',   '[',  ']', '}', '\\',    0,    0,    0,
       0,    0,    0,    0,    0,    0,    0,    0,
       0,    0,    0,  '~',   13,    0,    0,    0,
       0,    0,    0,    0,    0,    0,    0,    0,
       0,    0,    0,    0,    0,    0,    0,    0,
       0,    0,    0,    0,    0,    0,    0,    0,
       0,    0,    0,    0,    0,    0,    0,    0,
       0,    0,    0,    0,    0,    0,    0,    0,
       0,    0,    0,    0,    0,    0,    0,    0,
       0,    0,    0,    0,    0,    0,  '|',    0,
       0,    0,    0,    0,    0,    0,    0,    0,
       0 };

/*
static char num_map[] = "789-456+1230,";
static int  pad_map[] = { 7, 8, 9, 0, 4, 5, 6, 0, 1, 2, 3, 0, 0 };
*/

#else
 #ifdef KBD_IT

char key_map[] = {
       0,   27,  '1',  '2',  '3',  '4',  '5',  '6',
     '7',  '8',  '9',  '0', '\'',    0,  127,    9,
     'q',  'w',  'e',  'r',  't',  'y',  'u',  'i',
     'o',  'p',    0,  '+', '\n',    0,  'a',  's',
     'd',  'f',  'g',  'h',  'j',  'k',  'l',    0,
       0, '\\',    0,    0,  'z',  'x',  'c',  'v',
     'b',  'n',  'm',  ',',  '.',  '-',    0,  '*',
       0,   32,    0,    0,    0,    0,    0,    0,
       0,    0,    0,    0,    0,    0,    0,    0,
       0,    0,  '-',    0,    0,    0,  '+',    0,
       0,    0,    0,    0,    0,    0,  '<',    0,
       0,    0,    0,    0,    0,    0,    0,    0,
       0 };

char shift_map[] = {
       0,   27,  '!',  '"',    0,  '$',  '%',  '&',
     '/',  '(',  ')',  '=',  '?',  '^',  127,    9,
     'Q',  'W',  'E',  'R',  'T',  'Y',  'U',  'I',
     'O',  'P',    0,  '*',   13,    0,  'A',  'S',
     'D',  'F',  'G',  'H',  'J',  'K',  'L',    0,
       0,  '|',  '0',    0,  'Z',  'X',  'C',  'V',
     'B',  'N',  'M',  ';',  ':',  '_',    0,    0,
       0,   32,    0,    0,    0,    0,    0,    0,
       0,    0,    0,    0,    0,    0,    0,    0,
       0,    0,    0,    0,    0,    0,    0,    0,
       0,    0,    0,    0,    0,    0,  '>',    0,
       0,    0,    0,    0,    0,    0,    0,    0,
       0 };

char alt_map[] = {
       0,    0,    0,    0,    0,    0,    0,    0,
       0,    0,    0,    0,    0,    0,    0,    0,
       0,    0,    0,    0,    0,    0,    0,    0,
       0,    0,  '[',  ']',   13,    0,    0,    0,
       0,    0,    0,    0,    0,    0,    0,  '@',
     '#',    0,    0,    0,    0,    0,    0,    0,
       0,    0,    0,    0,    0,    0,    0,    0,
       0,    0,    0,    0,    0,    0,    0,    0,
       0,    0,    0,    0,    0,    0,    0,    0,
       0,    0,    0,    0,    0,    0,    0,    0,
       0,    0,    0,    0,    0,    0,    0,    0,
       0,    0,    0,    0,    0,    0,    0,    0,
       0 };

/*
static char num_map[] = "789-456+1230,";
static int  pad_map[] = { 7, 8, 9, 0, 4, 5, 6, 0, 1, 2, 3, 0, 0 };
*/

 #else
  #error No keyboard defined
 #endif
#endif

void keypress(void){
    char c=inb(0x60);
    c=scanCodeToChar(c);
    if(c!=0)
        put(c);
}

char scanCodeToChar(char scode){
    char ch;
    scode=scode&0x7F;
    if(kmode & ALTGR)
        ch=alt_map[(int)scode];
    else
        if(kmode & (LSHIFT|RSHIFT|LCTRL|RCTRL))
            ch=shift_map[(int)scode];
        else
            ch=key_map[(int)scode];

    if (ch == 0)
        return '\0';

    /* If CTRL is active the character CTRL-A == 0x01, CTRL-B == 0x02,
     * CTRL-Z == 0x1A. */
    if(kmode & (LCTRL|RCTRL|CAPS))
        if(
         (
          (ch>='a')
          &&
          (ch <='z')
         )
         /*||
         (
          (ch>=224)
          &&
          (ch<=254)
         )*/
        )
            ch -= 32;

    if (kmode & (LCTRL|RCTRL))
        ch &= 0x1f;

    /* If the character has been pressed in combination with
     * ALT key, the bit 7 is activated. For LATIN-1 map the character is
     * prepended with 0x33 value (now not handled).*/
    if (kmode & ALT)
        ch |= 0x80;
    return ch;
}
