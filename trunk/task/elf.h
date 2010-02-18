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


typedef unsigned int Elf32_Addr;/*unsigned program adress*/
typedef unsigned short Elf32_Half;/*unsigned medium integer*/
typedef unsigned int Elf32_Off;/*unsigned file offset*/
typedef int Elf32_Sword;/*signed large integer*/
typedef unsigned int Elf32_Word;/*unsigned large integer*/

#define ET_NONE 0 /*No file type*/
#define ET_REL 1 /*Relocatable file*/
#define ET_EXEC 2 /*Executable file*/
#define ET_DYN 3 /* Shared object file*/
#define ET_CORE 4 /*Core file*/
#define ET_LOPROC 0xff00 /*Processor-specific*/
#define ET_HIPROC 0xffff /*Processor-specific*/

#define EM_NONE 0 /*No machine*/
#define EM_M32 1 /*AT&T WE 32100*/
#define EM_SPARC 2 /*SPARC*/
#define EM_386 3 /*Intel 80386*/
#define EM_68K  4 /*Motorola 68000*/
#define EM_88K 5 /*Motorola 88000*/
#define EM_860 7 /*Intel 80860*/
#define EM_MIPS 8 /*MIPS RS3000*/


#define EV_NONE 0 /*Invalid version*/
#define EV_CURRENT 1 /*Current version*/


#define EI_MAG0 0 /*File identification*/
#define EI_MAG1 1 /*File identification*/
#define EI_MAG2 2 /*File identification*/
#define EI_MAG3 3 /*File identification*/
#define EI_CLASS 4 /*File class*/
#define EI_DATA 5 /*Data encoding*/
#define EI_VERSION 6 /*File version*/
#define EI_PAD 7 /*Start of padding bytes*/

#define ELFMAG0 0x7f /*e_ident[EI_MAG0]*/
#define ELFMAG1 'E' /*e_ident[EI_MAG1]*/
#define ELFMAG2 'L' /*e_ident[EI_MAG2]*/
#define ELFMAG3 'F' /*e_ident[EI_MAG3]*/

#define ELFCLASSNONE 0 /*Invalid class*/
#define ELFCLASS32 1 /*32-bit objects*/
#define ELFCLASS64 2 /*64-bit objects*/

#define ELFDATANONE 0 /*Invalid data encoding*/
#define ELFDATA2LSB 1
#define ELFDATA2MSB 2


#define EI_NIDENT     16

typedef struct
{
    unsigned char e_ident[EI_NIDENT];
    Elf32_Half    e_type;
    Elf32_Half    e_machine;
    Elf32_Word    e_version;
    Elf32_Addr    e_entry;
    Elf32_Off     e_phoff;
    Elf32_Off     e_shoff;
    Elf32_Word    e_flags;
    Elf32_Half    e_ehsize;
    Elf32_Half    e_phentsize;
    Elf32_Half    e_phnum;
    Elf32_Half    e_shentsize;
    Elf32_Half    e_shnum;
    Elf32_Half    e_shstrndx;
} Elf32_Ehdr;

void loader_checkHeader(char *path);
