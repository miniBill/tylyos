/* Copyright (C) 2008 Leonardo Taglialegne <leonardotaglialegne+clearos@gmail.com>
 * Copyright (C) 2008 Luca Salmin
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
#include "multiboot.h"
#include "screen.h"
#include "stdio.h"
#include "../memory/memory.h"

void OK(int i){
	putxy(78,i,'O');
	putxy(79,i,'K');
	cwritexy(78,i,0x10|Light_Green);
	cwritexy(79,i,0x10|Light_Green);
}
short abs(short s){return s<0?-s:s;}

void _kmain(multiboot_info_t* mbd, unsigned int magic){
	char * parameters="Parametri: \0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0";
	char * pointer="Prova puntatore";
	char * conversion="\0\0\0\0\0\0\0\0\0";
	short i;/*index*/
	int d=0;/*for delay loop*/
	int t=0;/*test number*/
	clearScreen();
	OK(t++);
	writeline("Prova writeline");
	OK(t++);
	writeline(pointer);
	OK(t++);
	write("Prova strapp(output: 101|C|A0):");
	strapp(conversion,"%b|",(void *)5);
	strapp(conversion,"%x|",(void *)12);
	strapp(conversion,"%x",(void *)160);
	writeline(conversion);
	OK(t++);
	strapp(parameters,"mbd.flags:%b|",(void *)mbd->flags);
	strapp(parameters,"magic:%d",(void *)magic);
	writeline(parameters);
	for(i=0;i<6;i++)
		put(read(i));
	writeline("put/read");
	OK(t++);
	writeline("Prova GDT");
	init_gdt();
	OK(t++);
	writeline("Ed ora, diamo il via alle danze!");
	OK(t++);
	i=0;
	while(1){
		for(d=0;d<10000;d++)
			;
		putxy(abs(i),t,' ');
		putxy(80-abs(i),t+1,' ');
		putxy(abs(i),t+2,' ');
		putxy(80-abs(i),t+3,' ');
		putxy(abs(i),t+4,' ');
		putxy(80-abs(i),t+5,' ');
		putxy(abs(i),t+6,' ');
		putxy(80-abs(i),t+7,' ');
		putxy(abs(i),t+8,' ');
		putxy(80-abs(i),t+9,' ');
		if(i==79)
			i=-78;
		else
			i++;
		putxy(abs(i),t,'*');
		putxy(80-abs(i),t+1,'*');
		putxy(abs(i),t+2,'*');
		putxy(80-abs(i),t+3,'*');
		putxy(abs(i),t+4,'*');
		putxy(80-abs(i),t+5,'*');
		putxy(abs(i),t+6,'*');
		putxy(80-abs(i),t+7,'*');
		putxy(abs(i),t+8,'*');
		putxy(80-abs(i),t+9,'*');
	}
}

