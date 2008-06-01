/* Copyright (C) 2008 Leonardo Taglialegne <leonardotaglialegne+clearos@gmail.com>
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

struct gdtEntry gdt[3];
struct gdtPtr gdtPointer;

extern void gdtFlush();


#define PAG_PRESENT 	0x1
#define PAG_NOTPRESENT 	0x0

#define PAG_READONLY 	0x0
#define PAG_READWRITE	0x2

#define PAG_USER	0x4
#define PAG_SUPERVISOR	0x0

#define PAG_4KPAGE	0x0


/* 1 pagina = 4096 byte = 0x1000 */

#define KERNEL_START        0x0 /* 1.048.576 byte */
#define MEMORY_START 0x00400000 /* 4.194.304 byte*/

#define MAX_PAGES_IN_MEMORY 100000 /* numero massimo di pagine allocabili in memoria contemporaneamente */

#define MIN_SIZE_ALLOCABLE 4 /* minima unità allocabile = 4byte */

unsigned int *pageDir,tempPageSelector,*tempPage; /* area da 4096byte che ospita la pagedir del kernel */

unsigned int memoryBitmap[MAX_PAGES_IN_MEMORY/32+1];	/* flag per ogni blocco di 4k della memoria fisica */

void initPaging();

void setPageTableSelector(unsigned int *obj,unsigned int tableAdress,unsigned int flags);
void setPageSelector(unsigned int *obj,unsigned int pageAdress,unsigned int flags);

int getBit(int x);
void setBit(int x,unsigned int value);
int getBitExt(unsigned int *bitmap,int x);
void setBitExt(unsigned int *bitmap,int x,unsigned int value);

/*
 scrive la bitmap in una pagina e inizializza tutti i bit a zero
*/
void writeBitmapOnPage(unsigned int* adress);

/*
 ritorna un indirizzo fisico per l'allocazione di una nuova pagina
 alloca: indica se segnare questo indirizzo come utilizzato
*/
unsigned int getNewPage(int alloca);

/*
 alloca una nuova pagetable e la inserisce nella pagedir
 ritorna l'indice in cui e' inserito il selettore
*/
unsigned int addNewPageTable(unsigned int flags);

/* alloca una nuova pagina e ritorna l'indirizzo logico */
unsigned int addNewPage(unsigned int flags);

/*
 dealloca una pagina
 BaseAdress: indirizzo logico di inizio pagina
*/
void deletePage(unsigned int virtualAdress);

/*
 dealloca una pagetable
 num: indice della pagetable nella pagedir
*/
void deletePageTable(unsigned int num);


void* malloc(unsigned int byte);
void free(void *pointer,unsigned int size);


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
#endif
