/* Copyright (C) 2008 Luca Salmin
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
#include "elf.h"
#include <drivers/screen/screen.h>
#include <fs/fs.h>
#include <lib/string.h>
#include <kernel/stdio.h>
#include <memory/memory.h>
#include <task/task.h>

void loader_checkHeader(char *path)
{
    unsigned int dimensione;
    Elf32_Ehdr *header1;
    File file=openFile(path,'r');
    
    if(file==0)
        return;
    
    dimensione=fileSize(file);
    
    printf(2,"file aperto: %d Bytes\n",dimensione);
    
    char *buffer=kmalloc(dimensione);
    
    readFile(file,buffer,dimensione);
    
    printf(2,"inizio parsing...\n");
    header1=(Elf32_Ehdr*)buffer;
    if(
            header1->e_ident[EI_MAG0] == ELFMAG0 &&
            header1->e_ident[EI_MAG1] == ELFMAG1 && /*E*/
            header1->e_ident[EI_MAG2] == ELFMAG2 && /*L*/
            header1->e_ident[EI_MAG3] == ELFMAG3 && /*F*/
            
            header1->e_ident[EI_CLASS] == ELFCLASS32 && /*32bit*/
            header1->e_ident[EI_DATA] == ELFDATA2LSB /*tipo di codifica*/
       )
    {
        printf(2,"formato verificato\n\n");
        
        //scorre la program table
        Elf32_Phdr *header2=(Elf32_Phdr*)&buffer[header1->e_phoff];
        for(int c=0;c<header1->e_phnum;c++)
        {
            if(header2[c].p_type==PT_NOTE)
            {
                printf(2,"[section] note section\n");
            }
            else if(header2[c].p_type==PT_DYNAMIC)
            {
                printf(2,"[section] dinamic linking information\n");
            }
            else if(header2[c].p_type==PT_LOAD)
            {
                printf(2,"[section] loadable segment\n");
                if(header2[c].p_flags & PF_R)
                    printf(2,"    flags: r");
                else
                    printf(2,"    flags: -");
                if(header2[c].p_flags & PF_W)
                    printf(2,"w");
                else
                    printf(2,"-");
                if(header2[c].p_flags & PF_X)
                    printf(2,"x\n");
                else
                    printf(2,"-\n");
                printf(2,"    vaddr: 0x%x\n",header2[c].p_vaddr);
                printf(2,"    file size: %d Bytes\n",header2[c].p_filesz);
                printf(2,"    segment size: %d Bytes\n",header2[c].p_memsz);
            }
            else if(header2[c].p_type==PT_NULL)
            {
                printf(2,"[section] NULL\n");
            }
            else if(header2[c].p_type==PT_PHDR)
            {
                printf(2,"[section] program header\n");
            }
            else if(header2[c].p_type==PT_INTERP)
            {
                printf(2,"[section] interprete \n    path: %s\n",&buffer[header2[c].p_offset]);
            }
            else
            {
                printf(2,"[section] Unknown, type: %x\n",header2[c].p_type);
            }
        }
        
    }
    else
    {
        printf(2,"il file non e' un elf\n");
        return;
    }
}

/*funzione che carica nella memoria di un task i relativi dati leggendoli da un file elf*/
loader_returnCode loader_loadElf(char *path,int procId)
{
    
    struct taskStruct *t=getTask(procId);
    
    /*setta i selettori di segmento nel TSS*/
    t->TSS.cs=segmentoCodiceUser;/*selettori default per i task, vengono modificati i descrittori prima di ogni switch*/
    t->TSS.ds=segmentoDatiUser; 
    t->TSS.ss=segmentoCodiceUser;
    
    t->TSS.ss0=segmentoDatiKernel;/*esp0 usa un indirizzo relativo a questo segmneto*/
    t->TSS.esp0=(unsigned int)stackKernel+KERNEL_STACK_SIZE-1;/*lo stack usato per il codice a ring0, a quanto pare viene resettato ogni volta in quanto mnca ebp0*/
    
    unsigned int dimensione;
    Elf32_Ehdr *header1;
    File file=openFile(path,'r');
    
    if(file==0)
        return LOADER_NOT_FOUND;
    
    dimensione=fileSize(file);
    
    char *buffer=kmalloc(dimensione);
    
    readFile(file,buffer,dimensione);
    
    header1=(Elf32_Ehdr*)buffer;
    /*controlla che sia un elf valido*/
    if(
        header1->e_ident[EI_MAG0] == ELFMAG0 &&
        header1->e_ident[EI_MAG1] == ELFMAG1 && /*E*/
        header1->e_ident[EI_MAG2] == ELFMAG2 && /*L*/
        header1->e_ident[EI_MAG3] == ELFMAG3 && /*F*/
        
        header1->e_ident[EI_CLASS] == ELFCLASS32 && /*32bit*/
        header1->e_ident[EI_DATA] == ELFDATA2LSB /*tipo di codifica*/
        )
    {    
        t->TSS.eip=header1->e_entry;
        printf(1,"entry: 0x%x\n",header1->e_entry);
        //scorre la program table
        Elf32_Phdr *header2=(Elf32_Phdr*)&buffer[header1->e_phoff];
        for(int c=0;c<header1->e_phnum;c++)
        {
            if(header2[c].p_type==PT_NOTE)
            {
                /*note section*/
            }
            else if(header2[c].p_type==PT_DYNAMIC)
            {
                /*dinamic linking information*/
            }
            else if(header2[c].p_type==PT_LOAD)
            {
                /*loadable segment*/
                /*mappa l'area nel processo e copia i dati dentro*/
                allocMemory(procId,header2[c].p_vaddr,header2[c].p_memsz);
                memcpyToTask(&buffer[header2[c].p_offset],header2[c].p_filesz,(char*)header2[c].p_vaddr,procId);  
                

                
                if( (header2[c].p_flags & PF_R) && (header2[c].p_flags & PF_W) )
                {
                    /*segmento dati*/
                    t->dataSegmentBase=header2[c].p_vaddr;
                    t->dataSegmentSize=header2[c].p_memsz+TASK_STACK_SIZE;/*aggiunge lo stack in fondo*/
                    t->stackSegmentBase=t->dataSegmentBase;
                    t->stackSegmentSize=t->dataSegmentSize;
                    /*a quanto pare lo stack va verso il basso nell architettura intel*/
                    t->TSS.esp=header2[c].p_vaddr+TASK_STACK_SIZE-1;
                    t->TSS.ebp=header2[c].p_vaddr+TASK_STACK_SIZE-1;
                }
              
                if( (header2[c].p_flags & PF_R) && (header2[c].p_flags & PF_X) )
                {
                    /*segmento codice*/
                    t->codeSegmentBase=header2[c].p_vaddr;
                    t->codeSegmentSize=header2[c].p_memsz;
                }
              
            }
            else if(header2[c].p_type==PT_NULL)
            {
                /*NULL*/
            }
            else if(header2[c].p_type==PT_PHDR)
            {
                /*program header*/
            }
            else if(header2[c].p_type==PT_INTERP)
            {
                /*interprete*/
            }
            else
            {
                /*Unknown type*/
            }
        }
        
    }
    else
    {
        return LOADER_BAD_FORMAT;
    }
    
    return LOADER_OK;
}
