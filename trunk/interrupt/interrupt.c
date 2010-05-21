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
#include <memory/gdtflags.h>
#include <lib/string.h>
#include <kernel/stdio.h>
#include <drivers/screen/screen.h>
#include <drivers/timer/timer.h>
#include <drivers/keyboard/keyboard.h>
#include <kernel/kernel.h>
#include <task/dispatcher.h>
#include <task/scheduler.h>

int xtemp;

char diskInterrupt[2];

/*riempie la IDT (interrupt descriptor table)*/
void initIdt(void) {
  /* inizializzazione */
  int c;
  diskInterrupt[0] = 0;
  diskInterrupt[1] = 0;
  asm("cli");
  xtemp = 0;
  for (c = 0;c < 256;c++)
    addIdtSeg(c, 0, 0, 0);

  /*TODO: eliminare dopo che tutti gli interrupt sono stati passati ai task gate in modo funzionante*/
/*
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
  for (c = 34;c < 50;c++)
    addIdtSeg(c, isr_34, INTERRUPT_PRESENT, segmentoCodiceKernel);
  addIdtSeg(32, isr_32, INTERRUPT_PRESENT, segmentoCodiceKernel);
  addIdtSeg(33, isr_33, INTERRUPT_PRESENT, segmentoCodiceKernel);

  addIdtSeg(46, isr_46, INTERRUPT_PRESENT, segmentoCodiceKernel);
  addIdtSeg(47, isr_47, INTERRUPT_PRESENT, segmentoCodiceKernel);
  addIdtSeg(0x80, isr_x80, INTERRUPT_PRESENT, segmentoCodiceKernel);
*/
    /*questo e' un test*/
   //addIdtGate(32,INTERRUPT_PRESENT, kernelInterruptTSSselector);


    /*inizializza i selettori*/
/*TODO*/
    isr0TSSselector=segmentSelector (  ISR_TSS_INDEX+0,0,RPL_KERNEL );
    isr1TSSselector=segmentSelector (  ISR_TSS_INDEX+1,0,RPL_KERNEL );
    isr2TSSselector=segmentSelector (  ISR_TSS_INDEX+2,0,RPL_KERNEL );
    isr3TSSselector=segmentSelector (  ISR_TSS_INDEX+3,0,RPL_KERNEL );
    isr4TSSselector=segmentSelector (  ISR_TSS_INDEX+4,0,RPL_KERNEL );
    isr5TSSselector=segmentSelector (  ISR_TSS_INDEX+5,0,RPL_KERNEL );
    isr6TSSselector=segmentSelector (  ISR_TSS_INDEX+6,0,RPL_KERNEL );
    isr7TSSselector=segmentSelector (  ISR_TSS_INDEX+7,0,RPL_KERNEL );
    isr8TSSselector=segmentSelector (  ISR_TSS_INDEX+8,0,RPL_KERNEL );
    isr9TSSselector=segmentSelector (  ISR_TSS_INDEX+9,0,RPL_KERNEL );
    isr10TSSselector=segmentSelector (  ISR_TSS_INDEX+10,0,RPL_KERNEL );
    isr11TSSselector=segmentSelector (  ISR_TSS_INDEX+11,0,RPL_KERNEL );
    isr12TSSselector=segmentSelector (  ISR_TSS_INDEX+12,0,RPL_KERNEL );
    isr13TSSselector=segmentSelector (  ISR_TSS_INDEX+13,0,RPL_KERNEL );
    isr14TSSselector=segmentSelector (  ISR_TSS_INDEX+14,0,RPL_KERNEL );
    isr15TSSselector=segmentSelector (  ISR_TSS_INDEX+15,0,RPL_KERNEL );
    isr16TSSselector=segmentSelector (  ISR_TSS_INDEX+16,0,RPL_KERNEL );
    isr17TSSselector=segmentSelector (  ISR_TSS_INDEX+17,0,RPL_KERNEL );
    isr18TSSselector=segmentSelector (  ISR_TSS_INDEX+18,0,RPL_KERNEL );
    isr32TSSselector=segmentSelector (  ISR_TSS_INDEX+32,0,RPL_KERNEL );
    isr33TSSselector=segmentSelector (  ISR_TSS_INDEX+33,0,RPL_KERNEL );

    isr34TSSselector=segmentSelector (  ISR_TSS_INDEX+34,0,RPL_KERNEL );

    isr46TSSselector=segmentSelector (  ISR_TSS_INDEX+46,0,RPL_KERNEL );
    isr47TSSselector=segmentSelector (  ISR_TSS_INDEX+47,0,RPL_KERNEL );
    isrx80TSSselector=segmentSelector (  ISR_TSS_INDEX+0x80,0,RPL_KERNEL );
    /*inizializza i TSS*/
/*TODO*/
    isr0TSS=kernelInterruptTSS;
    isr0TSS.eip=(unsigned int)isr_0;
    isr1TSS=kernelInterruptTSS;
    isr1TSS.eip=(unsigned int)isr_1;
    isr2TSS=kernelInterruptTSS;
    isr2TSS.eip=(unsigned int)isr_2;
    isr3TSS=kernelInterruptTSS;
    isr3TSS.eip=(unsigned int)isr_3;
    isr4TSS=kernelInterruptTSS;
    isr4TSS.eip=(unsigned int)isr_4;
    isr5TSS=kernelInterruptTSS;
    isr5TSS.eip=(unsigned int)isr_5;
    isr6TSS=kernelInterruptTSS;
    isr6TSS.eip=(unsigned int)isr_6;
    isr7TSS=kernelInterruptTSS;
    isr7TSS.eip=(unsigned int)isr_7;
    isr8TSS=kernelInterruptTSS;
    isr8TSS.eip=(unsigned int)isr_8;
    isr9TSS=kernelInterruptTSS;
    isr9TSS.eip=(unsigned int)isr_9;
    isr10TSS=kernelInterruptTSS;
    isr10TSS.eip=(unsigned int)isr_10;
    isr11TSS=kernelInterruptTSS;
    isr11TSS.eip=(unsigned int)isr_11;
    isr12TSS=kernelInterruptTSS;
    isr12TSS.eip=(unsigned int)isr_12;
    isr13TSS=kernelInterruptTSS;
    isr13TSS.eip=(unsigned int)isr_13;
    isr14TSS=kernelInterruptTSS;
    isr14TSS.eip=(unsigned int)isr_14;
    isr15TSS=kernelInterruptTSS;
    isr15TSS.eip=(unsigned int)isr_15;
    isr16TSS=kernelInterruptTSS;
    isr16TSS.eip=(unsigned int)isr_16;
    isr17TSS=kernelInterruptTSS;
    isr17TSS.eip=(unsigned int)isr_17;
    isr18TSS=kernelInterruptTSS;
    isr18TSS.eip=(unsigned int)isr_18;
    isr32TSS=kernelInterruptTSS;
    isr32TSS.eip=(unsigned int)isr_32;
    isr33TSS=kernelInterruptTSS;
    isr33TSS.eip=(unsigned int)isr_33;

    isr34TSS=kernelInterruptTSS;
    isr34TSS.eip=(unsigned int)isr_34;

    isr46TSS=kernelInterruptTSS;
    isr46TSS.eip=(unsigned int)isr_46;
    isr47TSS=kernelInterruptTSS;
    isr47TSS.eip=(unsigned int)isr_47;
    isrx80TSS=kernelInterruptTSS;
    isrx80TSS.eip=(unsigned int)isr_x80;
    /*inizializza i descrittori*/
/*TODO*/

    TSSset(ISR_TSS_INDEX+0,(unsigned int)&isr0TSS,MEM_TSS|MEM_KERNEL|MEM_PRESENT);   
    TSSset(ISR_TSS_INDEX+1,(unsigned int)&isr1TSS,MEM_TSS|MEM_KERNEL|MEM_PRESENT);   
    TSSset(ISR_TSS_INDEX+2,(unsigned int)&isr2TSS,MEM_TSS|MEM_KERNEL|MEM_PRESENT);   
    TSSset(ISR_TSS_INDEX+3,(unsigned int)&isr3TSS,MEM_TSS|MEM_KERNEL|MEM_PRESENT);   
    TSSset(ISR_TSS_INDEX+4,(unsigned int)&isr4TSS,MEM_TSS|MEM_KERNEL|MEM_PRESENT);   
    TSSset(ISR_TSS_INDEX+5,(unsigned int)&isr5TSS,MEM_TSS|MEM_KERNEL|MEM_PRESENT);   
    TSSset(ISR_TSS_INDEX+6,(unsigned int)&isr6TSS,MEM_TSS|MEM_KERNEL|MEM_PRESENT);   
    TSSset(ISR_TSS_INDEX+7,(unsigned int)&isr7TSS,MEM_TSS|MEM_KERNEL|MEM_PRESENT);   
    TSSset(ISR_TSS_INDEX+8,(unsigned int)&isr8TSS,MEM_TSS|MEM_KERNEL|MEM_PRESENT);   
    TSSset(ISR_TSS_INDEX+9,(unsigned int)&isr9TSS,MEM_TSS|MEM_KERNEL|MEM_PRESENT);   
    TSSset(ISR_TSS_INDEX+10,(unsigned int)&isr10TSS,MEM_TSS|MEM_KERNEL|MEM_PRESENT);   
    TSSset(ISR_TSS_INDEX+11,(unsigned int)&isr11TSS,MEM_TSS|MEM_KERNEL|MEM_PRESENT);   
    TSSset(ISR_TSS_INDEX+12,(unsigned int)&isr12TSS,MEM_TSS|MEM_KERNEL|MEM_PRESENT);   
    TSSset(ISR_TSS_INDEX+13,(unsigned int)&isr13TSS,MEM_TSS|MEM_KERNEL|MEM_PRESENT);   
    TSSset(ISR_TSS_INDEX+14,(unsigned int)&isr14TSS,MEM_TSS|MEM_KERNEL|MEM_PRESENT);   
    TSSset(ISR_TSS_INDEX+15,(unsigned int)&isr15TSS,MEM_TSS|MEM_KERNEL|MEM_PRESENT);   
    TSSset(ISR_TSS_INDEX+16,(unsigned int)&isr16TSS,MEM_TSS|MEM_KERNEL|MEM_PRESENT);   
    TSSset(ISR_TSS_INDEX+17,(unsigned int)&isr17TSS,MEM_TSS|MEM_KERNEL|MEM_PRESENT);   
    TSSset(ISR_TSS_INDEX+18,(unsigned int)&isr18TSS,MEM_TSS|MEM_KERNEL|MEM_PRESENT);   

    TSSset(ISR_TSS_INDEX+32,(unsigned int)&isr32TSS,MEM_TSS|MEM_KERNEL|MEM_PRESENT);   
    TSSset(ISR_TSS_INDEX+33,(unsigned int)&isr33TSS,MEM_TSS|MEM_KERNEL|MEM_PRESENT);   

    TSSset(ISR_TSS_INDEX+34,(unsigned int)&isr34TSS,MEM_TSS|MEM_KERNEL|MEM_PRESENT);   

    TSSset(ISR_TSS_INDEX+46,(unsigned int)&isr46TSS,MEM_TSS|MEM_KERNEL|MEM_PRESENT);   
    TSSset(ISR_TSS_INDEX+47,(unsigned int)&isr47TSS,MEM_TSS|MEM_KERNEL|MEM_PRESENT);   
    TSSset(ISR_TSS_INDEX+0x80,(unsigned int)&isrx80TSS,MEM_TSS|MEM_KERNEL|MEM_PRESENT);   

    /*inizializza la IDT*/
/*TODO*/
    addIdtGate(0,INTERRUPT_PRESENT, isr0TSSselector);
    addIdtGate(1,INTERRUPT_PRESENT, isr1TSSselector);
    addIdtGate(2,INTERRUPT_PRESENT, isr2TSSselector);
    addIdtGate(3,INTERRUPT_PRESENT, isr3TSSselector);
    addIdtGate(4,INTERRUPT_PRESENT, isr4TSSselector);
    addIdtGate(5,INTERRUPT_PRESENT, isr5TSSselector);
    addIdtGate(6,INTERRUPT_PRESENT, isr6TSSselector);
    addIdtGate(7,INTERRUPT_PRESENT, isr7TSSselector);
    addIdtGate(8,INTERRUPT_PRESENT, isr8TSSselector);
    addIdtGate(9,INTERRUPT_PRESENT, isr9TSSselector);
    addIdtGate(10,INTERRUPT_PRESENT, isr10TSSselector);
    addIdtGate(11,INTERRUPT_PRESENT, isr11TSSselector);
    addIdtGate(12,INTERRUPT_PRESENT, isr12TSSselector);
    addIdtGate(13,INTERRUPT_PRESENT, isr13TSSselector);
    addIdtGate(14,INTERRUPT_PRESENT, isr14TSSselector);
    addIdtGate(15,INTERRUPT_PRESENT, isr15TSSselector);
    addIdtGate(16,INTERRUPT_PRESENT, isr16TSSselector);
    addIdtGate(17,INTERRUPT_PRESENT, isr17TSSselector);
    addIdtGate(18,INTERRUPT_PRESENT, isr18TSSselector);
    addIdtGate(32,INTERRUPT_PRESENT|INTERRUPT_DPL_USER, isr32TSSselector);
    addIdtGate(33,INTERRUPT_PRESENT|INTERRUPT_DPL_USER, isr33TSSselector);
    for (c = 34;c < 50;c++)
        addIdtGate(c,INTERRUPT_PRESENT, isr34TSSselector);

    addIdtGate(46,INTERRUPT_PRESENT, isr46TSSselector);
    addIdtGate(47,INTERRUPT_PRESENT, isr47TSSselector);
    addIdtGate(0x80,INTERRUPT_PRESENT|INTERRUPT_DPL_USER, isrx80TSSselector);

  idt_pointer.limit = 0xFFFF;
  idt_pointer.base = (unsigned int) & idt;

  /*scrive idt_pointer nel registro lidt*/
  idt_load();
  /*rimappa gli offset delle interrupt generate dai PIC*/
  /*          32   40 */
  irq_remap(0x20, 0x28);

//asm("int $0x80");

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

void addIdtGate(short int i, unsigned char options, unsigned int seg_sel)
 {
  unsigned int indirizzo = (unsigned int) 0;
  idt[i].base_hi = indirizzo >> 16;
  idt[i].base_lo = (indirizzo & 0xFFFF);
  idt[i].always0 = 0x00;
  idt[i].sel = seg_sel;
  idt[i].flags = options | 0x5; /* 1|1|101: system|32bit|task gate  */
}

void sendICW(int pic_p, int pic_s , int data) {
  outb(0x20 + data, pic_p); /*master*/
  outb(0xA0 + data, pic_s); /*slave*/
}

/* rimappa i PIC (programmable input controller)
 * offset_1: offset riferito alla IDT delle interruzioni per il primo PIC
 * offset_2: offset riferito alla IDT delle interruzioni per il PIC slave
 */
void irq_remap(unsigned int offset_1, unsigned int offset_2) {
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
  sendICW(0x11, 0x11, 0);
  io_wait();

  /* ICW2: PIC_P a partire da "offset_1" */
  /*       PIC_S a partire da "offset_2" */
  sendICW(offset_1, offset_2, 1);
  io_wait();

  /* ICW3: PIC_P: IRQ2 per pilotare PIC_S    */
  /*       PIC_S: pilotato con IRQ2 da PIC_P */
  sendICW(0x04, 0x02, 1);
  io_wait();

  /* ICW4: si precisa la modalita' del microprocessore; 0x01 = 8086 */
  sendICW(0x01, 0x01, 1);
  io_wait();

  /* OCW1: azzera la maschera in modo da abilitare tutti i numeri IRQ */
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

  struct tss *currentTaskTSS=(struct tss*)getBaseFromSegmentDescriptor(newTSSselector>>3);

  unsigned short originalSelector;
  originalSelector=getTSS();
  struct tss *original=(struct tss*)getBaseFromSegmentDescriptor(originalSelector>>3);
if(original!=&kernelTSS)
{
  garbageTSS=*original;
  loadTSSregister(garbageTSSselector,GARBAGE_TSS_INDEX);
   gdt[originalSelector>>3].access &= 0xFD;
  garbageTSS.link=newTSSselector;
}
else
{
printf(2,"QUESTA ISR: %d sta' venendo eseguita nel senza task gate!!!\n",isr);

}

  /* codice che interpreta le interruzioni */
  switch (isr) {
    case 32:
      /*timer*/
      tick();
  /* Send End Of Interrupt to PIC */
  if (isr > 7) outb(0xA0, 0x20);
  outb(0x20, 0x20);
      schedule();
      break;
    case 33:
      keypress();
      break;
    case 34:
      break;
    case 46:
    case 47:
      /* TODO: disk interrupt */
      /*printf("{i%d}",isr);*/
      if (isr == 46)
        diskInterrupt[0] = 1;
      else /*47*/
        diskInterrupt[1] = 1;
      break;
    /*case 0x10:
      printf(0,"Ten");
      break;*/
    case 0x80:
        printf(0,"\n\nSYSCALL!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
 //       while(1);
      switch(currentTaskTSS->eax&0xFF){
        case 88:
          asm("cli");
          clearIdt();
          asm("int $1");
          break;
        case 250:
          printf(0, "TASK MESSAGE: %s" , (char*)user_start+currentTaskTSS->ebx);
          break;
      }
      break;
   /* case 0xE:
      break;//HEAVY HACK
      */
    default:
#ifdef PRINT_REGISTERS
      printf(0, "EAX: %d,EBX: %d,ECX: %d,EDX: %d,", eax, ebx, ecx, edx);
      printf(0, "EBP: %d,ESI: %d,EDI: %d,", ebp, esi, edi);
      printf(0, "DS: %d,ES: %d,FS: %d,GS: %d,", ds, es, fs, gs);
      printf(0, "EIP: %d,CS: %d,EFLAGS: %d,ERROR: %d\n", eip, cs, eflags, error);
#else
      c = eax ^ ebx ^ ecx ^ edx ^ ebp ^ esi ^ edi ^ ds ^ es ^ fs ^ gs ^ eip ^ cs ^ eflags ^ error ^ xtemp;/*HACK*/
#endif
      char message[1000];

    struct tss *taskTSSt=(struct tss*)getBaseFromSegmentDescriptor(newTSSselector>>3);

      sprintf(message, 1000, "I have recived an interrupt: 0x%x, task eip: %x error code: 0x%x A:0x%x B:0x%x C:0x%x D:0x%x IP:0x%x", isr,taskTSSt->eip,error,eax,ebx,ecx,edx,eip);
      kernelPanic("interrupt_handler()", message);
    }
  /* Send End Of Interrupt to PIC */
  if (isr > 7) outb(0xA0, 0x20);
  outb(0x20, 0x20);



  
  

}

char getDiskInterruptState(unsigned int disk) {
  char ret;
  ret = diskInterrupt[disk];
  if (ret == 1)
    diskInterrupt[disk] = 0;
  return ret;
}
