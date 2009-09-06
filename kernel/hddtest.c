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

#include "kernel.h"
#include <drivers/hdd/ata.h>
#include <lib/string.h>

int checkHdd(int controller,int disk){
	int present=0;
	int t=row()+1;
	int x=0;
	NO(t);
	greendot();
	present=isHddPresent(controller,disk);
	printf("Rilevamento presenza   hdd: %s %s\n",disk?"slave     ":"master    ",present?"presente":"assente");
	for(x=42;x<50;x++)
		cputxy(x,t,present?Light_Green:Light_Red);
	OK(t);
	if(present){
		int hddtype=0;
		t=row()+1;
		NO(t);
		greendot();
		printf("Identificazione:");
		hddtype=identifyHdd(controller,disk);
		if(hddtype & 1) /*1 o 3, ata or atapi*/
			OK(t);
	}
	return present;
}

int checkController(int controller){
	int present=0;
	int ret=0;
	int t=row()+1;
	int x=0;
	NO(t);
	greendot();
	present=isControllerPresent(controller);
	printf("Rilevamento controller hdd: %s %s\n",controller?"secondario":"primario  ",present?"presente":"assente");
	OK(t);
	for(x=42;x<50;x++)
		cputxy(x,t,present?Light_Green:Light_Red);
	if(!present)
		return 0;
	ret|=8;
	ret|=checkHdd(controller,0)<<2;
	ret|=checkHdd(controller,1)<<1;
	return ret;
}

int checkHdds(void){
	int ret=0;
	ret|=checkController(0);
	ret|=checkController(1)<<4;
	return ret;
}
