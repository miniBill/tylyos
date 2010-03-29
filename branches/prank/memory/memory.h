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