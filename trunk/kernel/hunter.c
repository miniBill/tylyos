/* Copyright (C) 2010 Leonardo Taglialegne <leonardotaglialegne+tylyos@gmail.com>
* Copyright (C) 2010 Luca Salmin
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

#include "hunter.h"
#include <lib/string.h>

#define START (char*)60000
#define LIMIT (char*)100000

static int check(char * p){
  if(p[0]=='b' && p[1]=='o' && p[2]=='c' && p[3]=='h' && p[4]=='s')
    return 1;
  return 0;
}

int hunt(void){
  char * p=START;
  while(1 || p<LIMIT)
    if(check(p)){
      printf(0,"trovato a 0x%x [] %s",p,p);
      return 1;
    }
    else
      p++;
  printf(0,"non trovato!\n");
  return 0;
}