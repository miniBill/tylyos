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
#include <drivers/screen/screen.h>
#include <lib/string.h>

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
 ritorna l'indice in cui è inserito il selettore
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
    unsigned int c=1,i;/* la prima pagetable mappa il kernel quindi è inutile leggerla */
    unsigned int *pointer;
    /* scorre le pagetable */
    for(c=1;c<1023;c++)
        if(getFisicAdressFromSelector(pageDir[c])!=0x0){
            pointer=(unsigned int*)virtualAdress(c,1023,0);/* indirizzo pagetable */
            for(i=0;i<1023;i++)
                if(getFisicAdressFromSelector(pointer[i])==0x0){/* se è un selettore di pagina vuoto */
                    /* alloca spazio e setta il selettore */
                    setPageSelector(&pointer[i], getNewPage(1)>>12 ,flags);
                    /* azzera la bitmap che indica le allocazioni nella pagina */
                    writeBitmapOnPage((unsigned int*)virtualAdress(c,i,0));
                    return virtualAdress(c,i,0);
                }
        }
    /* se si arriva quì vuol dire che in nessuna pagetable c'è spazio per una nuova pagina */
    /* crea nuova pagetable */
    c=addNewPageTable(flags);
    /* crea nuova pagina */
    pointer=(unsigned int*)virtualAdress(c,0,1023);
    setPageSelector(&pointer[0], getNewPage(1)>>12 ,flags);
    /* azzera la bitmap che indica le allocazioni nella pagina */
    writeBitmapOnPage((unsigned int*)virtualAdress(c,0,0));
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
        if(getFisicAdressFromSelector(page[c])!=0x0)/* se è una pagina allocata */
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
    unsigned int bitmapSize;
    unsigned int c=1,i,i2,i3;/* la prima pagetable mappa il kernel quindi è inutile leggerla */
    unsigned int *pointer,*pointer2;
    unsigned int flag,ret,bitCounter;

     /* ricavo la dimensione in byte della bitmap */
    bitmapSize=(0x1000-128)/8/MIN_SIZE_ALLOCABLE;

    if(byte<0x1000-bitmapSize){/* se il numero di byte da allocare stanno in una pagina */
        /* passa le pagetable */
        for(c=1;c<1023;c++)
            if(getFisicAdressFromSelector(pageDir[c])!=0x0){
                /* indirizzo pagetable */
                pointer=(unsigned int*)virtualAdress(c,1023,0);
                /* passa le pagine */
                for(i=0;i<1023;i++)
                    if(getFisicAdressFromSelector(pointer[i])!=0x0 && !(c==1 && (i==0 || i==1))){/* se non è una pagina vuota */
                        pointer2=(unsigned int*)virtualAdress(c,i,0);
                        flag=0;
                        bitCounter=0;
                        /* passa la bitmap cercando una sequenza di bit pari a zero abbastanza lunga */
                        for(i2=0;i2<bitmapSize*8;i2++){
                            if(getBitExt(pointer2,i2)==0){/* se il bit è 0 sett il flag e incrementa il bitCounter */
                                if(flag==0){
                                    flag=1;
                                    ret=i2;
                                }
                                bitCounter++;
                                if(byte<=bitCounter*4){/* se lo spazio è abbastanza grosso */
                                    /* alloca */
                                    for(i3=0;i3<bitCounter;i3++){
                                        setBitExt(pointer2,ret+i3,1);
                                    }

                                    /* ritorna l'indirizzo appena allocato */
                                    return (void*)virtualAdress(c,i,(unsigned int)(ret*MIN_SIZE_ALLOCABLE)+bitmapSize);
                                }
                            }else{/* se è a 1 resetta il flag e il bitCounter */
                                flag=0;
                                bitCounter=0;
                                ret=0;
                            }
                        }
                    }
                c++;
        }
        /* se arriva quì non c'è spazio in nessuna pagina allocata */
        /* alloca nuova pagina */
        pointer=(unsigned int*)addNewPage(PAG_PRESENT|PAG_READWRITE|PAG_SUPERVISOR|PAG_4KPAGE);
        /* alloca e ritorna il puntatore */
        i=byte/MIN_SIZE_ALLOCABLE;
        if(byte%MIN_SIZE_ALLOCABLE>0)/* approssima per eccesso */
         i++;
        for(i3=0;i3<i;i3++){
            setBitExt(pointer,i3,1);
        }
        return (void*)virtualAdress(getTableFromVirtualAdress((unsigned int)pointer),getPageFromVirtualAdress((unsigned int)pointer),bitmapSize);
    }else{/* non è possibile allocare un numero di byte contigui di questa dimensione, forse da sistemare in futuro */}
    return (void*)0;
}

void free(void *pointer){/* TODO: implementare */
    deletePage((unsigned int)pointer);
}

/*
 scrive la bitmap in una pagina e inizializza tutti i bit a zero
*/
void writeBitmapOnPage(unsigned int* adress){
    unsigned int bitmapSize,c;
    /* ricavo la dimensione in byte della bitmap */
    bitmapSize=0x1000/8/MIN_SIZE_ALLOCABLE;
    /* azzera tutti i bit */
    for(c=0;c<bitmapSize/4;c++){
        adress[c]=0;
    }
}

void initPaging(void){
    int c,c2;
    unsigned int pageTab,pageTab2;
    char debug[36]={0};
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
        debug[0]='\0';
        /* strapp(debug,"descrittore in: %x",(void *)(pageTab+(c*4)));
        strapp(debug,"indirizzo: %x",(void *)((c*0x1000)>>12));
         writeline(debug); */
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
    /* setta i tre selettori */
    setPageSelector((unsigned int*)((pageTab2)+(0*4)),(unsigned int)pageDir>>12,flags);
    

     /*setta un selettore utile per un indirizzamento temporaneo (da modificare al volo per modificare specifici indirizzi fisici)*/
    tempPageSelector=getNewPage(1);
    setPageSelector((unsigned int*)((pageTab2)+(1*4)),tempPageSelector>>12,flags);
    /* quindi il rispettivo indirizzo dopo l'attivazione della paginazione sarà */
    tempPageSelector=virtualAdress(1,1023,1*4);
    /* l'indirizzo che prenderà l'area del selettore temporaneo */
    tempPage=(unsigned int*)virtualAdress(1,1,0);

    write_cr3((unsigned int)pageDir); /* put that page directory address into CR3 */
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

