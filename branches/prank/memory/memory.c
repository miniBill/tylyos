/* Copyright (C) 2008 Leonardo Taglialegne <leonardotaglialegne+tylyos@gmail.com>
 * Copyright (C) 2008 Luca Salmin
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

#include "memory.h"
#include "gdtflags.h"

unsigned short segmentSelector(unsigned int index,char tableIndicator,char RPL){
    return ((index<<3)&0xFFF8)|((tableIndicator<<2)&0x4)|RPL;
}

void gdtSet(int num, unsigned long base, unsigned long limit, unsigned char gran, unsigned char access){
    gdt[num].baseLow = ( base & 0xFFFF );
    gdt[num].baseMiddle = ( base >> 16 ) & 0xFF;
    gdt[num].baseHigh = ( base >> 24 ) & 0xFF;

    gdt[num].limitLow = ( limit & 0xFFFF );
    gdt[num].granularity = ( ( limit >> 16 ) & 0x0F );
    gdt[num].granularity |= gran & 0xC0;
    gdt[num].access = access;
}

void initGdt(){
    gdtPointer.limit = ( sizeof ( struct gdtEntry ) * NUMERO_SEGMENTI ) - 1;
    gdtPointer.base = ( int ) &gdt;

    gdtSet ( 0, 0, 0, 0, 0 );
    gdtSet ( 1, 0, ( 0xFFFFFFFF ) /0x1000,MEM_GRANULAR|MEM_32,
             MEM_PRESENT|MEM_CODE_DATA|MEM_RW|MEM_KERNEL|MEM_CODE );
    gdtSet ( 2, 0, ( 0xFFFFFFFF ) /0x1000,MEM_GRANULAR|MEM_32,
             MEM_PRESENT|MEM_CODE_DATA|MEM_RW|MEM_KERNEL|MEM_DATA );

    segmentoCodiceKernel=segmentSelector ( 1,0,RPL_KERNEL );
    segmentoDatiKernel=segmentSelector ( 2,0,RPL_KERNEL );
    segmentoCodiceUser=segmentSelector ( 3,0,RPL_USER );
    segmentoDatiUser=segmentSelector ( 4,0,RPL_USER );

    gdtFlush ( segmentoCodiceKernel,segmentoDatiKernel );
}