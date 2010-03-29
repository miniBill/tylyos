/* Copyright (C) 2008 Luca Salmin
 * Copyright (C) 2008 Leonardo Taglialegne <leonardotaglialegne+tylyos@gmail.com>
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

#include "interrupt.h"
#include <memory/memory.h>
#include <kernel/stdio.h>
#include <drivers/timer/timer.h>
#include <drivers/keyboard/keyboard.h>
#include <kernel/kernel.h>

void initIdt(void) {
  int c;
  asm("cli");
  for (c = 0;c < 256;c++)
    addIdtSeg(c, 0, 0, 0);

  addIdtSeg(0, isr_0, INTERRUPT_PRESENT, segmentoCodiceKernel);
  addIdtSeg(1, isr_1, INTERRUPT_PRESENT, segmentoCodiceKernel);
  addIdtSeg(2, isr_2, INTERRUPT_PRESENT, segmentoCodiceKernel);
  addIdtSeg(3, isr_3, INTERRUPT_PRESENT, segmentoCodiceKernel);
  addIdtSeg(4, isr_4, INTERRUPT_PRESENT, segmentoCodiceKernel);
  addIdtSeg(5, isr_5, INTERRUPT_PRESENT, segmentoCodiceKernel);
  addIdtSeg(6, isr_6, INTERRUPT_PRESENT, segmentoCodiceKernel);
  addIdtSeg(7, isr_7, INTERRUPT_PRESENT, segmentoCodiceKernel);
  addIdtSeg(8, isr_8, INTERRUPT_PRESENT, segmentoCodiceKernel);
  addIdtSeg(9, isr_9, INTERRUPT_PRESENT, segmentoCodiceKernel);
  addIdtSeg(10, isr_10, INTERRUPT_PRESENT, segmentoCodiceKernel);
  addIdtSeg(11, isr_11, INTERRUPT_PRESENT, segmentoCodiceKernel);
  addIdtSeg(12, isr_12, INTERRUPT_PRESENT, segmentoCodiceKernel);
  addIdtSeg(13, isr_13, INTERRUPT_PRESENT, segmentoCodiceKernel);
  addIdtSeg(14, isr_14, INTERRUPT_PRESENT, segmentoCodiceKernel);
  addIdtSeg(15, isr_15, INTERRUPT_PRESENT, segmentoCodiceKernel);
  addIdtSeg(16, isr_16, INTERRUPT_PRESENT, segmentoCodiceKernel);
  addIdtSeg(17, isr_17, INTERRUPT_PRESENT, segmentoCodiceKernel);
  addIdtSeg(18, isr_18, INTERRUPT_PRESENT, segmentoCodiceKernel);
  for (c = 32;c < 50;c++)
    addIdtSeg(c, isr_32, INTERRUPT_PRESENT, segmentoCodiceKernel);
  addIdtSeg(32, isr_32, INTERRUPT_PRESENT, segmentoCodiceKernel);
  addIdtSeg(33, isr_33, INTERRUPT_PRESENT, segmentoCodiceKernel);
  addIdtSeg(46, isr_46, INTERRUPT_PRESENT, segmentoCodiceKernel);
  addIdtSeg(47, isr_47, INTERRUPT_PRESENT, segmentoCodiceKernel);
  addIdtSeg(0x80, isr_x80, INTERRUPT_PRESENT, segmentoCodiceKernel);

  idt_pointer.limit = 0xFFFF;
  idt_pointer.base = (unsigned int) & idt;

  idt_load();
  irq_remap(0x20, 0x28);

}

void clearIdt(void) {
  short int i;
  for (i = 0;i < 18;i++)
    addIdtSeg(i, 0, 0, 0);
}

void addIdtSeg(short int i, void (*gestore)(), unsigned char options, unsigned int seg_sel) {
  unsigned int indirizzo = (unsigned int) gestore;
  idt[i].base_hi = indirizzo >> 16;
  idt[i].base_lo = (indirizzo & 0xFFFF);
  idt[i].always0 = 0x00;
  idt[i].sel = seg_sel;
  idt[i].flags = options | 0xE; /* 1|110: 32bit|interrupt gate  */
}

void sendICW(int pic_p, int pic_s , int data) {
  outb(0x20 + data, pic_p); /*master*/
  outb(0xA0 + data, pic_s); /*slave*/
}

void irq_remap(unsigned int offset_1, unsigned int offset_2) {
  sendICW(0x11, 0x11, 0);
  io_wait();

  sendICW(offset_1, offset_2, 1);
  io_wait();

  sendICW(0x04, 0x02, 1);
  io_wait();

  sendICW(0x01, 0x01, 1);
  io_wait();

  sendICW(0x00, 0x00, 1);
  io_wait();
}

void interrupt_handler(
  unsigned int eax, unsigned int ebx, unsigned int ecx,
  unsigned int edx, unsigned int ebp, unsigned int esi,
  unsigned int edi, unsigned int ds, unsigned int es,
  unsigned int fs, unsigned int gs, unsigned int isr,
  unsigned int error, unsigned int eip, unsigned int cs,
  unsigned int eflags, ...) {
  int c;
  switch (isr) {
    case 32:
      tick();
      break;
    case 33:
      keypress();
      break;
    default:
      c = eax ^ ebx ^ ecx ^ edx ^ ebp ^ esi ^ edi ^ ds ^ es ^ fs ^ gs ^ eip ^ cs ^ eflags ^ error;
    }
  if (isr > 7) outb(0xA0, 0x20);
  outb(0x20, 0x20);
}