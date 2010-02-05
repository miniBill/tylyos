/* Copyright (C) 2010 Leonardo Taglialegne <leonardotaglialegne+tylyos@gmail.com>
* Copyright (C) 2010 Luca Salmin
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

#include <kernel/stdio.h>
#include <drivers/timer/timer.h>

/*TODO: testare le funzioni*/

void sound(short freq)
{

    unsigned short int freqdiv;
    unsigned char tmp;
    freqdiv = 1193180 / freq;
    outb(0x43, 0xB6);
    outb(0x42, (char) freqdiv);       /* first bits 0-7 */
    outb(0x42, (char) (freqdiv >> 8));/* then bits 8-15 */
    tmp = inb(0x61);
    if (tmp != (tmp | 3))   /* only output if bits are not correctly set */
        outb(0x61, tmp | 3);
}

void nosound()
{
    unsigned char tmp;
    tmp = inb(0x61);  /* KB controller port B */
    tmp &= 0xFC;      /* disable speaker + clock 2 gate */
    outb(0x61, tmp);  /* output */
}

void beep()
{
    sound(1000);
    sleep(250);
    nosound();
}