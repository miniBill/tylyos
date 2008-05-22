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

#include "memory.h"
#include "gdtflags.h"
#include "../drivers/screen/screen.h"
#include "../drivers/screen/io.h"

void gdtSet(int num, unsigned long base, unsigned long limit, unsigned char gran, unsigned char access){
    gdt[num].baseLow = (base & 0xFFFF);
    gdt[num].baseMiddle = (base >> 16) & 0xFF;
    gdt[num].baseHigh = (base >> 24) & 0xFF;

    gdt[num].limitLow = (limit & 0xFFFF);
    /*granularity.low is limit.high*/
    gdt[num].granularity = ((limit >> 16) & 0x0F);
    gdt[num].granularity |= gran & 0xC0;
    gdt[num].access = access;
}

void initGdt(){
    gdtPointer.limit = (sizeof(struct gdtEntry) * 3) - 1;
    gdtPointer.base = (int)&gdt;

    gdtSet(0, 0, 0, 0, 0); /*Il puntatore NULL!*/
    gdtSet(1, 0, 0xFFFFFFFF,MEM_GRANULAR|MEM_32,
        MEM_PRESENT|MEM_CODE_DATA|MEM_RW|MEM_CODE);
    gdtSet(2, 0, 0xFFFFFFFF,MEM_GRANULAR|MEM_32,
        MEM_PRESENT|MEM_CODE_DATA|MEM_RW);
    gdtFlush();
}

enum{
    PAGE_NOTPRESENT = 0x0,
    PAGE_PRESENT    = 0x1,

    PAGE_READONLY   = 0x0,
    PAGE_READWRITE  = 0x2,

    PAGE_SUPERVISOR = 0x0,
    PAGE_USER       = 0x4,

    PAGE_4KPAGE    = 0x0
};

/*######################### Paginazione ####################################*/

/*
	TODO: sistemare meglio e pulire codice
*/
void InitPaging()
{
    int c;
    char *debug="\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0";
    asm("cli");
    PageDir=(unsigned int*)MEMORY_START;
    setPageTableSelector(&PageDir[0],(MEMORY_START+4096)>>12,PAG_PRESENT|PAG_READWRITE|PAG_USER|PAG_4KPAGE);
    for(c=1;c<1024;c++)/* azzera gli altri record della pagedir */
    {
        setPageTableSelector(&PageDir[0],0,0);
    }
    /* ultimo record punta alla pagina della pagedir */
    setPageTableSelector(&PageDir[1023],(unsigned int)PageDir>>12,PAG_PRESENT|PAG_READWRITE|PAG_USER|PAG_4KPAGE);
    for(c=0;c<1024;c++)/* inserisce 1024 pagine nella prima pagetable */
    {
        debug[0]='\0';
        strapp(debug,"descrittore in: %x",(void *)((MEMORY_START+4096)+(c*4)));
        strapp(debug,"indirizzo: %x",(void *)((c*4096)>>12));
         writeline(debug); 
        setPageSelector((unsigned int*)(MEMORY_START+4096)+(c*4),(c*4096)>>12,PAG_PRESENT|PAG_READWRITE|PAG_USER|PAG_4KPAGE);
    }
    /* ultimo record punta alla pagina della tabella */
    setPageSelector((unsigned int*)MEMORY_START+4096+(1023*4),(MEMORY_START+4096)>>12,PAG_PRESENT|PAG_READWRITE|PAG_USER|PAG_4KPAGE);
    write_cr3((unsigned int)PageDir); /* put that page directory address into CR3 */
    write_cr0(read_cr0() | 0x80000000); /* set the paging bit in CR0 to 1 */
    asm("sti");
}

/* obj: indirizzo dell'area su cui scrivere il selettore
 * TableAdress: indirizzo della tabella nella memoria (NB: una tabella occupa una pagina da 4K
 * questo indirizzo indica i 20 bit più significativi dell indirizzo della tabella)
 */
void setPageTableSelector(unsigned int *obj,unsigned int tableAdress,unsigned int flags){
    *obj=0;
    *obj=flags;
    *obj|=tableAdress<<12;
}

/* obj: indirizzo dell'area su cui scrivere il selettore
 * TableAdress: indirizzo della pagina nella memoria (NB: una pagina occupa 4K
 * questo indirizzo indica i 20 bit più significativi dell indirizzo)
 */
void setPageSelector(unsigned int *obj,unsigned int pageAdress,unsigned int flags){
    *obj=0;
    *obj=flags;
    *obj|=pageAdress<<12;
}

int getBit(int x){
    int off1,off2;
    off1=x/32;
    off2=x%32;
    return (memoryBitmap[off1]>>off2)&0x1;
}

void setBit(int x,unsigned int value){
    int off1,off2;
    off1=x/32;
    off2=x%32;
    value&=0x1;
    if(value)
        memoryBitmap[off1]|=1<<off2;
    else
        memoryBitmap[off1]&=~(1<<off2);
}
