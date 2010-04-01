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
#ifndef MEMORY_H_
#define MEMORY_H_

#include <memory/heap.h>

void memcpy(char * source, unsigned int count, char * dest);

void invalidateLookasideBuffer();/*costringe la cpu ad aggiornare il lookaside buffer*/

unsigned int getEBP();
unsigned int getESP();

/*setta una riga della GDT (descrittore di segmento)*/
void gdtSet ( int num, unsigned long base, unsigned long limit, unsigned char gran, unsigned char access );

/*ritorna un selettore di segmento assemblato usando i dati passati*/
unsigned short segmentSelector ( unsigned int index,char tableIndicator,char RPL );

void initGdt();

struct gdtEntry
{
    unsigned short limitLow;
    unsigned short baseLow;
    unsigned char baseMiddle;
    unsigned char access; /* |p|dpl|s|type| */
    unsigned char granularity;
    unsigned char baseHigh;
} __attribute__ ( ( packed ) );

struct gdtPtr
{
    unsigned short limit;
    unsigned int base;
} __attribute__ ( ( packed ) );

#define NUMERO_SEGMENTI 10

struct gdtEntry gdt[NUMERO_SEGMENTI];
struct gdtPtr gdtPointer;

unsigned short segmentoCodiceKernel,segmentoDatiKernel;/*selettori di segmento kernel*/
unsigned short segmentoCodiceUser,segmentoDatiUser;/*selettori di segmento user*/

extern void gdtFlush ( unsigned short selettoreSegmentoCodice,unsigned short selettoreSegmentoDati );


#define PAG_PRESENT 	0x1
#define PAG_NOTPRESENT 	0x0

#define PAG_READONLY 	0x0
#define PAG_READWRITE	0x2

#define PAG_USER	0x4
#define PAG_SUPERVISOR	0x0

#define PAG_4KPAGE	0x0


/* 1 pagina = 4096 byte = 0x1000 */

#define KERNEL_START        0x0 /* indirizzo di inizio kernel*/
unsigned int kernel_end;    /*indirizzo finale dell immagine del kernel*/
unsigned int user_start;    /*indirizzo iniziale dell area user, viene calcolata sommando a kernel_end la dimensione dell immagine ed alineando l'indirizzo a 0x1000*/
#define HEAP_START          (0xB2D05000+user_start) /*per lasciare circa 3GB all area user*/
char *heapEndPointer;/*puntatore che indica fino a dove si estende l'heap*/


unsigned int loadedModuleSize;/*dimensione del modulo caricato in memoria, il valore viene usato per calcolare mallocMemoryStart*/
char *loadedModule;/*indirizzo del modulo caricato dalla funzione hunt_load*/

unsigned int memoriaFisica; /* byte di memoria fisica */

extern unsigned int l_pageDir,l_end,l_tempPage;
unsigned int *pageDir; /* area da 4096byte che ospita la pagedir del kernel seguita dall' area per le page tables*/
char *tempPage;

void initPaging();

void setPageTableSelector (unsigned int pageTableIndex ,unsigned int tableAdress,unsigned int flags );
void setPageSelector (unsigned int pageTableIndex,unsigned int pageIndex,unsigned int pageAdress,unsigned int flags );

int getBitExt ( unsigned int *bitmap,unsigned int x );
void setBitExt ( unsigned int *bitmap,unsigned int x,unsigned int value );


/* ritornano le componenti dell indirizzo logico */
unsigned int getTableFromVirtualAdress ( unsigned int adress );
unsigned int getPageFromVirtualAdress ( unsigned int adress );
unsigned int getOffsetFromVirtualAdress ( unsigned int adress );

/* ritorna l'indirizzo logico prendendo come parametri pagetable pagina e offset */
unsigned int virtualAdress ( unsigned int table,unsigned int page,unsigned int offset );

/* ritorna l'indirizzo che indica un selettore di pagetable o pagina */
unsigned int fisicAdressFromSelector ( unsigned int sel );
unsigned int fisicAdressFromSelectorExt ( unsigned int tableIndex,unsigned int pageIndex );

extern unsigned int read_cr0();
extern void write_cr0 ( unsigned int data );
extern unsigned int read_cr3();
extern void write_cr3 ( unsigned int data );



/*descrittore di pagina fisica*/
struct pagina
{
    unsigned int procID;
    unsigned int indirizzoLog;   /*indirizzo logino alla quale e' mappata la pagina nel processo*/
    unsigned int indirizzoFis;    /*indirizzo fisico, se uguale a 0 e' swappata sull hd */
    struct pagina *next;
};



/*aggiunge con un insert sort una pagina nella lista delle pagine*/
/*NON MODIFICA LA BITMAP*/
void addPaginaToTaskPageList ( struct pagina *p );

/*rimuove una pagina dalla lista delle pagine  E DEALLOCA LA STRUTTURA*/
/*NON MODIFICA LA BITMAP*/
unsigned int removePaginaFromList ( unsigned int procID,unsigned int indirizzoLogico );

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
 * NB: le pagine fisiche a cui si riferisce la bitmap sono quelle mappate dopo il kernel e vengono usate sia per i processi sia per l'heap del kernel stesso
 */
struct bitmap mappaPagineFisiche;

char mappaPagineFisicheBitmapData[(1024*1024)/8];/*un bit per ogni pagina del sistema, stimato per eccesso in modo di garantire 4GB di ram fisica*/

/*ritorna l'indice corrispondente ad una pagina fisica da utilizzare nella bitmap delle pagine fisiche*/
unsigned int convertFisAddrToBitmapIndex ( unsigned int addr );
/*ritorna l'indirizzo fisico corrispondente ad un determinato indice della bitmap delle pagine fisiche*/
unsigned int convertBitmapIndexToFisAddr ( unsigned int index );

/*setta lo stato di una pagina fisica nella bitmap
 * 0 libera
 * 1 allocata
 */
void setPaginaFisica ( unsigned int indirizzo,unsigned int stato );

#endif
