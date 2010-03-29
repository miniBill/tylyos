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

#include "memory.h"
#include "gdtflags.h"
#include <drivers/screen/screen.h>
#include <lib/string.h>
#include <kernel/kernel.h>

void memcpy ( char * source, unsigned int count, char * dest )  /*TODO: testare*/
{
    if ( (unsigned int)dest < (unsigned int)source ) /*evita di sovrascrivere il buffer durante la copia*/
    {
      unsigned int p;
        for ( p=0;p < count; p++ )
        {   
            dest[p]=source[p];
        }
    }
    else
    {
      int p;
        for ( p=count;p>=0; p-- )
        {
            dest[count-p]=source[count-p];
        }
        
    }
   
}

void invalidateLookasideBuffer()/*costringe la cpu ad aggiornare il lookaside buffer*/
{
    write_cr3 ( read_cr3() );
}

/*ritorna il valore del registro EBP*/
unsigned int getEBP()
{
    unsigned int data=0;
asm volatile ( "mov %%ebp, %0":"=r" ( data ) );
    return data;
}
/*ritorna il valore del registro ESP*/
unsigned int getESP()
{
    unsigned int data=0;
asm volatile ( "mov %%esp, %0":"=r" ( data ) );
    return data;
}

/*ritorna un selettore di segmento assemblato usando i dati passati*/
unsigned short segmentSelector ( unsigned int index,char tableIndicator,char RPL )
{
    return ( ( index<<3 ) &0xFFF8 ) | ( ( tableIndicator<<2 ) &0x4 ) | ( RPL );
}

/*setta una riga della GDT (descrittore di segmento)*/
void gdtSet ( int num, unsigned long base, unsigned long limit, unsigned char gran, unsigned char access )
{
    gdt[num].baseLow = ( base & 0xFFFF );
    gdt[num].baseMiddle = ( base >> 16 ) & 0xFF;
    gdt[num].baseHigh = ( base >> 24 ) & 0xFF;

    gdt[num].limitLow = ( limit & 0xFFFF );
    /*granularity.low is limit.high*/
    gdt[num].granularity = ( ( limit >> 16 ) & 0x0F );
    gdt[num].granularity |= gran & 0xC0;
    gdt[num].access = access;
}

/*inizializza i segmenti di default*/
void initGdt()
{
    gdtPointer.limit = ( sizeof ( struct gdtEntry ) * NUMERO_SEGMENTI ) - 1;
    gdtPointer.base = ( int ) &gdt;

    gdtSet ( 0, 0, 0, 0, 0 ); /*Il puntatore NULL!*/
    /*segmento codice kernel*/
    gdtSet ( 1, 0, ( 0xFFFFFFFF ) /0x1000,MEM_GRANULAR|MEM_32,
             MEM_PRESENT|MEM_CODE_DATA|MEM_RW|MEM_KERNEL|MEM_CODE );
    /*segmento dati kernel*/
    gdtSet ( 2, 0, ( 0xFFFFFFFF ) /0x1000,MEM_GRANULAR|MEM_32,
             MEM_PRESENT|MEM_CODE_DATA|MEM_RW|MEM_KERNEL|MEM_DATA );


    segmentoCodiceKernel=segmentSelector ( 1,0,RPL_KERNEL );
    segmentoDatiKernel=segmentSelector ( 2,0,RPL_KERNEL );
    segmentoCodiceUser=segmentSelector ( 3,0,RPL_USER );
    segmentoDatiUser=segmentSelector ( 4,0,RPL_USER );

    gdtFlush ( segmentoCodiceKernel,segmentoDatiKernel );
}

/*######################### Paginazione ####################################*/

/* ritorna l'indirizzo che indica un selettore di pagetable o pagina */
inline unsigned int fisicAdressFromSelector ( unsigned int sel )
{
    return sel&0xFFFFF000;
}

unsigned int fisicAdressFromSelectorExt ( unsigned int tableIndex,unsigned int pageIndex )
{
    unsigned int pointer=(unsigned int)pageDir+0x1000;
    unsigned int *obj=( unsigned int* ) ( pointer+(0x1000*tableIndex) + ( pageIndex*4 ) );
    return fisicAdressFromSelector(*obj);
}

/* ritorna l'indirizzo logico prendendo come parametri pagetable pagina e offset */
unsigned int virtualAdress ( unsigned int table,unsigned int page,unsigned int offset )
{
    unsigned int temp;
    temp=offset&0xFFF;
    temp|= ( page&0x3FF ) <<12;
    temp|=table<<22;
    return temp;
}

/* ritornano le componenti dell indirizzo logico */
unsigned int getTableFromVirtualAdress ( unsigned int adress )
{
    return ( adress>>22 ) &0x3FF;
}
unsigned int getPageFromVirtualAdress ( unsigned int adress )
{
    return ( adress>>12 ) &0x3FF;
}
unsigned int getOffsetFromVirtualAdress ( unsigned int adress )
{
    return adress&0xFFF;
}

/*setta il valore di un selettore*/
/* pageTableIndex: indice che specifica la page table
 * TableAdress: indirizzo della tabella nella memoria (NB: una tabella occupa una pagina da 4K
 * 
 */
void setPageTableSelector (unsigned int pageTableIndex ,unsigned int tableAdress,unsigned int flags )
{
    unsigned int *obj=( unsigned int* ) ( ( unsigned int ) pageDir + ( pageTableIndex*4 ) );
    *obj=0;
    *obj=flags;
    *obj|= ( tableAdress ) & 0xFFFFF000;
}

/* pageTableIndex: indice che specifica la page table
 * pageIndex: indice della pagina nella pagetable
 * pageAdress: indirizzo della pagina nella memoria (NB: una pagina occupa 4K
 * 
 */
void setPageSelector (unsigned int pageTableIndex,unsigned int pageIndex,unsigned int pageAdress,unsigned int flags )
{
    unsigned int pointer=(unsigned int)pageDir+0x1000;
    unsigned int *obj=( unsigned int* ) ( pointer+(0x1000*pageTableIndex) + ( pageIndex*4 ) );
    *obj=0;
    *obj=flags;
    *obj|= ( pageAdress ) & 0xFFFFF000;
}



int getBitExt ( unsigned int *bitmap,unsigned int x )
{
    int off1,off2;
    off1=x/32;
    off2=x%32;
    return ( bitmap[off1]>>off2 ) &1;
}
void setBitExt ( unsigned int *bitmap,unsigned int x,unsigned int value )
{
    int off1,off2;
    off1=x/32;
    off2=x%32;
    value&=0x1;
    if ( value )
        bitmap[off1]|=1<<off2;
    else
        bitmap[off1]&=~ ( 1<<off2 );
}

/*ritorna l'indice corrispondente ad una pagina fisica da utilizzare nella bitmap delle pagine fisiche*/
unsigned int convertFisAddrToBitmapIndex ( unsigned int addr )
{
    return ( addr-user_start ) /0x1000;
}
/*ritorna l'indirizzo fisico corrispondente ad un determinato indice della bitmap delle pagine fisiche*/
unsigned int convertBitmapIndexToFisAddr ( unsigned int index )
{
    return ( index*0x1000 ) +user_start;
}

/*setta lo stato di una pagina fisica nella bitmap
 * 0 libera
 * 1 allocata
 */
void setPaginaFisica ( unsigned int indirizzo,unsigned int stato )
{
    unsigned int index=convertFisAddrToBitmapIndex(indirizzo);
    if ( index < mappaPagineFisiche.size )
    {
        setBitExt ( mappaPagineFisiche.data,index,stato );
    }
}

