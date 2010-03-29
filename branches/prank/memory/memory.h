#pragma once

void gdtSet ( int num, unsigned long base, unsigned long limit, unsigned char gran, unsigned char access );
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