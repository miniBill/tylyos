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

