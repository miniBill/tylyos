/* Copyright (C) 2008 Leonardo Taglialegne <leonardotaglialegne+tylyos@gmail.com>
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

#include "ata.h"
#include "ataatapi.h"
#include <kernel/stdio.h>
#include <kernel/kernel.h>
#include <lib/string.h>

#define BochsBreak() outw(0x8A00,0x8A00); outw(0x8A00,0x08AE0);

void readSector ( int controller, int hdd, int sector, unsigned char buffer[512] )
{
    int i=0;
    short word=0;
    /*char byte,pbyte;*/
    outb ( port ( 6 ),0xE0| ( hdd<<4 ) | ( ( sector>>24 ) &0x0F ) );/*last 4 bits + drive + some magic*/
    /*outb(port+1,0x00);*//*NULL byte*//*commented, cause 't should be useless*/
    outb ( port ( 2 ),0x01 );/*sector count*/
    outb ( port ( 3 ), ( unsigned char ) sector );/*low 8 bits*/
    outb ( port ( 4 ), ( unsigned char ) ( sector >> 8 ) );/*next 8 bits*/
    outb ( port ( 5 ), ( unsigned char ) ( sector >> 16 ) );/*next 8 bits*/
    outb ( port ( 7 ),0x20 );/*read!*/
    /*wait till it's ready*/
    /*while(!((byte=inb(port+7))&0x80)){
    	sleep(1000);
    	if(byte!=pbyte){
    		printf("%d,",byte);
    		pbyte=byte;
    	}
    }*/
    sleep ( 5000 );/*HACK*/
    for ( i=0;i<256;i++ )
    {
        word=inw ( port ( 0 ) );
        buffer[i*2]= ( unsigned char ) word;
        buffer[i*2+1]= ( unsigned char ) ( word>>8 );
    }
}

void writeSector ( int controller, int hdd, int sector, unsigned char buffer[512] )
{
    int i=0;
    short word=0;
    outb ( port ( 1 ),0x00 );/*NULL byte*/
    outb ( port ( 2 ),0x01 );/*sector count*/
    outb ( port ( 3 ), ( unsigned char ) sector );/*low 8 bits*/
    outb ( port ( 4 ), ( unsigned char ) ( sector >> 8 ) );/*next 8 bits*/
    outb ( port ( 5 ), ( unsigned char ) ( sector >> 16 ) );/*next 8 bits*/
    outb ( port ( 6 ),0xE0| ( hdd<<4 ) | ( ( sector>>24 ) &0x0F ) );/*last 4 bits + drive + some magic*/
    outb ( port ( 7 ),0x30 );/*write!*/
    while ( ! ( inb ( port ( 7 ) ) &0x08 ) )
    {
        sleep ( 1000 );    /*wait till it's ready*/
    }
    for ( i=0;i<512;i++ )
    {
        word=buffer[i*2]|buffer[i*2+1]<<8;
        outw ( port ( 0 ),word );
    }
}
