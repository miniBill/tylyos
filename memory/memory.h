/* Copyright (C) 2008 Leonardo Taglialegne <leonardotaglialegne+clearos@gmail.com>
 * Copyright (C) 2008 Luca Salmin 
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
#ifndef MEMORY_H_
#define MEMORY_H_

unsigned int getEBP();
unsigned int getESP();

void initGdt();

struct gdtEntry{
    unsigned short limitLow;
    unsigned short baseLow;
    unsigned char baseMiddle;
    unsigned char access;
    unsigned char granularity;
    unsigned char baseHigh;
} __attribute__((packed));

struct gdtPtr{
    unsigned short limit;
    unsigned int base;
} __attribute__((packed));

#define NUMERO_SEGMENTI 5 

struct gdtEntry gdt[NUMERO_SEGMENTI];
struct gdtPtr gdtPointer;

unsigned short segmentoCodiceKernel,segmentoDatiKernel;/*selettori di segmento kernel*/
unsigned short segmentoCodiceUser,segmentoDatiUser;/*selettori di segmento user*/

extern void gdtFlush(unsigned short selettoreSegmentoCodice,unsigned short selettoreSegmentoDati);


#define PAG_PRESENT 	0x1
#define PAG_NOTPRESENT 	0x0

#define PAG_READONLY 	0x0
#define PAG_READWRITE	0x2

#define PAG_USER	0x4
#define PAG_SUPERVISOR	0x0

#define PAG_4KPAGE	0x0


/* 1 pagina = 4096 byte = 0x1000 */

#define KERNEL_START        0x0 /* indirizzo di inizio kernel*/
#define KERNEL_MEMORY_START 0x00400000 /* indirizzo inizio zona allocazioni del kernel  NB: deve essere multiplo di 0x1000*/
#define USER_START 0xFA3000 /*20MB circa NB: deve essere multiplo di 0x1000*/

unsigned int mallocMemoryStart; /* indirizzo inizio allocazioni kmalloc*/



unsigned int *pageDir; /* area da 4096byte che ospita la pagedir del kernel */


void initPaging();

void setPageTableSelector(unsigned int *obj,unsigned int tableAdress,unsigned int flags);
void setPageSelector(unsigned int *obj,unsigned int pageAdress,unsigned int flags);

int getBitExt(unsigned int *bitmap,int x);
void setBitExt(unsigned int *bitmap,int x,unsigned int value);





void* kmalloc(unsigned int byte);
void kfree(void *pointer);


/* ritornano le componenti dell indirizzo logico */
unsigned int getTableFromVirtualAdress(unsigned int adress);
unsigned int getPageFromVirtualAdress(unsigned int adress);
unsigned int getOffsetFromVirtualAdress(unsigned int adress);

/* ritorna l'indirizzo logico prendendo come parametri pagetable pagina e offset */
unsigned int virtualAdress(unsigned int table,unsigned int page,unsigned int offset);

/* ritorna l'indirizzo che indica un selettore di pagetable o pagina */
unsigned int fisicAdressFromSelector(unsigned int sel);

extern unsigned int read_cr0();
extern void write_cr0(unsigned int data);
extern unsigned int read_cr3();
extern void write_cr3(unsigned int data);

/*struttura lista allocazioni*/
struct memoryArea{
    unsigned int size;/*dimensione in byte*/
    struct memoryArea *next;/*indirizzo header della prossima area*/
} __attribute__((packed));

struct memoryArea *kmallocList;/*liste aree allocate e libere*/

#endif
