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
#include <lib/string.h>
#include <kernel/stdio.h>
#include <drivers/screen/screen.h>
#include <drivers/timer/timer.h>
#include <drivers/keyboard/keyboard.h>

int xtemp;

void initIdt(void){
    /* inizializzazione */
    int c;
    asm("cli");
    xtemp=0;
    for(c=0;c<256;c++)
        addIdtSegZ(c);

    addIdtSegN( 0, isr_0);
    addIdtSegN( 1, isr_1);
    addIdtSegN( 2, isr_2);
    addIdtSegN( 3, isr_3);
    addIdtSegN( 4, isr_4);
    addIdtSegN( 5, isr_5);
    addIdtSegN( 6, isr_6);
    addIdtSegN( 7, isr_7);
    addIdtSegN( 8, isr_8);
    addIdtSegN( 9, isr_9);
    addIdtSegN(10,isr_10);
    addIdtSegN(11,isr_11);
    addIdtSegN(12,isr_12);
    addIdtSegN(13,isr_13);
    addIdtSegN(14,isr_14);
    addIdtSegN(15,isr_15);
    addIdtSegN(16,isr_16);
    addIdtSegN(17,isr_17);
    addIdtSegN(18,isr_18);
    for(c=32;c<50;c++)
        addIdtSegN(c,isr_32);
    addIdtSegN(32,isr_32);
    addIdtSegN(33,isr_33);
    addIdtSegN(46,isr_46);
    addIdtSegN(47,isr_47);

    idt_pointer.limit=0xFFFF;
    idt_pointer.base=(unsigned int)&idt;

    idt_load();
    irq_remap(0x20,0x28);

}

void clearIdt(void){
    short int i;
    for(i=0;i<18;i++)
        addIdtSeg(i,0,0,0);
}

void addIdtSegN(short int i,void (*gestore)()){
    addIdtSeg(i,gestore,0x80,0x08);
}

void addIdtSegZ(short int i){
    addIdtSeg(i,0,0,0);
}

void addIdtSeg(short int i, void (*gestore)(), unsigned char options, unsigned int seg_sel){
    unsigned int indirizzo = (unsigned int)gestore;
    idt[i].base_hi = indirizzo >> 16;
    idt[i].base_lo= (indirizzo&0xFFFF);
    idt[i].always0=0x00;
    idt[i].sel = seg_sel;
    idt[i].flags = options|0xE; /* 1|11|0: valido|ring3|sistema  */
}

void sendICW(int pic_p,int pic_s ,int data){
    outb(0x20+data, pic_p);/*master*/
    outb(0xA0+data, pic_s);/*slave*/
}

/* rimappa i PIC (programmable input controller)
 * offset_1: offset riferito alla IDT delle interruzioni per il primo PIC
 * offset_2: offset riferito alla IDT delle interruzioni per il PIC slave
 */
void irq_remap(unsigned int offset_1, unsigned int offset_2){
    /*
     * PIC_P e' il PIC primario o "master"
     * PIC_S e' il PIC secondario o "slave"
     *
     * Quando si manifesta un IRQ che riguarda il PIC secondario,
     * il PIC primario riceve IRQ 2
     *
     * ICW = initialization command word
     * OCW = operation command word
     */

    /* Inizializzazione                                */
    /* 0x10 significa che si sta' inizializzando       */
    /* 0x01 significa che si deve arrivare fino a ICW4 */
    sendICW(0x11,0x11,0);
    io_wait();

    /* ICW2: PIC_P a partire da "offset_1" */
    /*       PIC_S a partire da "offset_2" */
    sendICW(offset_1,offset_2,1);
    io_wait();

    /* ICW3: PIC_P: IRQ2 per pilotare PIC_S    */
    /*       PIC_S: pilotato con IRQ2 da PIC_P */
    sendICW(0x04,0x02,1);
    io_wait();

    /* ICW4: si precisa la modalita' del microprocessore; 0x01 = 8086 */
    sendICW(0x01,0x01,1);
    io_wait();

    /* OCW1: azzera la maschera in modo da abilitare tutti i numeri IRQ */
    sendICW(0x00,0x00,1);
    io_wait();
}

void interrupt_handler(
    unsigned int eax, unsigned int ebx, unsigned int ecx,
    unsigned int edx, unsigned int ebp, unsigned int esi,
    unsigned int edi, unsigned int ds, unsigned int es,
    unsigned int fs, unsigned int gs, unsigned int isr,
    unsigned int error, unsigned int eip, unsigned int cs,
    unsigned int eflags, ...){
    if(isr==32){
        /*timer*/
        tick();
    }
    else{
        /* codice che interpreta le interruzioni */
        int c=0;
        xtemp++;
        if(isr==33)
            keypress();
        else{
            printf("interruzione, interrupt: %d, count: %d.\n",isr,xtemp);
#ifdef PRINT_REGISTERS
            printf("EAX: %d,EBX: %d,ECX: %d,EDX: %d,",eax,ebx,ecx,edx);
            printf("EBP: %d,ESI: %d,EDI: %d,",ebp,esi,edi);
            printf("DS: %d,ES: %d,FS: %d,GS: %d,",ds,es,fs,gs);
            printf("EIP: %d,CS: %d,EFLAGS: %d,ERROR: %d\n",eip,cs,eflags,error);
#else
            c=eax^ebx^ecx^edx^ebp^esi^edi^ds^es^fs^gs^eip^cs^eflags^error;/*HACK*/
#endif
        }
    }
    /* Send End Of Interrupt to PIC */
    if(isr>7)outb(0xA0,0x20);
    outb(0x20,0x20);
}
