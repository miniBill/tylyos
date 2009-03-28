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
    gdtSet(1, 0, 0xFFFFFFFF,MEM_GRANULAR|MEM_32,
        MEM_PRESENT|MEM_CODE_DATA|MEM_RW|MEM_KERNEL|MEM_CODE);
    /*segmento dati kernel*/
    gdtSet(2, 0, 0xFFFFFFFF,MEM_GRANULAR|MEM_32,
        MEM_PRESENT|MEM_CODE_DATA|MEM_RW|MEM_KERNEL|MEM_DATA);

    /*segmento codice user mode*/
    gdtSet(3, MEMORY_START, 0xFFFFFFFF-MEMORY_START,MEM_GRANULAR|MEM_32,
        MEM_PRESENT|MEM_CODE_DATA|MEM_RW|MEM_USER|MEM_CODE);
    /*segmento dati user mode*/
    gdtSet(4, MEMORY_START, 0xFFFFFFFF-MEMORY_START,MEM_GRANULAR|MEM_32,
        MEM_PRESENT|MEM_CODE_DATA|MEM_RW|MEM_USER|MEM_DATA);

    segmentoCodiceKernel=0x08;
    segmentoDatiKernel=0x10;
    gdtFlush(segmentoCodiceKernel,segmentoDatiKernel);
}

/*######################### Paginazione ####################################*/

/*
 ritorna un indirizzo fisico per l'allocazione di una nuova pagina
 alloca: indica se segnare questo indirizzo come utilizzato
*/
unsigned int getNewPage(int alloca){
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
    char *retval=malloc(num*size);
    unsigned int i;
    for(i=0;i<(num*size)/sizeof(char);i++)
        retval[i]=0;
    return retval;
}

/*
 alloca una nuova pagetable e la inserisce nella pagedir
 ritorna l'indice in cui � inserito il selettore
*/
unsigned int addNewPageTable(unsigned int flags){/* TODO: provare se funziona */
    unsigned int c=0,c2;
    unsigned int *pointer;
    /* scorre fino a trovare dove inserire il nuovo selettore */
    while(getFisicAdressFromSelector(pageDir[c])!=0x0)
        c++;
    /* alloca spazio */
    pointer=(unsigned int*)getNewPage(1);
    setPageTableSelector(&pageDir[c], (unsigned int)pointer>>12 ,flags);
    /* mi serve scrivere il 1023 record per mappare se stessa altrimenti non sarebbe indirizabile  */
    /* quindi setto il selettore di pagina temporaneo in modo che punti ai dati appena allocati per la tabella */
    setPageSelector((unsigned int*)tempPageSelector, (unsigned int)pointer>>12 ,flags);

    /* in questi dati poi modifico il 1024 record in modo che sia utilizzabile per indirizzarsi */
    setPageSelector(&tempPage[1023], (unsigned int)pointer>>12 ,flags);
    for(c2=0;c2<1023;c2++){
        /* azzera i selettori che contiene */
        setPageSelector(&tempPage[c2],0,0);
    }
    return c;
}

/* alloca una nuova pagina e ritorna l'indirizzo logico */
unsigned int addNewPage(unsigned int flags){
    unsigned int c=1,i;/* la prima pagetable mappa il kernel quindi � inutile leggerla */
    unsigned int *pointer;
    /* scorre le pagetable */
    for(c=1;c<1023;c++)
        if(getFisicAdressFromSelector(pageDir[c])!=0x0){
            pointer=(unsigned int*)virtualAdress(c,1023,0);/* indirizzo pagetable */
            for(i=0;i<1023;i++)
                if(getFisicAdressFromSelector(pointer[i])==0x0){/* se � un selettore di pagina vuoto */
                    /* alloca spazio e setta il selettore */
                    setPageSelector(&pointer[i], getNewPage(1)>>12 ,flags);
                    return virtualAdress(c,i,0);
                }
        }
    /* se si arriva qu� vuol dire che in nessuna pagetable c'� spazio per una nuova pagina */
    /* crea nuova pagetable */
    c=addNewPageTable(flags);
    /* crea nuova pagina */
    pointer=(unsigned int*)virtualAdress(c,0,1023);
    setPageSelector(&pointer[0], getNewPage(1)>>12 ,flags);

    return virtualAdress(c,0,0);
}

/*
 dealloca una pagina
 BaseAdress: indirizzo logico di inizio pagina
*/
void deletePage(unsigned int BaseAdress){
    unsigned int table,page,fisicAdress,*selectorAdress,*temp;
    /* scompongo l'indirizzo */
    table=getTableFromVirtualAdress(BaseAdress);
    page=getPageFromVirtualAdress(BaseAdress);
    /* prendo l'indirizzo fisico contenuto nel selettore */
    temp=(unsigned int*)virtualAdress(table,1023,page*4);
    fisicAdress=getFisicAdressFromSelector(*temp);
    /* setto a 0 il bit corrispondente a quell indirizzo nella bitmap */
    setBit( (fisicAdress-MEMORY_START)/0x1000 ,0);
    /* cancello il selettore della pagina */
    selectorAdress=(unsigned int*)virtualAdress(table,1023,page*4);
    setPageSelector(selectorAdress,0,0);
}

/*
 dealloca una pagetable
 num: indice della pagetable nella pagedir
*/
void deletePageTable(unsigned int num){/* TODO: provare se funziona */
    unsigned int *selector,*page,fisicAdress,*temp,c;
    /* trovo gli indirizzi del selettore e della pagina che la contiene */
    page=(unsigned int*)virtualAdress(num,1023,0);
    selector=(unsigned int*)&pageDir[num];
    /* passo tutti i descrittori che contiene e se sono allocate pagine le dealloco */
    for(c=0;c<1023;c++)
        if(getFisicAdressFromSelector(page[c])!=0x0)/* se � una pagina allocata */
            deletePage(virtualAdress(num,c,0));
    /* dealloco lo spazio per la tabella */
    /* prendo l'indirizzo fisico contenuto nel selettore */
    temp=(unsigned int*)virtualAdress(num,1023,4*1023);
    fisicAdress=getFisicAdressFromSelector(*temp);
    /* setto a 0 il bit corrispondente a quell indirizzo nella bitmap */
    setBit( (fisicAdress-MEMORY_START)/0x1000 ,0);
    /* cancello il selettore */
    setPageTableSelector(selector,0,0);
}

void* malloc(unsigned int byte){
    /*TODO:implementare*/
    unsigned int *pointer;
    writeline(">>WARNING: malloc per il kernel non implementata");
    byte=0;
    /* alloca nuova pagina */
    pointer=(unsigned int*)addNewPage(PAG_PRESENT|PAG_READWRITE|PAG_SUPERVISOR|PAG_4KPAGE);
    return (void*)pointer;
}

void free(void *pointer,unsigned int size){
    /*TODO:implementare*/
    writeline(">>WARNING: free per il kernel non implementata");
    size=0;
    deletePage((unsigned int)pointer);
}


void initPaging(void){
    int c,c2;
    unsigned int pageTab,pageTab2;
    char flags=PAG_PRESENT|PAG_READWRITE|PAG_SUPERVISOR|PAG_4KPAGE;
    asm("cli");
    /* azzera mappa */
    for(c=0;c<MAX_PAGES_IN_MEMORY/32+1;c++)
        memoryBitmap[c]=0;

    /* azzera i record della pagedir */
    for(c=0;c<1024;c++)
        setPageTableSelector(&pageDir[c],0,0);

    pageDir=(unsigned int*)getNewPage(1);
    pageTab=getNewPage(1);
    pageTab2=getNewPage(1);

    /* setta la prima pagetable nella pagedir */
    setPageTableSelector(&pageDir[0],pageTab>>12,flags);
    /* ultimo record punta alla pagina della pagedir */
    setPageTableSelector(&pageDir[1023],(unsigned int)pageDir>>12,flags);
    /* inserisce pagine nella prima pagetable */
    for(c2=0,c=KERNEL_START/0x1000;c<1024;c++,c2++){
        /* printf("descrittore in: 0x%x indirizzo: 0x%x",pageTab+(c*4),(c*0x1000)>>12);*/
        setPageSelector((unsigned int*)((pageTab)+(c2*4)),(c*0x1000)>>12,flags);
    }
    /* ultimo record punta alla pagina della tabella */
    setPageSelector((unsigned int*)(pageTab+(1023*4)),pageTab>>12,flags);


    /* mappa altre tre pagine che sono state usate per la pagedir e le pagetable */
    /* setta la seconda pagetable nella pagedir */
    setPageTableSelector(&pageDir[1],pageTab2>>12,flags);
    for(c=0;c<1024;c++)/* azzera */
        setPageSelector((unsigned int*)((pageTab2)+(c*4)),0,0);

    /* ultimo record punta alla pagina della tabella */
    setPageSelector((unsigned int*)(pageTab2+(1023*4)),pageTab2>>12,flags);
    /* setta il selettore della pagedir */
    setPageSelector((unsigned int*)((pageTab2)+(0*4)),(unsigned int)pageDir>>12,flags);
    

     /*setta un selettore utile per un indirizzamento temporaneo (da modificare al volo per modificare specifici indirizzi fisici)*/
    tempPageSelector=getNewPage(1);
    setPageSelector((unsigned int*)((pageTab2)+(1*4)),tempPageSelector>>12,flags);
    /* quindi il rispettivo indirizzo dopo l'attivazione della paginazione sar� */
    tempPageSelector=virtualAdress(1,1023,1*4);
    /* l'indirizzo che prender� l'area del selettore temporaneo */
    tempPage=(unsigned int*)virtualAdress(1,1,0);
    write_cr3((unsigned int)pageDir); /* put that page directory address into CR3 */
    write_cr0(read_cr0() | 0x80000000); /* set the paging bit in CR0 to 1 */
    asm("sti");
}

/*setta il valore di un selettore*/
/* obj: indirizzo dell'area su cui scrivere il selettore
 * TableAdress: indirizzo della tabella nella memoria (NB: una tabella occupa una pagina da 4K
 * questo indirizzo indica i 20 bit pi� significativi dell indirizzo della tabella)
 */
void setPageTableSelector(unsigned int *obj,unsigned int tableAdress,unsigned int flags){
    *obj=0;
    *obj=flags;
    *obj|=tableAdress<<12;
}

/* obj: indirizzo dell'area su cui scrivere il selettore
 * TableAdress: indirizzo della pagina nella memoria (NB: una pagina occupa 4K
 * questo indirizzo indica i 20 bit pi� significativi dell indirizzo)
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

