/* Copyright (C) 2008 Leonardo Taglialegne <leonardotaglialegne+clearos@gmail.com>
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

#include "ata.h"
#include <kernel/stdio.h>
#include <kernel/kernel.h>
#include <lib/string.h>

#define BochsBreak() outw(0x8A00,0x8A00); outw(0x8A00,0x08AE0);

int isControllerPresent(int controller){
	int port=controller?0x173:0x1F3;
	outb(port,0x88);
	if(inb(port)==0x88)
		return 1;
	return 0;
}

int isHddPresent(int controller, int hdd){
	short tmpword;
	int port=controller?0x176:0x1F6;
	if(!isControllerPresent(controller))
		return 0;
	
	outb(port, hdd?0xB0:0xA0);
	sleep(1);
	tmpword = inb(port+1);
	return (tmpword & 0x40)>0;
}

/*
Returns:
0 no hdd
1 atapi
2 error before read
3 sata
4 error after read
5 ata
*/
int identifyHdd(int controller, int hdd){
	short tmpword;
	int port=controller?0x176:0x1F6;
	outb(port, hdd?0xB0:0xA0);
	sleep(1);
	outb(port+1, 0xEC); /*IDENTIFY*/
	tmpword=inb(port+1);
	if((tmpword & 0x40) == 0)
		return 0;
	while(1){
		tmpword=inb(port+1);
		if(
			((tmpword & 0x80) == 0) && 
			((tmpword & 0x08) != 0) &&
			((tmpword & 0x01) == 0))
			break;
		if(tmpword & 0x01){
			tmpword=inb(port-2);
			if(tmpword == 0x14){ /*probably ATAPI*/
				tmpword=inb(port-1);
				if(tmpword == 0xEB){
					printf("ATAPI\n");
					return 1;
				}
			}
			if(tmpword == 0x3C){ /*probably SATA*/
				tmpword=inb(port-2);
				if(tmpword == 0xC3){
					printf("SATA\n");
					return 3;
				}
			}
			if(tmpword == 0x00){ /*real error*/
				printf("Glub!");
			}
			printf("Errore![prima read][controller %d,hdd %d]\n",controller,hdd);
			return 2;
		}
		sleep(1);
	}
	{
		int data[256];
		int i=0;
		long j=0;
		for(i=0;i<256;i++)
			data[i]=inw(port-6);
		for(i=100;i<104;i++){
			j|=data[i];
			if(i!=103)
				j<<=16;
		}
		i=(data[60]<<16)|data[61];
		if(i!=0){
			printf("%d settori [controller %d,drive %d] LBA%d UDMA(%x,%x)\n",(j>0)?j:i,controller,hdd,(j>0)?48:28,
			data[88]&0xFF,(data[88]&0xFF00)>>8);
			return 5;
		}
		printf("Errore![dopo read][controller %d,hdd %d]",controller,hdd);
		return 4;
	}
}

void readSector(int controller, int hdd, int sector, unsigned char buffer[512]){
	int port=controller?0x170:0x1F0;
	int i=0;
	short word=0;
	/*char byte,pbyte;*/
	outb(port+6,0xE0|(hdd<<4)|((sector>>24)&0x0F));/*last 4 bits + drive + some magic*/
	/*outb(port+1,0x00);*//*NULL byte*//*commented, cause 't should be useless*/
	outb(port+2,0x01);/*sector count*/
	outb(port+3,(unsigned char)sector);/*low 8 bits*/
	outb(port+4,(unsigned char)(sector >> 8));/*next 8 bits*/
	outb(port+5,(unsigned char)(sector >> 16));/*next 8 bits*/
	outb(port+7,0x20);/*read!*/
	/*wait till it's ready*/
	/*while(!((byte=inb(port+7))&0x80)){
		sleep(1000);
		if(byte!=pbyte){
			printf("%d,",byte);
			pbyte=byte;
		}
	}*/
	sleep(5000);/*HACK*/
	for(i=0;i<256;i++){
		word=inw(port);
		buffer[i*2]=(unsigned char)word;
		buffer[i*2+1]=(unsigned char)(word>>8);
	}
}

void writeSector(int controller, int hdd, int sector, unsigned char buffer[512]){
	int port=controller?0x170:0x1F0;
	int i=0;
	short word=0;
	outb(port+1,0x00);/*NULL byte*/
	outb(port+2,0x01);/*sector count*/
	outb(port+3,(unsigned char)sector);/*low 8 bits*/
	outb(port+4,(unsigned char)(sector >> 8));/*next 8 bits*/
	outb(port+5,(unsigned char)(sector >> 16));/*next 8 bits*/
	outb(port+6,0xE0|(hdd<<4)|((sector>>24)&0x0F));/*last 4 bits + drive + some magic*/
	outb(port+7,0x30);/*write!*/
	while(!(inb(port+7)&0x08)){sleep(1000);}/*wait till it's ready*/
	for(i=0;i<512;i++){
		word=buffer[i*2]|buffer[i*2+1]<<8;
		outw(port,word);
	}
}
