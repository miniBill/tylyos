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

#ifndef TASK_H
#define TASK_H

typedef struct tss
{
    unsigned int        link;
    unsigned int        esp0;
    unsigned short      ss0, __ss0h;
    unsigned int        esp1;
    unsigned short      ss1, __ss1h;
    unsigned int        esp2;
    unsigned short      ss2, __ss2h;
    unsigned int        cr3;
    unsigned int        eip;
    unsigned int        eflags;
    unsigned int        eax, ecx, edx, ebx;
    unsigned int        esp;
    unsigned int        ebp;
    unsigned int        esi;
    unsigned int        edi;
    unsigned short      es, __esh;
    unsigned short      cs, __csh;
    unsigned short      ss, __ssh;
    unsigned short      ds, __dsh;
    unsigned short      fs, __fsh;
    unsigned short      gs, __gsh;
    unsigned short      ldtr, __ldtrh;
    unsigned short      trace, io_map_addr;
} tss_t;

struct task
{
    unsigned int procID;
    char stato;
    char nome[20];
};


void initTaskManagement();

#endif

