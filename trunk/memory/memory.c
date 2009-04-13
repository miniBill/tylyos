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

#include "memory.h"
#include "gdtflags.h"
#include <drivers/screen/screen.h>
#include <lib/string.h>

/*ritorna il valore del registro EBP*/
unsigned int getEBP(){
    unsigned int data=0;
    asm volatile("mov %%ebp, %0":"=r" (data) );
    return data;
}
/*ritorna il valore del registro ESP*/
unsigned int getESP(){
    unsigned int data=0;
    asm volatile("mov %%esp, %0":"=r" (data) );
    return data;
}

/*ritorna un selettore di segmento assemblato usando i dati passati*/
unsigned short segmentSelector(unsigned int index,char tableIndicator,char RPL)
{
    return ((index<<3)&0xFFF8)|((tableIndicator<<2)&0x4)|(RPL);
}

/*setta una riga della GDT (descrittore di segmento)*/
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

/*inizializza i segmenti di default*/
void initGdt(){
    gdtPointer.limit = (sizeof(struct gdtEntry) * NUMERO_SEGMENTI) - 1;
    gdtPointer.base = (int)&gdt;

    gdtSet(0, 0, 0, 0, 0); /*Il puntatore NULL!*/
    /*segmento codice kernel*/
    gdtSet(1, 0, (0xFFFFFFFF)/0x1000,MEM_GRANULAR|MEM_32,
        MEM_PRESENT|MEM_CODE_DATA|MEM_RW|MEM_KERNEL|MEM_CODE);
    /*segmento dati kernel*/
    gdtSet(2, 0, (0xFFFFFFFF)/0x1000,MEM_GRANULAR|MEM_32,
        MEM_PRESENT|MEM_CODE_DATA|MEM_RW|MEM_KERNEL|MEM_DATA);


    segmentoCodiceKernel=segmentSelector(1,0,RPL_KERNEL);
    segmentoDatiKernel=segmentSelector(2,0,RPL_KERNEL);
    segmentoCodiceUser=segmentSelector(3,0,RPL_USER);
    segmentoDatiUser=segmentSelector(4,0,RPL_USER);

    gdtFlush(segmentoCodiceKernel,segmentoDatiKernel);
}

/*######################### Paginazione ####################################*/

/* ritorna l'indirizzo che indica un selettore di pagetable o pagina */
inline unsigned int getFisicAdressFromSelector(unsigned int sel){
    return sel&0xFFFFF000;
}

/* ritorna l'indirizzo logico prendendo come parametri pagetable pagina e offset */
unsigned int virtualAdress(unsigned int table,unsigned int page,unsigned int offset){
    unsigned int temp;
    temp=offset&0xFFF;
    temp|=(page&0x3FF)<<12;
    temp|=table<<22;
    return temp;
}

/* ritornano le componenti dell indirizzo logico */
unsigned int getTableFromVirtualAdress(unsigned int adress){
    return (adress>>22)&0x3FF;
}
unsigned int getPageFromVirtualAdress(unsigned int adress){
    return (adress>>12)&0x3FF;
}
unsigned int getOffsetFromVirtualAdress(unsigned int adress){
    return adress&0xFFF;
}

void *calloc(unsigned int num, unsigned int size){
    char *retval=kmalloc(num*size);
    unsigned int i;
    for(i=0;i<(num*size)/sizeof(char);i++)
        retval[i]=0;
    return retval;
}


void* kmalloc(unsigned int byte){
    /*TODO:inserire controllo per non sforare nella memoria user*/
    struct memoryArea *pre,*next;

    if(kmallocList!=0)
    {
        
        /*controlla se vi è spazio prima del primo elemento*/        
        if(
            (unsigned int)kmallocList-mallocMemoryStart
            >= byte + sizeof(struct memoryArea) 
          )
        {
            /*printf("prima della lista %d\n", (unsigned int)kmallocList-mallocMemoryStart-sizeof(struct memoryArea));*/
            pre=(struct memoryArea*)mallocMemoryStart;
            next=(struct memoryArea*)kmallocList;
            pre->next=next;
            pre->size=byte;
            kmallocList=pre;

            return (void*)((unsigned int)pre+sizeof(struct memoryArea));
        }

        pre=kmallocList;
        next=pre->next;
        
        while(next!=0)
        {
            /*controlla lo spazio fra le due allocazioni*/
            if(
               ((unsigned int)next)-
               ((unsigned int)pre+sizeof(struct memoryArea)+pre->size)
               >= byte
              )
            {
                /*c'è spazio, alloca fra pre e next*/
                struct memoryArea *temp;
                pre->next=(struct memoryArea*)((unsigned int)pre+sizeof(struct memoryArea)+pre->size);
                temp=pre->next;
                temp->next=next;
                temp->size=byte;

                /*printf("in mezzo\n");*/

                return (void*)((unsigned int)temp+sizeof(struct memoryArea));
            }
            pre=next;
            next=next->next;
        }
        /*non è stato trovato spazio negli spazi liberi fra le allocazioni*/
        /*verrà quindi allocato alla fine della lista*/
        next=(struct memoryArea*)((unsigned int)pre+pre->size+sizeof(struct memoryArea));
        pre->next=next;
        next->next=0;
        next->size=byte;

        /*printf("alla fine\n");*/

        return (void*)((unsigned int)next+sizeof(struct memoryArea));
    }
    else
    {
        /*lista allocazioni vuota*/
        kmallocList=(struct memoryArea*)mallocMemoryStart;
        kmallocList->next=0;
        kmallocList->size=byte;

        /*printf("lista vuota %d\n",kmallocList);*/

        return (void*)((unsigned int)kmallocList+sizeof(struct memoryArea));
    }

}

void kfree(void *pointer){
    unsigned int tempPointer,tempPrePointer;

    /*scorre la lista cercando l'elemento*/
    tempPointer=(unsigned int)kmallocList;
    tempPrePointer=0;
    while(tempPointer!=0)
    {
        if(tempPointer+sizeof(struct memoryArea)==(unsigned int)pointer)
        {
            /*trovato, dealloca l'elemento*/
            /*printf("trovato :D\n");*/
            if(tempPrePointer==0)
            {
                /*è il primo elemento della lista*/
                kmallocList=(*(struct memoryArea*)tempPointer).next;
            }
            else
            {
                if((*(struct memoryArea*)tempPointer).next!=0)
                {
                    /*se non è l'ultimo elemento bypassalo*/
                    (*(struct memoryArea*)tempPrePointer).next=(*(struct memoryArea*)tempPointer).next;
                }
                else
                {
                    /*se è l'ultimo elemento metti a 0 il puntatore preedente*/
                    (*(struct memoryArea*)tempPrePointer).next=0;
                }
                    
            }
        }
        tempPrePointer=tempPointer;
        tempPointer=(unsigned int)(*(struct memoryArea*)tempPointer).next;
    }
}


void initPaging(void){
    unsigned int pointer,fisicPointer;
    int pageIndex,pageTableIndex;
    unsigned int addMem;
    char flags=PAG_PRESENT|PAG_READWRITE|PAG_SUPERVISOR|PAG_4KPAGE;
    int c=0;
    asm("cli");

    /*mappa fino a USER_START le pagin 1:1 con la memoria fisica*/
    fisicPointer=KERNEL_START;
    pageDir=(unsigned int*)KERNEL_MEMORY_START;
    pointer=(unsigned int)pageDir;


    mallocMemoryStart=(unsigned int)pageDir+(((1024*1024)+1)*4);/*indirizzo base dell heap*/
    addMem=(double)memoriaFisica/4.0;/*un quarto della memoria fisica e' riservata al kernel*/
    if(addMem<MIN_HEAP_SIZE)/*se un quarto della memoria non e' abbastanza e' meglio garantirsi un minimo*/
        addMem=MIN_HEAP_SIZE;
    /*printf("addMem: %d ",addMem);*/
    userMemoryStart=mallocMemoryStart+addMem;
    userMemoryStart+= 0x1000-(userMemoryStart%0x1000);


    for(c=0;c<(1024*1024)+1;c++)
        pageDir[c]=0;

    pageIndex=0;
    pageTableIndex=0;
    while(fisicPointer<userMemoryStart)
    {
        /*passa i selettori di pagina della pagetabla puntata da pointer*/
        setPageSelector((unsigned int*)(pointer+(pageIndex*4)),fisicPointer>>12,flags);
        pageIndex++;
        if(pageIndex==1024)
            pageIndex=0;
        fisicPointer+=0x1000;


        if(pageIndex==0)
        {
            /*se li ha passati tutti crea una nuova page table*/
            setPageTableSelector((unsigned int*)((unsigned int)pageDir+(pageTableIndex*4)),pointer>>12,flags);
            pointer+=0x1000; 
            pageTableIndex++;
        }
    }



    /*segmento codice user mode*/
    gdtSet(3, userMemoryStart, (0xFFFFFFFF-userMemoryStart)/0x1000,MEM_GRANULAR|MEM_32,
        MEM_PRESENT|MEM_CODE_DATA|MEM_RW|MEM_USER|MEM_CODE);
    /*segmento dati user mode*/
    gdtSet(4, userMemoryStart, (0xFFFFFFFF-userMemoryStart)/0x1000,MEM_GRANULAR|MEM_32,
        MEM_PRESENT|MEM_CODE_DATA|MEM_RW|MEM_USER|MEM_DATA);




    kmallocList=0;/*nessuna allocazione*/

    write_cr3((unsigned int)pageDir); /* put that page directory address into CR3 */
    write_cr0(read_cr0() | 0x80000000); /* set the paging bit in CR0 to 1 */

    asm("sti");


    /*inizializzazione gestione della memoria fisica*/
    pagesList=0;/*nessuna pagina per i processi allocata*/
    
}

/*setta il valore di un selettore*/
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



int getBitExt(unsigned int *bitmap,int x){
    int off1,off2;
    off1=x/32;
    off2=x%32;
    return (bitmap[off1]>>off2)&1;
}
void setBitExt(unsigned int *bitmap,int x,unsigned int value){
    int off1,off2;
    off1=x/32;
    off2=x%32;
    value&=0x1;
    if(value)
        bitmap[off1]|=1<<off2;
    else
        bitmap[off1]&=~(1<<off2);
}

