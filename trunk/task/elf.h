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
    Elf32_Off     e_shoff;/*posizione del section header*/
    Elf32_Word    e_flags;
    Elf32_Half    e_ehsize;
    Elf32_Half    e_phentsize;
    Elf32_Half    e_phnum;
    Elf32_Half    e_shentsize;/*dimensione in bytes di ogni songola entry del section header*/
    Elf32_Half    e_shnum;/*numero di entries nel section header*/
    Elf32_Half    e_shstrndx;
} Elf32_Ehdr;

typedef struct
{
    Elf32_Word sh_name;/*This member specifies the name of the section. Its value is an index into the section header string table section*/
    Elf32_Word sh_type;/*This member categorizes the section’s contents and semantics.*/    
    Elf32_Word sh_flags;/*Sections support 1-bit flags that describe miscellaneous attributes.*/
    Elf32_Addr sh_addr;/*If the section will appear in the memory image of a process, this member gives the address at which the section’s first byte should reside. Otherwise, the member contains 0.*/
    Elf32_Off  sh_offset;/*This member’s value gives the byte offset from the beginning of the file to the first byte in the section.*/
    Elf32_Word sh_size;/*This member gives the section’s size in bytes. Unless the section type is SHT_NOBITS, the section occupies sh_size bytes in the file. A section of type SHT_NOBITS may have a non-zero size, but it occupies no space in the file.*/
    Elf32_Word sh_link;/*This member holds a section header table index link, whose interpretation depends on the section type.*/
    Elf32_Word sh_info;/*This member holds extra information, whose interpretation depends on the section type.*/
    Elf32_Word sh_addralign;/*Some sections have address alignment constraints. For example, if a section holds a doubleword, the system must ensure doubleword alignment for the entire section.
                                That is, the value of sh_addr must be congruent to 0, modulo the value of
                                sh_addralign. Currently, only 0 and positive integral powers of two are allowed.
                                Values 0 and 1 mean the section has no alignment constraints.*/
    Elf32_Word sh_entsize;/*Some sections hold a table of fixed-size entries, such as a symbol table. For such a section, this member gives the size in bytes of each entry.
                                The member contains 0 if the section does not hold a table of fixed-size entries.*/
} Elf32_Shdr;

/*Section Types, sh_type*/
#define SHT_NULL        0
#define SHT_PROGBITS    1
#define SHT_SYMTAB      2
#define SHT_STRTAB      3
#define SHT_RELA        4
#define SHT_HASH        5
#define SHT_DYNAMIC     6
#define SHT_NOTE        7
#define SHT_NOBITS      8
#define SHT_REL         9
#define SHT_SHLIB       10
#define SHT_DYNSYM      11
#define SHT_LOPROC      0x70000000
#define SHT_HIPROC      0x7fffffff
#define SHT_LOUSER      0x80000000
#define SHT_HIUSER      0xffffffff

/*Section Attribute Flags, sh_flags*/
#define SHF_WRITE       0x1/*The section contains data that should be writable during process execution.*/
#define SHF_ALLOC       0x2/*The section occupies memory during process execution. Some control sections do not reside in the memory image of an object file; this attribute is off for those sections.*/
#define SHF_EXECINSTR   0x4/*The section contains executable machine instructions.*/
#define SHF_MASKPROC    0xf0000000/*All bits included in this mask are reserved for processor-specific semantics.*/

typedef struct {
    Elf32_Word p_type;/*This member tells what kind of segment this array element describes or how to interpret the array element’s information.*/
    Elf32_Off  p_offset;/*This member gives the offset from the beginning of the file at which the first byte of the segment resides.*/
    Elf32_Addr p_vaddr;/*This member gives the virtual address at which the first byte of the segment resides in memory.*/
    Elf32_Addr p_paddr;/*On systems for which physical addressing is relevant, this member is reserved for the
                            segment’s physical address. Because System V ignores physical addressing for application programs,
                            this member has unspecified contents for executable files and shared objects.*/
    Elf32_Word p_filesz;/*This member gives the number of bytes in the file image of the segment; it may be zero*/
    Elf32_Word p_memsz;/*This member gives the number of bytes in the memory image of the segment; it may be zero.*/
    Elf32_Word p_flags;/*This member gives flags relevant to the segment.*/
    Elf32_Word p_align;/*loadable process segments must have
                            congruent values for p_vaddr and p_offset, modulo the page size. This member
                            gives the value to which the segments are aligned in memory and in the file. Values 0
                            and 1 mean no alignment is required. Otherwise, p_align should be a positive, integral
                            power of 2, and p_vaddr should equal p_offset, modulo p_align.*/
} Elf32_Phdr;

/*Segment Types, p_type*/
#define PT_NULL     0/*The array element is unused; other members’ values are undefined. This type lets the program header table have ignored entries.*/
#define PT_LOAD     1/*The array element specifies a loadable segment, described by p_filesz and p_memsz.
                        The bytes from the file are mapped to the beginning of the memory segment. If the
                        segment’s memory size (p_memsz) is larger than the file size (p_filesz), the ‘‘extra’’
                        bytes are defined to hold the value 0 and to follow the segment’s initialized area. The file
                        size may not be larger than the memory size. Loadable segment entries in the program
                        header table appear in ascending order, sorted on the p_vaddr member.*/
#define PT_DYNAMIC  2/*The array element specifies dynamic linking information.*/
#define PT_INTERP   3/*The array element specifies the location and size of a null-terminated path name to invoke as an interpreter.*/
#define PT_NOTE     4/*The array element specifies the location and size of auxiliary information.*/
#define PT_SHLIB    5/**/
#define PT_PHDR     6/*The array element, if present, specifies the location and size of the program header table
                        itself, both in the file and in the memory image of the program. This segment type may
                        not occur more than once in a file. Moreover, it may occur only if the program header
                        table is part of the memory image of the program. If it is present, it must precede any
                        loadable segment entry.*/
#define PT_LOPROC   0x70000000
#define PT_HIPROC   0x7fffffff

/*Segment flags: p_flags*/
#define PF_X        (1 << 0)    /* Segment is executable */
#define PF_W       (1 << 1)    /* Segment is writable */
#define PF_R       (1 << 2)    /* Segment is readable */

typedef char loader_returnCode;

#define LOADER_OK               0x00
#define LOADER_NOT_FOUND        0x01
#define LOADER_GENERIC_ERROR    0x02
#define LOADER_BAD_FORMAT       0x03

void loader_checkHeader(char *path);
/*funzione che carica nella memoria di un task i relativi dati leggendoli da un file elf*/
loader_returnCode loader_loadElf(char *_path,int procId);
