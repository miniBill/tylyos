/* Copyright (C) 2008 Luca Salmin
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

#include "stdio.h"

unsigned char inb(int portnum){
    unsigned char data=0;
    asm("inb %%dx, %%al" : "=a" (data) : "d" (portnum));
    return data;
}

void outb(int data, int portnum){
    asm("outb %%al, %%dx" :: "d" (data),"a" (portnum));
}

void io_wait(void){
    asm("jmp 1f;1:jmp 1f;1:");
}
