/* Copyright (C) 2008 Luca Salmin
 * Copyright (C) 2008 Leonardo Taglialegne <leonardotaglialegne+tylyos@gmail.com>
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

#include "pipe.h"
#include <memory/heap.h>

/*alloca una pipe e ne ritorna la struct*/
struct pipe *createPipe()
{
    struct pipe *p;
    p=(struct pipe*)kmalloc(sizeof(struct pipe));
    p->inputPointer=0;
    p->outputPointer=0;
    return p;
}
/*dealloca la struttura*/
void deletePipe(struct pipe *p)
{
    kfree(p);
}

/*ritorna il numero di byte scritti, se e' minore di count significa che la pipe e' piena*/
unsigned int writeOnPipe(struct pipe *p,char *data,unsigned int count)
{
    if(p->inputPointer==p->outputPointer-1 || (p->inputPointer==PIPE_BUFFER_SIZE-1 && p->outputPointer==0))
        return 0;
    else
    {
        unsigned int i=0;
        while(i<count)
        {
            p->buffer[p->inputPointer]=data[i];
            if(p->inputPointer==p->outputPointer-1 || (p->inputPointer==PIPE_BUFFER_SIZE-1 && p->outputPointer==0))
            {
                i++;
                break;
            }

            p->inputPointer++;
            if(p->inputPointer==PIPE_BUFFER_SIZE)
            {
                p->inputPointer=0;
            }
            i++;
        }
        return i;
    }
}

/*ritorna il numero di byte letti, se e' minore di count significa che la pipe e' vuota*/
unsigned int readOnPipe(struct pipe *p,char *data,unsigned int count)
{
    /*se il puntatore output coincide con quello di input non ci sono dati da leggere*/
    if(p->outputPointer==p->inputPointer)
        return 0;
    else
    {
        unsigned int i=0;
        while(i<count)
        {
            data[i]=p->buffer[p->outputPointer];
            p->buffer[p->outputPointer]='A';
            if(i+1<count)
               data[i+1]=0;
            if(p->outputPointer==p->inputPointer)
            {
                break;
            }

            p->outputPointer++;
            if(p->outputPointer==PIPE_BUFFER_SIZE)
            {
                p->outputPointer=0;
            }
            i++;
        }
        //data[i+1]=0;
        return i;
    }
}

