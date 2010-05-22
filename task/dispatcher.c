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
#include <task/task.h>

/*funzione per caricare nel task register il selettore del tss, il parametro i specifica l'indice della gdt*/
void loadTSSregister(short unsigned int selector,unsigned int i)/*TODO: sarebbe meglio ricavare i dal selettore*/
{
    gdt[i].access &= 0xFD;/*azzera il fottuto flag BUSY, i manuali intel non specificano che ltr lo setta senza dire niente*/
    asm volatile ("ltr %0\n" : :"r"(selector));
}

unsigned int getTSS()
{
    unsigned int data=0;
asm volatile ( "str %0":"=r" ( data ) );
    return data;
}

void switchTo(unsigned int selector)
{
    unsigned int sel[2];

    sel[1] = selector;

    //asm ("lcall %0": :"m" (*sel));
    /*ribilita gli interrupt*/
    //asm ("sti");
    /*esegue lo switch*/
    asm ("lcall *%0": :"m" (*sel));
}





void dispatch(int procID)
{
/*disabilita gli interrupt, le isr altrimenti danno errore per via dei cambi che questa funzione fa sui selettori dei tss*/
asm("cli");
    /*risetta per sicurezza i selettori dei tss*/
    newTSSselector=segmentSelector ( NEW_TSS_INDEX,0,RPL_USER );

    struct taskStruct *t;
    t=getTask(procID);
    if(t==0)
        kernelPanic("dispatch","il task non esiste"); 
    /*mappa in memoria le pagine del task preparando il contesto*/
    dispatcher_mapPages(t);
    /*il descrittore selezionato da newTSS deve essere modificato in modo che punti al tss del nuovo task*/
    TSSset(NEW_TSS_INDEX,(unsigned int)&t->TSS,MEM_TSS|MEM_USER|MEM_PRESENT);

  t->TSS.link=kernelInterruptTSSselector & 0x0000ffff;
  /*abilita gli interrupt nel contesto del task*/
  t->TSS.eflags|=1<<9;

    t->lastScheduledTime=time();

switchTo(newTSSselector);
  

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
                printf(1,"pagina settata (task addr space: 0x%x)\n",virtualAddr-user_start);
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
