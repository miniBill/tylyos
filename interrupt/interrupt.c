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

#include "interrupt.h"
#include "../kernel/stdio.h"
#include "../kernel/screen.h"

int xtemp;

void initIDT()
{
	/* inizializzazione */
	int c;
xtemp=0;
	for(c=0;c<256;c++)
	{
		addIDTseg(c,0,0,0);
	}

	addIDTseg(0,isr_0,0x80,0x08);
	addIDTseg(1,isr_1,0x80,0x08);
	addIDTseg(2,isr_2,0x80,0x08);
	addIDTseg(3,isr_3,0x80,0x08);
	addIDTseg(4,isr_4,0x80,0x08);
	addIDTseg(5,isr_5,0x80,0x08);
	addIDTseg(6,isr_6,0x80,0x08);
	addIDTseg(7,isr_7,0x80,0x08);
	addIDTseg(8,isr_8,0x80,0x08);
	addIDTseg(9,isr_9,0x80,0x08);
	addIDTseg(10,isr_10,0x80,0x08);
	addIDTseg(11,isr_11,0x80,0x08);
	addIDTseg(12,isr_12,0x80,0x08);
	addIDTseg(13,isr_13,0x80,0x08);
	addIDTseg(14,isr_14,0x80,0x08);
	addIDTseg(15,isr_15,0x80,0x08);
	addIDTseg(16,isr_16,0x80,0x08);
	addIDTseg(17,isr_17,0x80,0x08);
	addIDTseg(18,isr_18,0x80,0x08);
addIDTseg(34,isr_32,0x80,0x08);

	idt_pointer.limit=0xFFFF;
	idt_pointer.base=(unsigned int)&idt;

	idt_load();
	irq_remap(33,50);
}

void addIDTseg(short int i, void (*gestore)(), unsigned char options, unsigned int seg_sel){
    unsigned int indirizzo;
    indirizzo = (unsigned int)gestore;
    idt[i].base_lo= (indirizzo&0xFFFF);
    idt[i].always0=0x00;
    idt[i].sel = seg_sel;
    idt[i].flags = options|0xE; /* 1|11|0: valido|ring3|sistema  */
    idt[i].base_hi = indirizzo >> 16;
}







/*
	rimappa i PIC (programmable input controller)
	offset_1: offset riferito alla IDT delle interruzioni per il primo PIC
	offset_2: offset riferito alla IDT delle interruzioni per il PIC slave
*/
void irq_remap (unsigned int offset_1, unsigned int offset_2)
{
    /*
    * PIC_P � il PIC primario o �master�;
    * PIC_S � il PIC secondario o �slave�.
    *
    * Quando si manifesta un IRQ che riguarda il PIC secondario,
    * il PIC primario riceve IRQ 2.
    *
    * ICW = initialization command word.
    * OCW = operation command word.
    */
    writeline("kernel: PIC (programmable interrupt controller) remap: ");

    outb (0x20, 0x10 + 0x01);   /* Inizializzazione: 0x10 significa che 	*/
    outb (0xA0, 0x10 + 0x01);   /* si tratta di ICW1; 0x01 significa che	*/
    write("ICW1");            	/* si deve arrivare fino a ICW4.		*/

    outb (0x21, offset_1);      /* ICW2: PIC_P a partire da �offset_1�.		*/
    outb (0xA1, offset_2);      /*       PIC_S a partire da �offset_2�.		*/
    write(", ICW2");
    outb (0x21, 0x04);          /* ICW3 PIC_P: IRQ2 pilotato da PIC_S.		*/
    outb (0xA1, 0x02);          /* ICW3 PIC_S: pilota IRQ2 di PIC_P.		*/
    write(", ICW3");
    outb (0x21, 0x01);          /* ICW4: si precisa solo la modalit�		*/
    outb (0xA1, 0x01);          /* del microprocessore; 0x01 = 8086.		*/
    write(", ICW4");

    outb (0x21, 0x00);          /* OCW1: azzera la maschera in modo da		*/
    outb (0xA1, 0x00);          /* abilitare tutti i numeri IRQ.		*/
    writeline(", OCW1.");
}



void interrupt_handler (unsigned int eax,
                        unsigned int ecx,
                        unsigned int edx,
                        unsigned int ebx,
                        unsigned int ebp,
                        unsigned int esi,
                        unsigned int edi,
                        unsigned int ds,
                        unsigned int es,
                        unsigned int fs,
                        unsigned int gs,
                        unsigned int isr,
                        unsigned int error,
                        unsigned int eip,
                        unsigned int cs,
                        unsigned int eflags, ...)
{
	/* codice che interpreta le interruzioni */
	char *out="interruzione\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0";
xtemp++;
	 /* strapp(out," %d",isr+1); */
	 strapp(out,"|error: %d",isr); 
	
	 writeline(out); 
	
   	
}
