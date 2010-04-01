/* Copyright (C) 2008 Leonardo Taglialegne <leonardotaglialegne+tylyos@gmail.com>
* Copyright (C) 2008 Luca Salmin
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

#ifndef MEMORY_HEAP_H
#define MEMORY_HEAP_H

void *calloc ( unsigned int num, unsigned int size );
void* kmalloc ( unsigned int byte );
void kfree ( void *pointer );

unsigned int getHeapSize();/*ritorna il numero di bytes fisici riservati all heap*/

void increaseHeapSize();/*alloca una pagina fisica e la mappa in fondo all heap*/
void decreaseHeapSize();/*dealloca l'ultima pagina dell heap*/

/*struttura lista allocazioni*/
struct memoryArea
{
    unsigned int size;/*dimensione in byte*/
    struct memoryArea *next;/*indirizzo header della prossima area*/
} __attribute__ ( ( packed ) );

struct memoryArea *kmallocList;/*liste aree allocate e libere*/

#endif