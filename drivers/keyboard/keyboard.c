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
#include <kernel/stdio.h>
#include <kernel/kernel.h>
#include <drivers/screen/screen.h>
#include <lib/string.h>

unsigned char kmode = 0;

#ifdef KBD_US

char key_map[] = {
       0,   27,  '1',  '2',  '3',  '4',  '5',  '6',
     '7',  '8',  '9',8  '0',  '-',  '=',  127,    9,
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

char altgr_map[] = {
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
       0,    0,  '1',  '2',  '3',  '4',  '5',  '6',
     '7',  '8',  '9',  '0',  '-',  '+',    0,    0,
     'q',  'w',  'e',  'r',  't',  'y',  'u',  'i',
     'o',  'p',    0,  '+', '\n',    0,  'a',  's',
     'd',  'f',  'g',  'h',  'j',  'k',  'l',    0,
       0, '\\',    0,    0,  'z',  'x',  'c',  'v',
     'b',  'n',  'm',  '§',    0,    0,    0,  '*',
       0,    0,    0,    0,    0,    0,    0,    0,
       0,    0,    0,    0,    0,    0,    0,    0,
       0,    0,    0,    0,    0,    0,    0,    0,
       0,    0,    0,    0,    0,    0,    0,    0,
       0,    0,    0,    0,    0,    0,    0,    0,
       0 };

char shift_map[] = {
       0,    0,  '!',    0,    0,  '$',  '%',  '^',
     '&',  '*',  '(',  ')',  '_',  '=',    0,    0,
     'Q',  'W',  'E',  'R',  'T',  'Y',  'U',  'I',
     'O',  'P',    0,  '*',    0,    0,  'A',  'S',
     'D',  'F',  'G',  'H',  'J',  'K',  'L',  '§',
     '"',  '|',  '0',    0,  'Z',  'X',  'C',  'V',
     'B',  'N',  'M',    0,  '>',  '§',    0,    0,
       0,    0,    0,    0,    0,    0,    0,    0,
       0,    0,    0,    0,    0,    0,    0,    0,
       0,    0,    0,    0,    0,    0,    0,    0,
       0,    0,    0,    0,    0,    0,    0,    0,
       0,    0,    0,    0,    0,    0,    0,    0,
       0 };

char altgr_map[] = {
       0,    0,    0,    0,    0,    0,    0,    0,
       0,    0,    0,    0,    0,    0,    0,    0,
       0,    0,    0,    0,    0,    0,    0,    0,
       0,    0,  '[',  ']',   0/*13*/,    0,    0,    0,
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

unsigned escape=0;
unsigned shift=0;
unsigned ctrl=0;
unsigned alt=0;
unsigned clock=0;

void keypress(void){
    unsigned char c=inb(0x60);
    unsigned char ch=0;
    unsigned char released=0;
    if(c==0xE0){
        escape=1;
        return;
    }
    if(c&0x80){
        c&=~0x80;
        released=1;
        ch=0;
    }
    if(c>0x80){
        char test[13]={0};
        test[0]=0;
        write("(!)");
        itoa(c,test);
        put('|');
        write(test);
        put('|');
    }
    if(c==0x36||c==0x2A){
        shift=1-shift;
        putxy(1,ROWS-1,(shift^clock)?'S':'s');
        return;
    }
    if(c==0x1D){
        ctrl=1-ctrl;
        putxy(4,ROWS-1,ctrl?'C':'c');
        return;
    }
    if(c==0x38){
        alt=1-alt;
        putxy(7,ROWS-1,alt?'A':'a');
        return;
    }
    if(c==0x3A){
        if(released)
            return;
        clock=1-clock;
        putxy(1,ROWS-1,(shift^clock)?'S':'s');
        return;
    }
    if(ctrl&&alt){
        ch=altgr_map[c];
        if(c==16){
            writexy(0,0,"Trying halt...");
            halt();
        }
    }
    else{
        if(shift^clock)
            ch=shift_map[c];
        else
            ch=key_map[c];
    }
    if(ch!=0){
        if(released)
            write("(R");
        put(ch);
        if(released)
           put(')');
    }
    else{
        char number[13]={0};
        number[0]=0;
        put('(');
        itobase(c,16,number);
        write(number);
        put(')');
    }
}
