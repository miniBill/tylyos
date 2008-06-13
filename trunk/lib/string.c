/* Copyright (C) 2008 Leonardo Taglialegne <leonardotaglialegne+clearos@gmail.com>
 * Copyright (C) 2008 Roberto Domenella
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

int strapp(char* dest,const char* format,unsigned int num){/*HACK, to have compile-time checks*/
/*int strapp(char* dest,char* format,void* p){*/
    /*indice format string*/
    int i;
    /*indice output*/
    int o;
    o=strlen(dest);
    for(i=0;i<strlen(format);i++){
        if(format[i]=='%'){
            int base=10;
            /*int num=(int)p;*/
            /*char buf[MAXN] = {0};*/
            /*lunghezza del numero*/
            /*int l=0;*/
            /*indice per copiare*/
            /*int b=0;*/
            switch(format[i+1]){
                case '0':
                    dest[o++]=0;
                    break;
                case 'd':
                    break;
                case 'x':
                    base=16;
                    break;
                case 'b':
                    base=2;
                    break;
                case 'o':
                    base=8;
                    break;
                default:
                    /*NON DOVREBBE SUCCEDERE, E SE SUCCEDE LA FORMAT STRING E' SBAGLIATA*/
                    base=10;
                    break;
            }
            if(format[i+1]!='0'){
                itobase(num,base,dest+o);
                while(dest[o]!=0)
                    o++;
                /*for(l=0;num && (l<MAXN) ; num /= base,l++)
                    buf[l] = "0123456789ABCDEF"[num % base];
                for(b=(l-1);b>=0;b--)
                    dest[o++]=buf[b];*/
            }
            i++;
        }
        else
            dest[o++]=format[i];
    }
    dest[o]=0;
    return 0;
}

int strlen(const char * string){
    int ret=0;
    while(string[ret]!=0)
        ret++;
    return ret;
}

void itoa(int a,char buff[12]){
    itobase(a,10,buff);
}

void itobase(int a,int base,char buff[12]){
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
