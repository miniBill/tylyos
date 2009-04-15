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

/*ritorna un selettore di segmento assemblato usando i dati passati*/
unsigned short segmentSelector(unsigned int index,char tableIndicator,char RPL);

void initGdt();

struct gdtEntry{
    unsigned short limitLow;
    unsigned short baseLow;
    unsigned char baseMiddle;
    unsigned char access; /* |p|dpl|s|type| */
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

#define MIN_HEAP_SIZE 0x98A000 /* 10MB circa di heap a cui verrà sommato il valore calcolato in base allla memoria fisica */

unsigned int mallocMemoryStart; /* indirizzo inizio allocazioni kmalloc*/
unsigned int memoriaFisica; /* byte di memoria fisica */

unsigned int userMemoryStart; /* indirizzo di partenza del segmento user*/

unsigned int *pageDir; /* area da 4096byte che ospita la pagedir del kernel */


void initPaging();

void setPageTableSelector(unsigned int *obj,unsigned int tableAdress,unsigned int flags);
void setPageSelector(unsigned int *obj,unsigned int pageAdress,unsigned int flags);

int getBitExt(unsigned int *bitmap,unsigned int x);
void setBitExt(unsigned int *bitmap,unsigned int x,unsigned int value);





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


/*descrittore di pagina fisica*/
struct pagina
{
    unsigned int procID;         
    unsigned int indirizzoLog;   /*indirizzo logino alla quale èappata la pagina nel processo*/
    unsigned int indirizzoFis;    /*indirizzo fisico, se uguale a 0 e' swappata sull hd */
    struct pagina *next;
};

struct pagina *pagesList;


/*aggiunge con un insert sort una pagina nella lista delle pagine*/
/*NON MODIFICA LA BITMAP*/
void addPaginaToList(struct pagina *p);

/*rimuove una pagina dalla lista delle pagine  E DEALLOCA LA STRUTTURA*/
/*NON MODIFICA LA BITMAP*/
unsigned int removePaginaFromList(unsigned int procID,unsigned int indirizzoLogico);

/*ritorna un indirizzo fisico libero, pronto per lallocazione di una nuova pagina*/
/*NON MODIFICA LA BITMAP*/
unsigned int getFreePage();


struct bitmap
{
    unsigned int *data;
    unsigned int size;
};

/*rappresenta attraverso una bitmap lo stato di ogni pagina fisica in memoria
 * 0 non usata
 * 1 usata
 *
 * NB: le pagine fisiche a cui si riferisce la bitmap sono quelle mappate dopo il kernel
 */
struct bitmap mappaPagineFisiche;

/*ritorna l'indice corrispondente ad una pagina fisica da utilizzare nella bitmap delle pagine fisiche*/
unsigned int convertFisAddrToBitmapIndex(unsigned int addr);
/*ritorna l'indirizzo fisico corrispondente ad un determinato indice della bitmap delle pagine fisiche*/
unsigned int convertBitmapIndexToFisAddr(unsigned int index);

/*setta lo stato di una pagina fisica nella bitmap
 * 0 libera
 * 1 allocata
 */
void setPaginaFisica(unsigned int index,unsigned int stato);

/*funzione da utilizzare per l'allocazione di una nuova pagina per un task*/
struct pagina *allocaNuovaPagina(unsigned int procID,unsigned int indirizzoLog);

/*funzione da utilizzare per la deallocazione di una pagina di un task*/
unsigned int deallocaPagina(unsigned int procID,unsigned int indirizzoLog);

#endif
