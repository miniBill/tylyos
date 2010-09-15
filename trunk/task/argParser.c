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

void parseAndLoadArgs(unsigned int procID,char *string)
{
    struct taskStruct *t;
    t=getTask(procID);
    if(t==0)
        return;
    t->argc=1;
    t->argv=t->dataSegmentBase+t->dataSegmentSize;
    unsigned int tmp=t->dataSegmentBase+t->dataSegmentSize+4;
 allocMemory(procID,t->argv+user_start,5+4+1000);
     memcpyToTask( &tmp, 4,user_start+t->dataSegmentBase+t->dataSegmentSize,procID );
     memcpyToTask( "test\0", 5,user_start+t->dataSegmentBase+t->dataSegmentSize+4,procID );
    t->dataSegmentSize+=4+5;
}
