/* Copyright (C) 2008 Luca Salmin
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

#ifndef INTERRUPT_H_
#define INTERRUPT_H_

struct idt_entry{
    unsigned short base_lo;
    unsigned short sel;
    unsigned char always0;
    unsigned char flags;
    unsigned short base_hi;
} __attribute__((packed));

struct idt_ptr{
    unsigned short limit;
    unsigned int base;
} __attribute__((packed));

struct idt_entry idt[256];
struct idt_ptr idt_pointer;

extern void idt_load(void);

void initIdt(void);
void clearIdt(void);

void addIdtSeg(short int i, void (*gestore)(), unsigned char options, unsigned int seg_sel);

/*
 * rimappa i PIC (programmable input controller)
 * offset_1: offset riferito alla IDT delle interruzioni per il primo PIC
 * offset_2: offset riferito alla IDT delle interruzioni per il PIC slave
*/
void irq_remap (unsigned int offset_1, unsigned int offset_2);

void interrupt_handler (unsigned int eax, unsigned int ebx, unsigned int ecx, unsigned int edx,
                        unsigned int ebp, unsigned int esi, unsigned int edi, unsigned int ds,
                        unsigned int es, unsigned int fs, unsigned int gs, unsigned int isr,
                        unsigned int error, unsigned int eip, unsigned int cs,
                        unsigned int eflags, ...);

/*############### funzioni interruzioni ################*/
extern void isr_0(void);
extern void isr_1(void);
extern void isr_2(void);
extern void isr_3(void);
extern void isr_4(void);
extern void isr_5(void);
extern void isr_6(void);
extern void isr_7(void);
extern void isr_8(void);
extern void isr_9(void);
extern void isr_10(void);
extern void isr_11(void);
extern void isr_12(void);
extern void isr_13(void);
extern void isr_14(void);
extern void isr_15(void);
extern void isr_16(void);
extern void isr_17(void);
extern void isr_18(void);
extern void isr_32(void);
extern void isr_33(void);
extern void isr_47(void);

#endif
