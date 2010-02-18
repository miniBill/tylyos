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
    header1=buffer;
    if(
            header1->e_ident[EI_MAG0] == ELFMAG0 &&
            header1->e_ident[EI_MAG1] == ELFMAG1 &&
            header1->e_ident[EI_MAG2] == ELFMAG2 &&
            header1->e_ident[EI_MAG3] == ELFMAG3
       )
    {
        printf(2,"formato verificato\n");
    }
    else
    {
        printf(2,"il file non e' un elf\n");
        return;
    }
}