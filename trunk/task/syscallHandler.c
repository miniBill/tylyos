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
#include <interrupt/interrupt.h>
#include <task/scheduler.h>
#include <task/dispatcher.h>
#include <memory/memory.h>
#include <lib/string.h>
#include <fs/fs.h>

void handleSyscall()
{
          File ret;
          int *desc;
    switch(runningTask->TSS.eax&0xFF){
        case 88:
          asm("cli");
          clearIdt();
          asm("int $1");
          break;
        case 250:
          printf(0,(char*)user_start+runningTask->TSS.ebx);
          break;
        case 251:
        //  printf(1,"sleep %s %dms\n",runningTask->nome,runningTask->TSS.ebx);
          setTaskStateSleeping(runningTask->procID,runningTask->TSS.ebx);
          sendPicAck();
          forceSchedule();
          break;
        case 252:/*open file*/
          ret = openFile(runningTask->procID,(char*)user_start+runningTask->TSS.ebx,(char)user_start+runningTask->TSS.ecx);
          runningTask->TSS.eax=(unsigned int)ret;
          printf(1,"aperto file: %d\n",ret);
          break;
        case 253:/*close file*/
          closeFile(runningTask->procID,runningTask->TSS.ebx);
          printf(1,"chiuso file %d\n",runningTask->TSS.ebx);
          break;
        case 254:/*read file*/
          printf(1,"read file %d: %d byte\n",runningTask->TSS.ebx,runningTask->TSS.edx);
          ret = readFile(runningTask->procID,(File)runningTask->TSS.ebx,(char*)user_start+runningTask->TSS.ecx,runningTask->TSS.edx);
          runningTask->TSS.eax=(unsigned int)ret;
          break;
        case 200:/*pipe*/
          desc=(int*)(user_start+runningTask->TSS.ebx);
          pipe(runningTask->procID,desc);
          break;
      }
}
