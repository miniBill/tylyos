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

#ifndef INTERRUPT_H_
#define INTERRUPT_H_

#define INTERRUPT_PRESENT 0x80
#define INTERRUPT_DPL_USER 0x60

struct idt_entry
{
    unsigned short base_lo;
    unsigned short sel;
    unsigned char always0;
    unsigned char flags;
    unsigned short base_hi;
} __attribute__ ( ( packed ) );

struct idt_ptr
{
    unsigned short limit;
    unsigned int base;
} __attribute__ ( ( packed ) );

struct idt_entry idt[256];
struct idt_ptr idt_pointer;

char kernelStack[0x9000];/*stack usato durante gli interrupt, settato nel tss dei task gate nella idt*/

extern void idt_load ( void );

void initIdt ( void );
void clearIdt ( void );

void addIdtSeg ( short int i, void ( *gestore ) (), unsigned char options, unsigned int seg_sel );
void addIdtGate(short int i, unsigned char options, unsigned int seg_sel);

/*
 * ritorna 1 se è stato generato un interrupt dal disco specificato, successivamente azzera lo stato
 * disk: specifica il disco, valori 0 o 1
 */
char getDiskInterruptState ( unsigned int disk );

/*
 * rimappa i PIC (programmable input controller)
 * offset_1: offset riferito alla IDT delle interruzioni per il primo PIC
 * offset_2: offset riferito alla IDT delle interruzioni per il PIC slave
*/
void irq_remap ( unsigned int offset_1, unsigned int offset_2 );

void interrupt_handler ( unsigned int eax, unsigned int ebx, unsigned int ecx, unsigned int edx,
                         unsigned int ebp, unsigned int esi, unsigned int edi, unsigned int ds,
                         unsigned int es, unsigned int fs, unsigned int gs, unsigned int isr,
                         unsigned int error, unsigned int eip, unsigned int cs,
                         unsigned int eflags, ... );

/*############### funzioni interruzioni ################*/
extern void isr_0 ( void );
extern void isr_1 ( void );
extern void isr_2 ( void );
extern void isr_3 ( void );
extern void isr_4 ( void );
extern void isr_5 ( void );
extern void isr_6 ( void );
extern void isr_7 ( void );
extern void isr_8 ( void );
extern void isr_9 ( void );
extern void isr_10 ( void );
extern void isr_11 ( void );
extern void isr_12 ( void );
extern void isr_13 ( void );
extern void isr_14 ( void );
extern void isr_15 ( void );
extern void isr_16 ( void );
extern void isr_17 ( void );
extern void isr_18 ( void );
extern void isr_32 ( void );
extern void isr_33 ( void );
extern void isr_34 ( void );
extern void isr_46 ( void );
extern void isr_47 ( void );
extern void isr_x80 ( void );

/*tss e selettore usato per impedire a lret degli interrupt di modificare il tss originale
viene infatti settato prima del return al task in modo da subire le modifiche al posto del tss originale*/
struct tss garbageTSS;unsigned short garbageTSSselector;

unsigned short isr0TSSselector; struct tss isr0TSS;
unsigned short isr1TSSselector; struct tss isr1TSS;
unsigned short isr2TSSselector; struct tss isr2TSS;
unsigned short isr3TSSselector; struct tss isr3TSS;
unsigned short isr4TSSselector; struct tss isr4TSS;
unsigned short isr5TSSselector; struct tss isr5TSS;
unsigned short isr6TSSselector; struct tss isr6TSS;
unsigned short isr7TSSselector; struct tss isr7TSS;
unsigned short isr8TSSselector; struct tss isr8TSS;
unsigned short isr9TSSselector; struct tss isr9TSS;
unsigned short isr10TSSselector; struct tss isr10TSS;
unsigned short isr11TSSselector; struct tss isr11TSS;
unsigned short isr12TSSselector; struct tss isr12TSS;
unsigned short isr13TSSselector; struct tss isr13TSS;
unsigned short isr14TSSselector; struct tss isr14TSS;
unsigned short isr15TSSselector; struct tss isr15TSS;
unsigned short isr16TSSselector; struct tss isr16TSS;
unsigned short isr17TSSselector; struct tss isr17TSS;
unsigned short isr18TSSselector; struct tss isr18TSS;

unsigned short isr32TSSselector; struct tss isr32TSS;
unsigned short isr33TSSselector; struct tss isr33TSS;
unsigned short isr34TSSselector; struct tss isr34TSS;

unsigned short isr46TSSselector; struct tss isr46TSS;
unsigned short isr47TSSselector; struct tss isr47TSS;
unsigned short isrx80TSSselector; struct tss isrx80TSS;


#endif
