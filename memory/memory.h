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
#ifndef MEMORY_H_
#define MEMORY_H_

void initGdt();

struct gdtEntry{
    unsigned short limitLow;
    unsigned short baseLow;
    unsigned char baseMiddle;
    unsigned char access;
    unsigned char granularity;
    unsigned char baseHigh;
} __attribute__((packed));

struct gdtPtr{
    unsigned short limit;
    unsigned int base;
} __attribute__((packed));

struct gdtEntry gdt[3];
struct gdtPtr gdtPointer;

extern void gdtFlush();

/*
	1 pagina = 4096 byte = 0x1000
*/

#define KERNEL_START	0x00100000	/* 1.048.576 byte */
#define MEMORY_START 0x00400000	/* 4.194.304 byte*/

#define MAX_PAGES_IN_MEMORY	100	/* numero massimo di pagine allocabili in memoria contemporaneamente */

unsigned int PageDir[1024];		/* area da 4096byte che ospita la pagedir del kernel */
unsigned int memoryBitmap[MAX_PAGES_IN_MEMORY/32+1];	/* flag per ogni blocco di 4k della memoria fisica */

void InitPaging();

int getBit(int x);
void setBit(int x,unsigned int value);

extern unsigned int read_cr0();
extern void write_cr0(unsigned int data);
extern unsigned int read_cr3();
extern void write_cr3(unsigned int data);
#endif
