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
#include "../interrupt/interrupt.h"

void OK(int i){
    putxy(COLUMNS-2,i,'O');
    putxy(COLUMNS-1,i,'K');
    cwritexy(COLUMNS-2,i,Light_Green);
    cwritexy(COLUMNS-1,i,Light_Green);
}

short abs(short s){return s<0?-s:s;}

void _kmain(multiboot_info_t* mbd, unsigned int magic){
    char * parameters="Parametri: \0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0";
    char * pointer="Prova puntatore";
    char * conversion="\0\0\0\0\0\0\0\0\0";
    short i;/*index*/
    int t=0;/*test number*/

    clearScreen();
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
    strapp(parameters,"magic:%x",(void *)magic);
    writeline(parameters);
    OK(t++);
    for(i=0;i<6;i++)
        put(read(i));
    writeline("put/read");
    OK(t++);
    writeline("Prova GDT");
    init_gdt();
    OK(t++);
    writeline("Prova IDT");
    OK(t++);
    initIDT();
    OK(t++);
    asm("int $0x20");/* interrupt 32 per provare il funzionamento */
    writeline("Ed ora, diamo il via alle danze!");
    OK(t++);
    while(1);
}

