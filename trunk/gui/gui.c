/* Copyright (C) 2008 Leonardo Taglialegne <leonardotaglialegne+tylyos@gmail.com>
 * Copyright (C) 2008 Luca Salmin
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

#include <drivers/screen/screen.h>
#include <lib/string.h>
#include <kernel/stdio.h>
#include <memory/memory.h>
#include <lib/string.h>
#include <kernel/stdio.h>
#include <kernel/kernel.h>

#include <drivers/screen/vga.h>

#include "mandelbrot.h"
#include "gui.h"
#include "font.h"


void VGA_writeChar(char ch,unsigned int _x,unsigned int _y)
{
    unsigned int c=0;
    for(unsigned int y=0; y<8; y++)
    {
        for(unsigned int x=0; x<8; x++)
        {
            if( getBitExt((unsigned int*)g_8x8_font,((8*8)*ch)+c)==1)
            {
                if(_x+(8-x)<VGA_width && _y+y<VGA_height)
                    VGA_address[VGA_width*(_y+y)+(_x+(8-x))]=0;
            }
            c++;
        }
    }
}

void VGA_writeString(char *s,unsigned int x,unsigned int y)
{
    unsigned int count=strlen(s);
    
    for(unsigned int c=0;c<count;c++)
    {
        VGA_writeChar(s[c],x+7*c,y);
    }
}


