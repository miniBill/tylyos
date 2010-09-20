/* Copyright (C) 2008 Luca Salmin
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

#include <task/task.h>
#include <memory/memory.h>
#include <task/argParser.h>
#include <lib/string.h>

void parseAndLoadArgs(unsigned int procID,char *string)
{
    struct taskStruct *t;
    t=getTask(procID);
    if(t==0)
        return;
    t->argv=(char **)(t->dataSegmentBase+t->dataSegmentSize);
/*
    unsigned int tmp=t->dataSegmentBase+t->dataSegmentSize+4;
 allocMemory(procID,t->argv+user_start,5+4+1000);
     memcpyToTask( &tmp, 4,user_start+t->dataSegmentBase+t->dataSegmentSize,procID );
     memcpyToTask( "test\0", 5,user_start+t->dataSegmentBase+t->dataSegmentSize+4,procID );
    t->dataSegmentSize+=4+5;
*/
    int n=parseArgsNum(string);
    t->argc=n;
    unsigned int *array=kmalloc(4*n);
    char *buffer=kmalloc(strlen(string)+1);
    unsigned int c,i=0,d=0;
    char apice=0;
    for(c=0;c<strlen(string);c++)
    {
        if(string[c]==' ')
            break;
    }
    if(c==strlen(string) || c+1==strlen(string))
    {
        t->argc=0;
        kfree(array);
        kfree(buffer);
        return;
    }
    for(;c<strlen(string);c++)
    {
        if(string[c]=='\'')
        {
            apice=!apice;
            continue;
        }
        if(string[c]==' ' && !apice)
        {
            array[i]=t->dataSegmentBase+t->dataSegmentSize+(4*n)+d+1;
            i++;
        }
        else
        {
            buffer[d]=string[c];
            buffer[d+1]=0;
        }
        d++;
    }

 allocMemory(procID,user_start+t->dataSegmentBase+t->dataSegmentSize,(4*n)+d+1);
    memcpyToTask( (char*)array, 4*n,(char*)(user_start+t->dataSegmentBase+t->dataSegmentSize),procID );
    memcpyToTask( buffer, d+1,(char*)(user_start+t->dataSegmentBase+t->dataSegmentSize+(4*n)),procID );
    t->dataSegmentSize+=(4*n)+d+1;

    kfree(array);
    kfree(buffer);
}

unsigned int parseArgsNum(char *string)
{
    unsigned int n=0;
    char apice=0;
    for(unsigned int c=0;c<strlen(string);c++)
    {
        if(string[c]=='\'')
            apice=!apice;
        if(string[c]==' ' && !apice)
            n++;
    }

    return n;
}
