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

#include "atapi.h"
#include "ataatapi.h"
#include <kernel/stdio.h>
#include <kernel/kernel.h>

void atapiRead ( int controller,int hdd,short * command,int maxByteCount )
{
    short tmpword=0;
    selectHdd ( controller,hdd );
    setMode ( controller,0 );

    /*send byte count*/
    outb ( port ( 4 ),maxByteCount & 0xff );
    outb ( port ( 5 ),maxByteCount >> 8 );
    tmpword=inw ( port ( 7 ) );
    while ( ( tmpword & 0x40 ) || ! ( tmpword & 0x20 ) )
    {
        printf (0, "BSY " );//HACK
        sleep ( 1000 );
    }
    outb ( port ( 7 ),0xA0 );/*PACKET*/
    for ( tmpword=0;tmpword<6;tmpword++ )
        outw ( port ( 0 ),command[tmpword] );
}
