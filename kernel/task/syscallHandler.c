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
          unsigned int id;
    switch(runningTask->TSS.eax&0xFF){
        case 1:/*exit*/
          kill(runningTask->procID);
          runningTask=0;
          //printf(0,"KILLED!!!\n");
          forceSchedule();
          break;
        case 2:/*fork*/
          id=fork(runningTask->procID);
          runningTask->TSS.eax=id;
          getTask(id)->TSS.eax=0;
          //printf(0,"FORK!!!\n");
          break;
        case 3:/*exec*/
          exec(runningTask->procID,(char*)user_start+runningTask->TSS.ebx);
          //printf(0,"EXEC!!!\n");
          forceSchedule();
          break;
        case 4:/*move file descriptor*/
          ret=movFileDescriptorTo(runningTask->procID,runningTask->TSS.ebx,runningTask->TSS.ecx);
          //printf(0,"MOV!!!\n");
          runningTask->TSS.eax=(unsigned int)ret;
          break;
        case 5:/*get arg*/
          runningTask->TSS.eax=(unsigned int)runningTask->argc;
          runningTask->TSS.ebx=(unsigned int)runningTask->argv;
          break;
        case 6:/*kill*/
          kill(runningTask->TSS.ebx);
          forceSchedule();
          break;
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
          //printf(1,"aperto file: %d\n",ret);
          break;
        case 253:/*close file*/
          closeFile(runningTask->procID,runningTask->TSS.ebx);
          //printf(1,"chiuso file %d\n",runningTask->TSS.ebx);
          break;
        case 254:/*read file*/
          //printf(1,"%d) read file %d: %d byte \"%s\"\n",runningTask->procID,runningTask->TSS.ebx,runningTask->TSS.edx,user_start+runningTask->TSS.ecx);
          ret = readFile(runningTask->procID,(File)runningTask->TSS.ebx,(char*)user_start+runningTask->TSS.ecx,runningTask->TSS.edx);
          runningTask->TSS.eax=(unsigned int)ret;
          break;
        case 200:/*pipe*/
          desc=(int*)(user_start+runningTask->TSS.ebx);
          pipe(runningTask->procID,desc);
          //printf(0,"pipe %d,%d\n",desc[0],desc[1]);
          break;
        case 201:/*write file*/
          //printf(1,"write file %d: %d byte \"%s\"\n",runningTask->TSS.ebx,runningTask->TSS.edx,user_start+runningTask->TSS.ecx);
          ret = writeFile(runningTask->procID,(File)runningTask->TSS.ebx,(char*)user_start+runningTask->TSS.ecx,runningTask->TSS.edx);
          runningTask->TSS.eax=(unsigned int)ret;
          break;
      }
}
