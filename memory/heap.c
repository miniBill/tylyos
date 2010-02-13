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

#include "heap.h"
#include <memory/memory.h>
#include <kernel/kernel.h>

void *calloc ( unsigned int num, unsigned int size )
{
    char *retval=kmalloc ( num*size );
    unsigned int i;
    for ( i=0;i< ( num*size ) /sizeof ( char );i++ )
        retval[i]=0;
    return retval;
}


void* kmalloc ( unsigned int byte )
{
    /*TODO:verificare che il controllo per non sforare nella memoria user funzioni*/
    struct memoryArea *pre,*next;
    
    if ( kmallocList!=0 )
    {
        
        /*controlla se vi e' spazio prima del primo elemento*/
        if (
            ( unsigned int ) kmallocList-HEAP_START
            >= byte + sizeof ( struct memoryArea )
            )
        {
            /*printf("prima della lista %d\n", (unsigned int)kmallocList-mallocMemoryStart-sizeof(struct memoryArea));*/
            pre= ( struct memoryArea* ) HEAP_START;
            next= ( struct memoryArea* ) kmallocList;
            pre->next=next;
            pre->size=byte;
            kmallocList=pre;
            
            return ( void* ) ( ( unsigned int ) pre+sizeof ( struct memoryArea ) );
        }
        
        pre=kmallocList;
        next=pre->next;
        
        while ( next!=0 )
        {
            /*controlla lo spazio fra le due allocazioni*/
            if (
                ( ( unsigned int ) next )-
                ( ( unsigned int ) pre+sizeof ( struct memoryArea ) +pre->size )
                >= byte
                )
            {
                /*c'e' spazio, alloca fra pre e next*/
                struct memoryArea *temp;
                pre->next= ( struct memoryArea* ) ( ( unsigned int ) pre+sizeof ( struct memoryArea ) +pre->size );
                temp=pre->next;
                temp->next=next;
                temp->size=byte;
                
                /*printf("in mezzo\n");*/
                
                return ( void* ) ( ( unsigned int ) temp+sizeof ( struct memoryArea ) );
            }
            pre=next;
            next=next->next;
        }
        /*non e' stato trovato spazio negli spazi liberi fra le allocazioni*/
        /*verra' quindi allocato alla fine della lista*/
              
        next= ( struct memoryArea* ) ( ( unsigned int ) pre+pre->size+sizeof ( struct memoryArea ) );
        /*controlla di non sforare*/
        if ( ( unsigned int ) next+sizeof ( struct memoryArea ) +byte >= ( unsigned int ) heapEndPointer )
        {
            if(sizeof ( struct memoryArea ) + byte < 0x1000)
            {
                increaseHeapSize();
            }
            else
                kernelPanic ( "kmalloc()","l' allocazione e' troppo grande" );
        }
        
        pre->next=next;
        next->next=0;
        next->size=byte;
        
        /*printf("alla fine\n");*/
        
        return ( void* ) ( ( unsigned int ) next+sizeof ( struct memoryArea ) );
        
    }
    else
    {
        /*lista allocazioni vuota*/
        kmallocList= ( struct memoryArea* ) HEAP_START;
        kmallocList->next=0;
        kmallocList->size=byte;
        
        /*printf("lista vuota %d\n",kmallocList);*/
        
        return ( void* ) ( ( unsigned int ) kmallocList+sizeof ( struct memoryArea ) );
    }
    return 0;/*in ogni caso qui' non ci deve arrivare*/
}

void kfree ( void *pointer )
{
    unsigned int tempPointer,tempPrePointer;
    
    /*scorre la lista cercando l'elemento*/
    tempPointer= ( unsigned int ) kmallocList;
    tempPrePointer=0;
    while ( tempPointer!=0 )
    {
        if ( tempPointer+sizeof ( struct memoryArea ) == ( unsigned int ) pointer )
        {
            /*trovato, dealloca l'elemento*/
            /*printf("trovato :D\n");*/
            if ( tempPrePointer==0 )
            {
                /*� il primo elemento della lista*/
                kmallocList= ( * ( struct memoryArea* ) tempPointer ).next;
                return;
            }
            else
            {
                if ( ( * ( struct memoryArea* ) tempPointer ).next!=0 )
                {
                    /*se non � l'ultimo elemento bypassalo*/
                    ( * ( struct memoryArea* ) tempPrePointer ).next= ( * ( struct memoryArea* ) tempPointer ).next;
                    return;
                }
                else
                {
                    /*se � l'ultimo elemento metti a 0 il puntatore preedente*/
                    ( * ( struct memoryArea* ) tempPrePointer ).next=0;
                    return;
                }
                
            }
        }
        tempPrePointer=tempPointer;
        tempPointer= ( unsigned int ) ( * ( struct memoryArea* ) tempPointer ).next;
    }
    kernelPanic ( "kfree()","a kernel function is trying to deallocate something but i can't find it in the allocation list, sorry." );
}

void increaseHeapSize()
{
    /*TODO: inserire il controllo per la dimensione massima dell heap*/
    unsigned int nuovaPaginaFisica=getFreePage();
    
    if(nuovaPaginaFisica==0)
        kernelPanic("increaseHeapSize()","I need a free page...");
    else
        setPaginaFisica(nuovaPaginaFisica,1);/*setta la pagina fisica come utilizzata*/
    
    setPageSelector(
                        getTableFromVirtualAdress((unsigned int)heapEndPointer),
                        getPageFromVirtualAdress((unsigned int)heapEndPointer),
                        nuovaPaginaFisica,
                        PAG_PRESENT|PAG_READWRITE|PAG_SUPERVISOR|PAG_4KPAGE
                   );
                   
    invalidateLookasideBuffer();
    
    heapEndPointer+=0x1000;
}

void decreaseHeapSize()
{
    heapEndPointer-=0x1000;
    
    unsigned int paginaFisica = fisicAdressFromSelectorExt(
                       getTableFromVirtualAdress((unsigned int)heapEndPointer),
                       getPageFromVirtualAdress((unsigned int)heapEndPointer)
                                                          );
    setPaginaFisica(paginaFisica,0);/*setta la pagina fisica come non utilizzata*/
    
    setPageSelector(
                        getTableFromVirtualAdress((unsigned int)heapEndPointer),
                        getPageFromVirtualAdress((unsigned int)heapEndPointer),
                        0,
                        PAG_NOTPRESENT
                   );
                    
   invalidateLookasideBuffer();
}

unsigned int getHeapSize()
{
    return ((unsigned int)heapEndPointer) - HEAP_START;
}
