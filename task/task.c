/* Copyright (C) 2008 Luca Salmin
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

#include "task.h"
#include <memory/memory.h>
#include <lib/string.h>

void initTaskManagement()
{
    taskListRoot=0;

    return;
}

/*setta un selettore di segmento TSS nella GDT*/
/*num: indice nella GDT*/
/*base: base segmento*/
/*access: MEM_TSS_BUSY|MEM_TSS|MEM_KERNEL|MEM_RING1|MEM_RING2|MEM_USER|MEM_PRESENT|MEM_NOT_PRESENT*/
void TSSset(int num, unsigned long base, unsigned char access){
    unsigned long limit=0x67;
    gdt[num].baseLow = (base & 0xFFFF);
    gdt[num].baseMiddle = (base >> 16) & 0xFF;
    gdt[num].baseHigh = (base >> 24) & 0xFF;

    gdt[num].limitLow = (limit & 0xFFFF);
    /*granularity.low is limit.high*/
    gdt[num].granularity = ((limit >> 16) & 0x0F);
    gdt[num].access = access;
}

int addTask(char nome[MAX_TASK_NAME_LEN],char privilegi)
{
    /*alloca le strutture necessarie*/
    struct taskStruct *newTask=kmalloc(sizeof(struct taskStruct));
    struct taskListElement *newListElement=kmalloc(sizeof(struct taskListElement));
    struct taskListElement *pointer;

    newListElement->task=newTask;
    newListElement->next=0;
    

    newTask->listaPagine=0;
    newTask->procID=getNewProcID();
    newTask->privilegi=privilegi;
    strcpy(nome,newTask->nome);

    /*scrive i valori di default nel TSS*/

    newTask->TSS.link=0;
    /*stack pointer*/
    newTask->TSS.esp0=0;/*RICORDARSI DI FARLO AGGIORNARE DAL LOADER*/
    newTask->TSS.esp1=0;/*RICORDARSI DI FARLO AGGIORNARE DAL LOADER*/
    newTask->TSS.esp2=0;/*RICORDARSI DI FARLO AGGIORNARE DAL LOADER*/

    /*aree da settare a 0*/
    newTask->TSS.__ss0h=0;/*sempre a 0*/
    newTask->TSS.__ss1h=0;/*sempre a 0*/
    newTask->TSS.__ss2h=0;/*sempre a 0*/

    /*selettori stack*/ 
    newTask->TSS.ss0=segmentoDatiUser;
    newTask->TSS.ss1=segmentoDatiUser;
    newTask->TSS.ss2=segmentoDatiUser;

    newTask->TSS.cr3=(unsigned int)pageDir;

    /*istruction pointer*/
    newTask->TSS.eip=0;/*RICORDARSI DI FARLO AGGIORNARE DAL LOADER*/

    newTask->TSS.eflags=0;

    newTask->TSS.eax=0;
    newTask->TSS.ecx=0;
    newTask->TSS.edx=0;
    newTask->TSS.ebx=0;

    newTask->TSS.esi=0;
    newTask->TSS.edi=0;

    newTask->TSS.esp=0;/*RICORDARSI DI FARLO AGGIORNARE DAL LOADER*/
    newTask->TSS.ebp=0;/*RICORDARSI DI FARLO AGGIORNARE DAL LOADER*/
  
    /*selettori segmenti*/
    newTask->TSS.es=segmentoDatiUser;
    newTask->TSS.cs=segmentoCodiceUser;
    newTask->TSS.ss=segmentoDatiUser;
    newTask->TSS.ds=segmentoDatiUser;
    newTask->TSS.fs=segmentoDatiUser;
    newTask->TSS.gs=segmentoDatiUser;

    /*selettore LDT*/
    newTask->TSS.ldtr=0;

    /*campi da azzerare*/
    newTask->TSS.__esh=0;
    newTask->TSS.__csh=0;
    newTask->TSS.__ssh=0;
    newTask->TSS.__dsh=0;
    newTask->TSS.__fsh=0;
    newTask->TSS.__gsh=0;
    newTask->TSS.__ldtrh=0;

    newTask->TSS.trace=0;

    /*aggiunge alla lista dei task*/
    pointer=taskListRoot; 
   
    if(pointer!=0) 
    {
        while(pointer->next!=0)
        {
            pointer=pointer->next;
        }

        pointer->next=newListElement;
    }
    else
        taskListRoot=newListElement;


    return newTask->procID;
}

/*rimuove un task dalla lista dei task e dealloca le strutture*/
int removeTask(unsigned int procID)
{
    struct taskListElement *pointer=taskListRoot;
    struct taskListElement *prePointer=0;

    if(pointer!=0)
    {
        do
        {
            if(pointer->task->procID==procID)
            {
                /*trovato*/
                if(prePointer==0)
                {
                    kfree(taskListRoot->task);
                    kfree(taskListRoot);

                    taskListRoot=0;
                }
                else
                {
                    prePointer->next=pointer->next;
                    
                    /*dealloca le strutture*/
                    kfree(pointer->task);
                    kfree(pointer);
                }
                return 1;
            }
            prePointer=pointer;
            pointer=pointer->next;
        }while(pointer!=0);
    }


    return 0;
}

struct taskStruct *getTask(unsigned int procID)
{
   struct taskListElement *pointer=taskListRoot;

    if(pointer!=0)
    {
        do
        {
            if(pointer->task->procID==procID)
            {
                /*trovato*/
                return pointer->task;
            }
            pointer=pointer->next;
       }while(pointer!=0);
    }


    return 0; 
}

/*ritorna un id non utilizzato*/
unsigned int getNewProcID()
{
    struct taskListElement *pointer;
    unsigned int id=0;

    while(1)
    {
        pointer=taskListRoot;

        if(pointer!=0)
        {
            int find=0;
            do
            {
                if(pointer->task->procID==id)
                    find=1;
                pointer=pointer->next;
            }
            while(pointer!=0);

            if(find==0)
                return id;
        }
        else
        {
            return id; 
        }

        id++;
    }
}
