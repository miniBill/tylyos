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
    gdtSet ( 3, t->codeSegmentBase, (t->codeSegmentSize+0x1000) /0x1000,MEM_GRANULAR|MEM_32,MEM_PRESENT|MEM_CODE_DATA|MEM_RW|MEM_USER|MEM_CODE );
    /*segmento dati user mode*/
    gdtSet ( 4, t->dataSegmentBase, (t->dataSegmentSize+0x1000) /0x1000,MEM_GRANULAR|MEM_32,MEM_PRESENT|MEM_CODE_DATA|MEM_RW|MEM_USER|MEM_DATA );
    /*TODO: JMP riferendosi al TSS del nuovo task per mandare in esecuzione*/
}

/*funzione che mappa in memoria le pagine del task*/
void dispatcher_mapPages(struct taskStruct *t)
{
    /*TODO: passare la lista delle pagine del task t e ricostruire le pagetables, ricordarsi di invaldare il lookaside buffer*/
}