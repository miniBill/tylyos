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

/*void add_IDTseg(short int i, void (*gestore)(), unsigned char options, unsigned int seg_sel){
    unsigned int indirizzo;
    indirizzo = (unsigned int)gestore;
    IDT_Table[i].offset_low= (indirizzo&0xFFFF);
    IDT_Table[i].null_par=0x00;
    IDT_Table[i].seg_selector = seg_sel;
    IDT_Table[i].options = options|0xE;*/ /*INTERRUPT GATE*/
/*    IDT_Table[i].offset_high = indirizzo >> 16;
}

void set_idtr(IDT_Descriptor *addr, unsigned short int limit){
    IDT_Register idtr;
    idtr.idt_limit = limit*8;
    idtr.idt_base = (unsigned long)addr;
    __asm__ __volatile__("lidt %0": :"g" (idtr));
}*/

/*void _globalException(int n, int error){
}*/

/*void _irqinterrupt(){
}*/

/*################# funzioni interruzioni #####################*/
void int0(){
}

void int1(){
}

void int2(){
}

void int3(){
}

void int4(){
}

void int5(){
}

void int6(){
}

void int7(){
}

void int8(){
}

void int9(){
}

void int10(){
}

void int11(){
}

void int12(){
}

void int13(){
}

void int14(){
}

void int15(){
}

void int16(){
}

void int17(){
}

void int18(){
}

void int19(){
}

