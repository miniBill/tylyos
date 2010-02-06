/* Copyright (C) 2008 Leonardo Taglialegne <leonardotaglialegne+tylyos@gmail.com>
 * Copyright (C) 2008 Roberto Domenella
 * Copyright (C) 2008 Giordano Cristini
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

#include "string.h"
#include <drivers/screen/screen.h>
#include <memory/memory.h>

unsigned int strlen(const char * string) {
  unsigned int ret = 0;
  while (string[ret] != 0)
    ret++;
  return ret;
}

void strcpy(const char *source, char *dest) {
  unsigned int c;
  for (c = 0;c < strlen(source);c++)
    dest[c] = source[c];
  dest[c] = 0;
}

void itoa(int a, char buff[11]) {
  itobase(a, 10, buff);
}

void itobase(int a, unsigned short base, char * buff) {
  if (a == 0) {
      buff[0] = '0';
      buff[1] = 0;
    }
  else {
      int i, l;
      char temp[MAXN] = {0};
      if (a < 0) {
          a = -a;
          buff[0] = '-';
        }
      else
        buff[0] = 0;
      for (l = 0;a && (l < MAXN) ; a /= base, l++)
        temp[l] = "0123456789ABCDEF"[a % base];
      for (i = l - 1, l = buff[0] == '-';i >= 0;i--)
        buff[l++] = temp[i];
      buff[l] = 0;

    }
}

int printf(unsigned int console, const char* format, ...) {
  int size = 0;
  char ** arg = (char**) & format;
  char buf[33];//Longest string will bit a binary int32, so 33 chars are enough
  unsigned int i;
  arg++;/*jump "format"*/
  for (i = 0;i < strlen(format);i++) {
      if (format[i] == '%') {
          switch (format[i+1]) {
            case 'd':
              itoa(* ((int *) arg++), buf);
              goto number;
              break;
            case 'x':
              itobase(* ((int *) arg++), 16, buf);
              goto number;
              break;
            case 'o':
              itobase(* ((int *) arg++), 8, buf);
              goto number;
              break;
            case 'b':
              itobase(* ((int *) arg++), 2, buf);
              goto number;
              break;
            case 'c':
              put(* ((char *) arg++),console);
              size++;
              break;
            case 's':
              write((char*) *arg,console);     /*watch out: Deep Magic*/
              size += strlen((char *) * arg++);
              break;
number:
              write(buf,console);
              size += strlen(buf);
              break;
            default:
              itoa(* ((int *) arg++), buf);
              write("Malformed format string!",console);
              goto number;
              break;
            }

          i++;
        }
      else {
          if (format[i] != '\n')
            put(format[i],console);
          else
            nl(console);
          size++;
        }
    }
  return size;
}

int findchar(const char *str,char c,unsigned int i){
  unsigned int l=strlen(str);
  i++;
  if(l==0)//prevention >> debug
    return -1;
  unsigned int x=0;
  do{
    if(str[x++]==c)
      i--;
  }while(x<l /*don't go over*/ && i!=0 /*returns on the (i+1)th occurence*/);
  if(i!=0)
    return -1;
  return x-1;
}

void substr(char *in,char *out,unsigned int max,unsigned int start,int count){
  unsigned int l=strlen(in);
  if(count<0)
    count=l-start;
  if(start>=l)
    return;//these aren't the droids you're looking for
  if(count>max)//again, prevention >> debug
    count=max;
  unsigned int i;
  for(i=0;count>0;i++,count--)
    out[i]=in[start + i];
}