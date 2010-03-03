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


void dispatch(unsigned int procID)
{
    struct taskStruct *t;
    t=getTask(procID);
    /*prepara i dati del task in modo che siano mappati in meoria*/
    dispatcher_mapPages(t);
    /*TODO: il descrittore selezionato da currentTSS deve essere settato uguale a quello di newTSS*/
    /*TODO: il registro che contiene il tss deve essere settato con il selettore currentTSS*/
    /*TODO: il descrittore selezionato da newTSS deve essere modificato in modo che punti al tss del nuovo task*/
    /*TODO: vanno modificati i selettori di segmento selezionati da segmentoDatiUser e segmentoCodiceUser usando base e limit presenti nella struttura del nuovo task*/
    /*TODO: JMP riferendosi al TSS del nuovo task per mandare in esecuzione*/
}

/*funzione che mappa in memoria le pagine del task*/
void dispatcher_mapPages(struct taskStruct *t)
{
    /*TODO: passare la lista delle pagine del task t e ricostruire le pagetables, ricordarsi di invaldare il lookaside buffer*/
}