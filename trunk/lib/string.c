/* Copyright (C) 2008 Leonardo Taglialegne <leonardotaglialegne+clearos@gmail.com>
 * Copyright (C) 2008 Roberto Domenella
 * Copyright (C) 2008 Giordano Cristini
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

#include "string.h"
#include <drivers/screen/screen.h>
#include <memory/memory.h>

int strlen(const char * string){
    int ret=0;
    while(string[ret]!=0)
        ret++;
    return ret;
}

void itoa(int a,char buff[11]){
    itobase(a,10,buff);
}

void itobase(int a,int base,char * buff){
    char temp[MAXN]={0};
    if(a==0)
        buff[0]='0';
    else{
        int i,l;
        if(a<0){
            a=-a;
            buff[0]='-';
        }
        for(l=0;a && (l<MAXN) ; a /= base,l++)
            temp[l] = "0123456789ABCDEF"[a % base];
        for(i=(l-1),l=0;i>=0;i--)
            buff[l++]=temp[i];
        buff[l]=0;
    }
}

int printf(const char* format, int val){
    char buf[11];
    /*un numero può avere fino a 10 cifre, e bisogna contare pure lo \0*/
    int size=0;
    int i;
    for(i=0;i<strlen(format);i++){
        if(format[i]=='%'){
            switch(format[i+1]){
                case 'd':
                    itoa(val,buf);
                    write(buf);
                    size+=strlen(buf);
                    break;
                case 'x':
                    itobase(val,16,buf);
                    write(buf);
                    size+=strlen(buf);
                    break;
            }
            i++;
        }
        else{
            if(format[i]!='\n')
                put(format[i]);
            else
                nl();
            size++;
        }
    }
    return size;
}
