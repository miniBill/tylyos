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

#include "ataatapi.h"
#include <kernel/stdio.h>
#include <kernel/kernel.h>

int isControllerPresent(int controller){
	int port=port(3);
	outb(port,0x88);
	if(inb(port)==0x88)
		return 1;
	return 0;
}

int isHddPresent(int controller, int hdd){
	short tmpword;
	if(!isControllerPresent(controller))
		return 0;
	
	outb(port(6), hdd?0xB0:0xA0);
	sleep(1);
	tmpword = inb(port(7));
	return (tmpword & 0x40)>0;
}

int identifyHdd(int controller, int hdd){
	short tmpword;
	short retval=0;
	outb(port(6), hdd?0xB0:0xA0);
	sleep(1);
	outb(port(7), 0xEC); /*IDENTIFY*/
	tmpword=inb(port(7));
	if((tmpword & 0x40) == 0)
		return 0;
	while(1){
		tmpword=inb(port(7));
		if(
			((tmpword & 0x80) == 0) && 
			((tmpword & 0x08) != 0) &&
			((tmpword & 0x01) == 0))
			break;
		if(tmpword & 0x01){
			tmpword=inb(port(4));
			if(tmpword == 0x14){ /*probably ATAPI*/
				tmpword=inb(port(5));
				if(tmpword == 0xEB){
					printf("ATAPI ");
					retval=1;
					break;
				}
			}
			if(tmpword == 0x3C){ /*probably SATA*/
				tmpword=inb(port(4));
				if(tmpword == 0xC3){
					printf("SATA\n");
					retval=3;
					break;
				}
			}
			if(tmpword == 0x00)/*real error*/
				printf("Glub!");
			printf("Errore! [prima read] [contr. %d,hdd %d]\n",controller,hdd);
			return 2;
		}
		sleep(1);
	}
	{
		int data[256];
		int i=0;
		long j=0;
		for(i=0;i<256;i++)
			data[i]=inw(port(0));
		for(i=100;i<104;i++){
			j|=data[i];
			if(i!=103)
				j<<=16;
		}
		i=(data[60]<<16)|data[61];
		if(i!=0 || j!=0){
			printf("0x%x settori [contr. %d,drive %d] LBA%d UDMA(%x,%x)\n",(j>0)?j:i,controller,hdd,(j>0)?48:28,
			data[88]&0xFF,(data[88]&0xFF00)>>8);
			return retval != 0 ? retval : 5;
		}
		j=0;
		for(i=0;i<256;i++)
			if(data[i]!=0)
				j++;
		printf("Errore! [dopo read] [contr. %d,hdd %d] %d nonzero\n",controller,hdd,j);
		return 4;
	}
}

void selectHdd(int controller,int hdd){
	outb(port(6), hdd<<4);
}

void setMode(int controller,int dma){
	outb(port(1),dma);
}