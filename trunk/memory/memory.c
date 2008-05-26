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

unsigned int GetNewPage(int alloca){
    int c;
    for(c=0;c<MAX_PAGES_IN_MEMORY;c++){
        if(getBit(c)==0){
            if(alloca==1)
                setBit(c,1);
            return MEMORY_START+(c*0x1000);
        }
    }
    return 0x0;
}

/* TODO: sistemare meglio e pulire codice */
void InitPaging(){
    int c,c2;
    unsigned int PageTab;
    char debug[36]={0};
    asm("cli");
    /* azzera mappa */
    for(c=0;c<MAX_PAGES_IN_MEMORY/32+1;c++)
        memoryBitmap[c]=0;
    PageDir=(unsigned int*)GetNewPage(1);
    PageTab=GetNewPage(1);
    /* azzera gli altri record della pagedir */
    for(c=0;c<1024;c++)
        setPageTableSelector(&PageDir[c],0,0);
    /* setta la prima pagetable nella pagedir */
    setPageTableSelector(&PageDir[0],PageTab>>12,PAG_PRESENT|PAG_READWRITE|PAG_SUPERVISOR|PAG_4KPAGE);
    /* ultimo record punta alla pagina della pagedir */
    setPageTableSelector(&PageDir[1023],(unsigned int)PageDir>>12,PAG_PRESENT|PAG_READWRITE|PAG_SUPERVISOR|PAG_4KPAGE);
    /* inserisce pagine nella prima pagetable */
    for(c2=0,c=KERNEL_START/0x1000;c<0x1000;c++,c2++){
        debug[0]='\0';
        /* strapp(debug,"descrittore in: %x",(void *)(PageTab+(c*4)));
        strapp(debug,"indirizzo: %x",(void *)((c*0x1000)>>12));
         writeline(debug); */
        setPageSelector((unsigned int*)((PageTab)+(c2*4)),(c*0x1000)>>12,PAG_PRESENT|PAG_READWRITE|PAG_SUPERVISOR|PAG_4KPAGE);
    }
    /* ultimo record punta alla pagina della tabella */
    setPageSelector((unsigned int*)(PageTab+(1023*4)),PageTab>>12,PAG_PRESENT|PAG_READWRITE|PAG_SUPERVISOR|PAG_4KPAGE);

    write_cr3((unsigned int)PageDir); /* put that page directory address into CR3 */
    write_cr0(read_cr0() | 0x80000000); /* set the paging bit in CR0 to 1 */
    /*write("1?");
    asm("sti");FIXME
    write("2?");*/
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
    *obj|=(pageAdress<<12)&0xFFFFF000;
}

int getBit(int x){
    int off1,off2;
    off1=x/32;
    off2=x%32;
    return (memoryBitmap[off1]>>off2)&1;
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
