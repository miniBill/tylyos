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

#include "dispatcher.h"
#include <memory/memory.h>
#include <memory/gdtflags.h>
#include <kernel/kernel.h>
#include <lib/string.h>

/*funzione per caricare nel task register il selettore del tss, il parametro i specifica l'indice della gdt*/
void loadTSSregister(short unsigned int selector,unsigned int i)/*TODO: sarebbe meglio ricavare i dal selettore*/
{
    asm volatile ("ltr %0\n" : :"r"(selector));
    gdt[i].access &= 0xFD;/*azzera il fottuto flag BUSY, i manuali intel non specificano che ltr lo setta senza dire niente*/
}



void dispatch(int procID)
{
    struct taskStruct *t;
    t=getTask(procID);
    /*prepara i dati del task in modo che siano mappati in meoria*/
    dispatcher_mapPages(t);
    /*il descrittore selezionato da currentTSS deve essere settato uguale a quello di newTSS*/
    gdt[CURRENT_TSS_INDEX]=gdt[NEW_TSS_INDEX];
    /*il registro che contiene il tss deve essere settato con il selettore currentTSS*/
    loadTSSregister(currentTSS,CURRENT_TSS_INDEX);
    /*il descrittore selezionato da newTSS deve essere modificato in modo che punti al tss del nuovo task*/
    TSSset(NEW_TSS_INDEX,(unsigned int)&t->TSS,MEM_TSS|MEM_KERNEL|MEM_PRESENT);
    /*vanno modificati i selettori di segmento selezionati da segmentoDatiUser e segmentoCodiceUser usando base e limit presenti nella struttura del nuovo task*/
    /*TODO: verificare che limit sia passato in modo corretto alla funzione*/
    /*segmento codice user mode, verranno modificati prima di ogni switch*/
    gdtSet ( 3, t->codeSegmentBase, (t->codeSegmentSize+0x1000) /0x1000,MEM_GRANULAR|MEM_32, MEM_PRESENT|MEM_CODE_DATA|MEM_RW|MEM_KERNEL|MEM_CODE );
    /*segmento dati user mode*/
    gdtSet ( 4, t->dataSegmentBase, (t->dataSegmentSize+0x1000) /0x1000,MEM_GRANULAR|MEM_32, MEM_PRESENT|MEM_CODE_DATA|MEM_RW|MEM_KERNEL|MEM_DATA );
    /*TODO: JMP riferendosi al TSS del nuovo task per mandare in esecuzione*/

   /*asm volatile ("push %0 ;\n \
                   push $0;\n \
                   retf" 
                  : :"r"(newTSS));*/
  //asm volatile ("jmp *(%0)\n" : :"r"(newTSS));
  contextSwitch();
   
}

/*funzione che mappa in memoria le pagine del task*/
void dispatcher_mapPages(struct taskStruct *t)
{
    unsigned int virtualAddr=user_start;
    struct pagina *currentPage=t->listaPagine;
    char flags=PAG_PRESENT|PAG_READWRITE|PAG_USER|PAG_4KPAGE;
    
    while(virtualAddr < HEAP_START)/*passa tutte le pagine virtuali riservate ai processi*/
    {
        if(currentPage!=0)
        {
            if(currentPage->indirizzoLog == virtualAddr)
            {
                /*setta la pagina e scorri la lista avanti*/
                printf(1,"pagina settata\n");
                setPageSelector ( getTableFromVirtualAdress(virtualAddr), getPageFromVirtualAdress(virtualAddr),currentPage->indirizzoFis,flags );
                currentPage=currentPage->next;
            }
            else if(currentPage->indirizzoLog > virtualAddr)
            {
                /*setta la pagina come vuota perche' la lista delle pagine e' ordinata*/
                setPageSelector( getTableFromVirtualAdress(virtualAddr), getPageFromVirtualAdress(virtualAddr),0,PAG_NOTPRESENT);
            }
            else
            {
                /*se si arriva qui' il pc rischia di implodere e diventare una supernova, non e' affatto una bella cosa*/
                kernelPanic("dispatcher_mapPages()","the fucking ordered list ISN'T ordered");
            }
        }
        else
        {
            /*setta la pagina come vuota*/
            setPageSelector( getTableFromVirtualAdress(virtualAddr), getPageFromVirtualAdress(virtualAddr),0,PAG_NOTPRESENT);
        }
        
        virtualAddr+=0x1000;
    }
    invalidateLookasideBuffer();
}