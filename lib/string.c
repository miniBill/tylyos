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
  unsigned int l=strlen(source);
  for (c = 0;c < l;c++)
    dest[c] = source[c];
  dest[c] = 0;
}

void itoa(int a, char buff[11]) {
  itobase(a, 10, buff);
}

void uitobase(unsigned int a, unsigned short base, char * buff) {
  if (a == 0) {
      buff[0] = '0';
      buff[1] = 0;
    }
  else {
      int l;
      char temp[MAXN] = {0};
      buff[0] = 0;
      for(l = 0;a && (l < MAXN) ; a /= base, l++)
        temp[l] = "0123456789ABCDEF"[a % base];
      for(int i = l - 1, l = buff[0] == '-';i >= 0;i--)
        buff[l++] = temp[i];
      buff[l] = 0;
    }
}

void itobase(int a,unsigned short base, char * buff){
  if(a<0){
    a=-a;
    buff[0]='-';
    uitobase(a,base,buff+1);
  }
  else
    uitobase(a,base,buff);
}

unsigned int printf(unsigned int console, const char* format, ...) {
  unsigned  int size = 0;
  char ** arg = (char**) & format;
  char buf[33];//Longest string will bit a binary int32, so 33 chars are enough
  unsigned int i;
  arg++;/*jump "format"*/
  unsigned int fl=strlen(format);
  for (i = 0;i < fl;i++) {
      if (format[i] == '%') {
          switch (format[i+1]) {
            case '%':
              put('%',console);
              size++;
              break;
            case 'd':
              itoa(* ((int *) arg++), buf);
              goto number;
              break;
            case 'u':
              uitobase(* ((int *) arg++), 10, buf);
              goto number;
              break;
            case 'x':
              uitobase(* ((int *) arg++), 16, buf);
              goto number;
              break;
            case 'o':
              uitobase(* ((int *) arg++), 8, buf);
              goto number;
              break;
            case 'b':
              uitobase(* ((int *) arg++), 2, buf);
              goto number;
              break;
            case 'c':
              put(* ((char *) arg++), console);
              size++;
              break;
            case 's':
              write((char*) *arg, console);    /*watch out: Deep Magic*/
              size += strlen((char *) * arg++);
              break;
number:
              write(buf, console);
              size += strlen(buf);
              break;
            default:
              itoa(* ((int *) arg++), buf);
              write("Malformed format string!", console);
              goto number;
              break;
            }

          i++;
        }
      else {
          if (format[i] != '\n')
            put(format[i], console);
          else
            nl(console);
          size++;
        }
    }
  return size;
}

unsigned int sprintf(char * out, unsigned int max, const char * format, ...) {
  unsigned int size = 0;
  char ** arg = (char**) & format;
  char buf[33];//Longest string will bit a binary int32, so 33 chars are enough
  unsigned int i;
  arg++;/*jump "format"*/
  unsigned int fl=strlen(format);
  for (i = 0;size < max && i < fl;i++) {
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
              out[size++] = * ((char *) arg++);
              break;
            case 's':
              if(size+strlen((char *) *arg)<max){
                strcpy((char*) *arg, out + size);
                size += strlen((char *) * arg++);
              }
              else //no more space, return!
                return size;
              break;
number:
              if(size+strlen(buf) < max){
                strcpy(buf, out + size);
                size += strlen(buf);
              }
              else //no more space, return!
                return size;
              break;
            default:
              break;
            }
          i++;
        }
      else
        out[size++] = format[i];
    }
  return size;
}

int findchar(const char *str, char c, unsigned int i) {
  unsigned int l = strlen(str);
  i++;
  if (l == 0)//prevention >> debug
    return -1;
  unsigned int x = 0;
  do {
      if (str[x++] == c)
        i--;
    }
  while (x < l /*don't go over*/ && i != 0 /*returns on the (i+1)th occurence*/);
  if (i != 0)
    return -1;
  return x -1;
}

void substr(char *in, char *out, unsigned int max, unsigned int start, int count) {
  unsigned int l = strlen(in);
  if (count < 0)
    count = l - start;
  if (start >= l) {
      out[0] = 0;
      return;//these aren't the droids you're looking for
    }
  if ((unsigned int)count > max)//again, prevention >> debug
    count = max;
  unsigned int i;
  for (i = 0;count > 0;i++, count--)
    out[i] = in[start + i];
  out[i] = 0;//out could be full of garbage
}

int split(char *in, char *out, unsigned int max, char c, unsigned int i) {
  if (i == 0) {
      int index = findchar(in, c, 0);
      if (index < 0)
        return index;
      substr(in, out, max, 0, index);
      return strlen(out);
    }
  int index = findchar(in, c, i - 1);
  if (index < 0) {
      out[0] = 0;
      return index;
    }
  int medius = findchar(in, c, i);
  if (medius < 0)
    medius++;
  substr(in, out, max, index + 1, medius - index - 1);
  return strlen(out);
}

int strcmp(char *s, char *t) {
  for (; *s == *t; s++, t++)
    if (*s == '\0')
      return 0;
  return *s - *t;
}
