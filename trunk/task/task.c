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


#include "task.h"
#include <memory/memory.h>
#include <memory/gdtflags.h>
#include <lib/string.h>
#include <kernel/kernel.h>
#include <task/elf.h>

void initTaskManagement()
{
    taskListRoot=0;
    
    currentTSS=segmentSelector ( 5,0,RPL_KERNEL );
    newTSS=segmentSelector ( 6,0,RPL_KERNEL );

    return;
}

/*setta un selettore di segmento TSS nella GDT*/
/*num: indice nella GDT*/
/*base: base segmento*/
/*access: MEM_TSS_BUSY|MEM_TSS|MEM_KERNEL|MEM_RING1|MEM_RING2|MEM_USER|MEM_PRESENT|MEM_NOT_PRESENT*/
void TSSset ( int num, unsigned long base, unsigned char access )
{
    unsigned long limit=0x67;
    gdt[num].baseLow = ( base & 0xFFFF );
    gdt[num].baseMiddle = ( base >> 16 ) & 0xFF;
    gdt[num].baseHigh = ( base >> 24 ) & 0xFF;

    gdt[num].limitLow = ( limit & 0xFFFF );
    /*granularity.low is limit.high*/
    gdt[num].granularity = ( ( limit >> 16 ) & 0x0F );
    gdt[num].access = access;
}

/*funzione da usare per eseguire un task, il parametro path serve a dafinire il file in formato elf da cui caricare il task*/
int exec(char *path,char privilegi)
{
    struct taskStruct *newTask;
    int taskId=addTask("test",privilegi);
    newTask=getTask(taskId);
    printf(1,"caricamento elf...\n");
    /*parsa l'ELF e carica in memoria i vari segmenti*/
    if(loader_loadElf(path,taskId)==LOADER_OK)
    {
        printf(1,"elf caricato\n");
    }
    else
    {
        printf(1,"errore nel caricamento dell elf\n");
    }
   
  
    return taskId;
}

/*funzione che alloca le strutture necessarie per un nuovo task e le prapara per il loader*/
int addTask ( char nome[MAX_TASK_NAME_LEN],char privilegi )
{
    /*alloca le strutture necessarie*/
    struct taskStruct *newTask=kmalloc ( sizeof ( struct taskStruct ) );
    struct taskListElement *newListElement=kmalloc ( sizeof ( struct taskListElement ) );
    struct taskListElement *pointer;

    newListElement->task=newTask;
    newListElement->next=0;


    newTask->listaPagine=0;
    newTask->procID=getNewProcID();
    newTask->privilegi=privilegi;
    strcpy ( nome,newTask->nome );
    newTask->codeSegmentBase=0;/*RICORDARSI DI FARLO AGGIORNARE DAL LOADER*/
    newTask->codeSegmentSize=0;/*RICORDARSI DI FARLO AGGIORNARE DAL LOADER*/
    newTask->dataSegmentBase=0;/*RICORDARSI DI FARLO AGGIORNARE DAL LOADER*/
    newTask->dataSegmentSize=0;/*RICORDARSI DI FARLO AGGIORNARE DAL LOADER*/
    newTask->stackSegmentBase=0;/*RICORDARSI DI FARLO AGGIORNARE DAL LOADER*/
    newTask->stackSegmentSize=0;/*RICORDARSI DI FARLO AGGIORNARE DAL LOADER*/

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

    newTask->TSS.cr3= ( unsigned int ) pageDir;

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

    if ( pointer!=0 )
    {
        while ( pointer->next!=0 )
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
int removeTask ( unsigned int procID )
{
    struct taskListElement *pointer=taskListRoot;
    struct taskListElement *prePointer=0;

    if ( pointer!=0 )
    {
        do
        {
            if ( pointer->task->procID==procID )
            {
                /*trovato*/
                if ( prePointer==0 )
                {
                    kfree ( taskListRoot->task );
                    kfree ( taskListRoot );

                    taskListRoot=0;
                }
                else
                {
                    prePointer->next=pointer->next;

                    /*dealloca le strutture*/
                    kfree ( pointer->task );
                    kfree ( pointer );
                }
                return 1;
            }
            prePointer=pointer;
            pointer=pointer->next;
        }
        while ( pointer!=0 );
    }


    return 0;
}

struct taskStruct *getTask ( unsigned int procID )
{
    struct taskListElement *pointer=taskListRoot;

    if ( pointer!=0 )
    {
        do
        {
            if ( pointer->task->procID==procID )
            {
                /*trovato*/
                return pointer->task;
            }
            pointer=pointer->next;
        }
        while ( pointer!=0 );
    }


    return 0;
}

/*ritorna un id non utilizzato*/
unsigned int getNewProcID()
{
    struct taskListElement *pointer;
    unsigned int id=0;

    while ( 1 )
    {
        pointer=taskListRoot;

        if ( pointer!=0 )
        {
            int find=0;
            do
            {
                if ( pointer->task->procID==id )
                    find=1;
                pointer=pointer->next;
            }
            while ( pointer!=0 );

            if ( find==0 )
                return id;
        }
        else
        {
            return id;
        }

        id++;
    }
}


/*funzione da utilizzare per l'allocazione di una nuova pagina per un task
la pagina fisica viene allocata e viene ritornata l'istanza aggiunta alla lista delle pagine del task*/
struct pagina *allocaNuovaPagina ( unsigned int procID,unsigned int indirizzoLog )
{
    /*prima controlla che la pagina fisica non esista gia'*/
    struct pagina *check=getPagina(procID,indirizzoLog);
    if(check!=0)
        return check;
    
    struct pagina *temp=kmalloc ( sizeof ( struct pagina ) );
    
    /*setta i valori*/
    temp->procID=procID;
    temp->indirizzoLog=indirizzoLog;
    
    temp->indirizzoFis=getFreePage();/*cerca una pagina fisica libera*/
    /*TODO: verificare che il controllo funzioni*/
    if ( temp->indirizzoFis==0 )
    {
        kernelPanic ( "allocaNuovaPagina()","the fisic memory is full. I can't allocate a new page, sorry." );
    }
    setPaginaFisica ( temp->indirizzoFis ,1 );/*segna la pagina come utilizzata*/
    /*aggiunge alla lista delle pagine*/
    addPaginaToTaskPageList ( temp );

    return temp;
}

/*funzione da utilizzare per la deallocazione di una pagina di un task*/
unsigned int deallocaPagina ( unsigned int procID,unsigned int indirizzoLog )
{
    struct pagina *pointer;
    pointer=getTask ( procID )->listaPagine;
    
    /*cerca la pagina nella lista*/
    while ( pointer!=0 )
    {
        if ( pointer->procID==procID && pointer->indirizzoLog==indirizzoLog )
            break;
        pointer=pointer->next;
    }
    
    if ( pointer==0 ) /*se non ha trovato la pagina*/
    {
        kernelPanic ( "deallocaPagina()","I can't find the page to delete." );
        return 0;
    }
    
    /*aggiorna la bitmap se necessario*/
    if ( pointer->indirizzoFis!=0 ) /*se la pagina e' in memoria RAM*/
    {
        setPaginaFisica ( pointer->indirizzoFis ,0 );
    }
    else/*se la pagina non e' in memoria RAM*/
    {
        /*TODO: implementare la deallocazione nel caso la pagina sia swappata sull hard disk*/
        kernelPanic ( "deallocaPagina()","I can't remove a swapped page." );
    }
    
    /*rimuove la pagina dalla lista e dealloca la struttura*/
    removePaginaFromList ( procID,indirizzoLog );
    
    return 1;
}

struct pagina *getPagina(unsigned int procID,unsigned int indirizzoLog)
{
    struct pagina *pointer;
    pointer=getTask ( procID )->listaPagine;
    
    /*cerca la pagina nella lista*/
    while ( pointer!=0 )
    {
        if ( pointer->procID==procID && pointer->indirizzoLog==indirizzoLog )
            break;
        pointer=pointer->next;
    }
    
    if ( pointer==0 ) /*se non ha trovato la pagina*/
    {
        return 0;
    }
    else
    {
        return pointer;
    }
}

/*funzione che si assicura che nel range definito da parametro un processo abbia delle pagine utilizzabili*/
unsigned int allocMemory(unsigned int procID, unsigned int baseLogicAddr,unsigned int size)
{
    /*TODO: aggiungere un controllo sul range*/
    unsigned int c=0;
    unsigned int startMem=(unsigned int)baseLogicAddr & 0xFFFFF000;
    while(startMem<=(unsigned int)baseLogicAddr+size)
    {
        allocaNuovaPagina (procID,startMem);/*alloca la pagina, in caso esista già la funzione non esegue niente*/
        startMem+=0x1000;
        c+=0x1000;
    }
    return c;
}

/*funzione per copiare dei dati in memoria ad un task non correntemente mappato*/
void memcpyToTask( char * source, unsigned int count, char * dest, unsigned int procID )
{
    /*usa una pagina temporanea per mappare gli indirizzi fisici e scriverci i dati*/
    char flags=PAG_PRESENT|PAG_READWRITE|PAG_SUPERVISOR|PAG_4KPAGE;
    unsigned int tempTableI,tempPageI;
    tempTableI=getTableFromVirtualAdress((unsigned int)tempPage);
    tempPageI=getPageFromVirtualAdress((unsigned int)tempPage);
    
    unsigned int startMem= (unsigned int)dest & 0xFFFFF000;/*l' indirizzo logico della prima pagina su cui scrivere*/
    unsigned int offset=getOffsetFromVirtualAdress((unsigned int)dest);
    unsigned int pointer=offset;/*indice usato per la destinazione, la prima volta pointer vale offset mentre i cicli successivi viene settato a zero*/
    unsigned int sourcePointer=0;/*indice usato per l'origine, non viene mai azzerato*/
    /*passa tutte le pagine da scrivere*/
    while(startMem<=(unsigned int)dest+count)/*finche' non si sono passate tutte le pagine sotto l' area da scrivere*/
    {

        /*recupera l'indirizzo fisico della pagina logica*/
        struct pagina *pg=getPagina(procID,startMem);
        if(pg==0)
        {
            printf(1,"huston we have a problem at 0x%x max:0x%x\n",startMem,(unsigned int)dest+count);
        }
        unsigned int fisicAddr= pg->indirizzoFis;
        /*prepara la pagina temporanea*/
        printf(1,"    indirizzo fisico: 0x%x -> 0x%x\n",fisicAddr,startMem);
        setPageSelector ( tempTableI, tempPageI,fisicAddr,flags );
        invalidateLookasideBuffer();
        /*scrivi i dati*/
        while(pointer<0x1000 && sourcePointer < count)
        {
            tempPage[pointer]=source[sourcePointer];
            pointer++;
            sourcePointer++;
        }
        startMem+=0x1000;
        pointer=0;

    }
}
