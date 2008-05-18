/* Copyright (C) 2008 Luca Salmin
 * Copyright (C) 2008 Leonardo Taglialegne <leonardotaglialegne+clearos@gmail.com>
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
#include "../drivers/keyboard/keyboard.h"
#include "../kernel/stdio.h"
#include "../drivers/screen/io.h"
#include "../drivers/screen/screen.h"

/* #define PRINT_REGISTERS */


int xtemp;

void initIDT(){
    /* inizializzazione */
    int c;
    asm("cli");
    xtemp=0;
    for(c=0;c<256;c++)
        addIDTseg(c,0,0,0);

    addIDTseg( 0, isr_0,0x80,0x08);
    addIDTseg( 1, isr_1,0x80,0x08);
    addIDTseg( 2, isr_2,0x80,0x08);
    addIDTseg( 3, isr_3,0x80,0x08);
    addIDTseg( 4, isr_4,0x80,0x08);
    addIDTseg( 5, isr_5,0x80,0x08);
    addIDTseg( 6, isr_6,0x80,0x08);
    addIDTseg( 7, isr_7,0x80,0x08);
    addIDTseg( 8, isr_8,0x80,0x08);
    addIDTseg( 9, isr_9,0x80,0x08);
    addIDTseg(10,isr_10,0x80,0x08);
    addIDTseg(11,isr_11,0x80,0x08);
    addIDTseg(12,isr_12,0x80,0x08);
    addIDTseg(13,isr_13,0x80,0x08);
    addIDTseg(14,isr_14,0x80,0x08);
    addIDTseg(15,isr_15,0x80,0x08);
    addIDTseg(16,isr_16,0x80,0x08);
    addIDTseg(17,isr_17,0x80,0x08);
    addIDTseg(18,isr_18,0x80,0x08);
    for(c=33;c<60;c++)
        addIDTseg(c,isr_32,0x80,0x08);
    addIDTseg(32,isr_18,0x80,0x08);
    addIDTseg(33,isr_18,0x80,0x08);
    addIDTseg(47,isr_18,0x80,0x08);

    idt_pointer.limit=0xFFFF;
    idt_pointer.base=(unsigned int)&idt;

    idt_load();
    irq_remap(33,50);
    asm("sti");
}

void addIDTseg(short int i, void (*gestore)(), unsigned char options, unsigned int seg_sel){
    unsigned int indirizzo = (unsigned int)gestore;
    idt[i].base_hi = indirizzo >> 16;
    idt[i].base_lo= (indirizzo&0xFFFF);
    idt[i].always0=0x00;
    idt[i].sel = seg_sel;
    idt[i].flags = options|0xE; /* 1|11|0: valido|ring3|sistema  */
}

void sendICW(int pic_p,int pic_s ,int data){
    outb (0x20+data, pic_p);/*master*/
    outb (0xA0+data, pic_s);/*slave*/
}

/* rimappa i PIC (programmable input controller)
 * offset_1: offset riferito alla IDT delle interruzioni per il primo PIC
 * offset_2: offset riferito alla IDT delle interruzioni per il PIC slave
 */
void irq_remap(unsigned int offset_1, unsigned int offset_2){
    /*
     * PIC_P � il PIC primario o "master"
     * PIC_S � il PIC secondario o "slave"
     *
     * Quando si manifesta un IRQ che riguarda il PIC secondario,
     * il PIC primario riceve IRQ 2
     *
     * ICW = initialization command word
     * OCW = operation command word
     */
    write("Kernel: PIC remap: ");

    /* Inizializzazione                                */
    /* 0x10 significa che si st� inizializzando        */
    /* 0x01 significa che si deve arrivare fino a ICW4 */
    sendICW(0x11,0x11,0);
    write("ICW1, ");

    /* ICW2: PIC_P a partire da "offset_1" */
    /*       PIC_S a partire da "offset_2" */
    sendICW(offset_1,offset_2,1);
    write("ICW2, ");

    /* ICW3: PIC_P: IRQ2 per pilotare PIC_S    */
    /*       PIC_S: pilotato con IRQ2 da PIC_P */
    sendICW(0x04,0x02,1);
    write("ICW3, ");

    /* ICW4: si precisa la modalit� del microprocessore; 0x01 = 8086 */
    sendICW(0x01,0x01,1);
    write("ICW4, ");

    /* OCW1: azzera la maschera in modo da abilitare tutti i numeri IRQ */
    sendICW(0x00,0x00,1);
    write("OCW1.");

    writeline("PIC remapped");
}

void print(unsigned int reg,char * space){
    int c=0;
    while(*(space+c)!=':')
        c++;
    *(space+c+1)=0;
    strapp(space,"%x ",(void*)reg);
    write(space);
}

void interrupt_handler(
    unsigned int eax, unsigned int ebx, unsigned int ecx,
    unsigned int edx, unsigned int ebp, unsigned int esi,
    unsigned int edi, unsigned int ds, unsigned int es,
    unsigned int fs, unsigned int gs, unsigned int isr,
    unsigned int error, unsigned int eip, unsigned int cs,
    unsigned int eflags, ...){
    /* codice che interpreta le interruzioni */
    int c=12;
    char * out="interruzione"/*12 bytes*/
        "\0\0\0\0\0\0\0\0"
        "\0\0\0\0\0\0\0\0"
        "\0\0\0\0\0\0\0\0"
        "\0\0\0\0\0\0\0\0";/*32 bytes*/
    if(isr!=13){/*HACK*/
    xtemp++;
    for(;c<44;c++)
        *(out+c)=0;
    strapp(out,", interrupt: %d",(void *)isr);
    strapp(out,", count: %d",(void *)xtemp);
   /* writeline(out); */
#ifdef PRINT_REGISTERS
    print(eax,"EAX:\0\0\0\0\0\0\0\0");
    print(ebx,"EBX:\0\0\0\0\0\0\0\0");
    print(ecx,"ECX:\0\0\0\0\0\0\0\0");
    print(edx,"EDX:\0\0\0\0\0\0\0\0");
    print(ebp,"EBP:\0\0\0\0\0\0\0\0");
    print(esi,"ESI:\0\0\0\0\0\0\0\0");
    print(edi,"EDI:\0\0\0\0\0\0\0\0");
    print(ds,"DS:\0\0\0\0\0\0\0\0");
    print(es,"ES:\0\0\0\0\0\0\0\0");
    print(fs,"FS:\0\0\0\0\0\0\0\0");
    print(gs,"GS:\0\0\0\0\0\0\0\0");
    print(eip,"EIP:\0\0\0\0\0\0\0\0");
    print(cs,"CS:\0\0\0\0\0\0\0\0");
    print(eflags,"EFLAGS:\0\0\0\0\0\0\0\0");
    print(error,"ERROR:\0\0\0\0\0\0\0\0");
#endif
   /* writeline(""); */
    if(isr==9){
        c=inb(0x60);
        put(ScanCodeToChar(c));
    }
    }/*HACK*/
    /* Send End Of Interrupt to PIC */
    if(isr>7)outb(0xA0,0x20);
    outb(0x20,0x20);
}
