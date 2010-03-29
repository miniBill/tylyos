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
#include <lib/string.h>
#include <kernel/stdio.h>
#include <kernel/kernel.h>

#include <gui/gui.h>
#include "vga.h"
#include "palette.h"


//define the ports , taken from http://files.osdev.org/mirrors/geezer/osd/graphics/modes.c
#define   VGA_AC_INDEX      0x3C0
#define   VGA_AC_WRITE      0x3C0
#define   VGA_AC_READ         0x3C1
#define   VGA_MISC_WRITE      0x3C2
#define VGA_SEQ_INDEX      0x3C4
#define VGA_SEQ_DATA      0x3C5
#define   VGA_DAC_READ_INDEX   0x3C7
#define   VGA_DAC_WRITE_INDEX   0x3C8
#define   VGA_DAC_DATA      0x3C9
#define   VGA_MISC_READ      0x3CC
#define VGA_GC_INDEX       0x3CE
#define VGA_GC_DATA       0x3CF
#define VGA_CRTC_INDEX      0x3D4      /* 0x3B4 */
#define VGA_CRTC_DATA      0x3D5      /* 0x3B5 */
#define   VGA_INSTAT_READ      0x3DA
#define   VGA_NUM_SEQ_REGS   5
#define   VGA_NUM_CRTC_REGS   25
#define   VGA_NUM_GC_REGS      9
#define   VGA_NUM_AC_REGS      21
#define   VGA_NUM_REGS      (1+VGA_NUM_SEQ_REGS+VGA_NUM_CRTC_REGS+VGA_NUM_GC_REGS+VGA_NUM_AC_REGS)

unsigned char mode_320_200_256[]={
    0x63, 0x03, 0x01, 0x0F, 0x00, 0x0E,
    0x5F, 0x4F, 0x50, 0x82, 0x54, 0x80, 0xBF, 0x1F,
    0x00, 0x41, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x9C, 0x0E, 0x8F, 0x28, 0x40, 0x96, 0xB9, 0xA3,
    0xFF,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x05, 0x0F,
    0xFF,
    0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
    0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F,
    0x41, 0x00, 0x0F, 0x00, 0x00
};

void write_registers(unsigned char *regs){
    unsigned i;
    
    outb(VGA_MISC_WRITE, *regs);
    regs++;
    for(i = 0; i < VGA_NUM_SEQ_REGS; i++){
        outb(VGA_SEQ_INDEX, i);
        outb(VGA_SEQ_DATA, *regs);
        regs++;
    }
    outb(VGA_CRTC_INDEX, 0x03);
    outb(VGA_CRTC_DATA, inb(VGA_CRTC_DATA) | 0x80);
    outb(VGA_CRTC_INDEX, 0x11);
    outb(VGA_CRTC_DATA, inb(VGA_CRTC_DATA) & ~0x80);
    regs[0x03] |= 0x80;
    regs[0x11] &= ~0x80;
    for(i = 0; i < VGA_NUM_CRTC_REGS; i++){
        outb(VGA_CRTC_INDEX, i);
        outb(VGA_CRTC_DATA, *regs);
        regs++;
    }
    for(i = 0; i < VGA_NUM_GC_REGS; i++){
        outb(VGA_GC_INDEX, i);
        outb(VGA_GC_DATA, *regs);
        regs++;
    }
    for(i = 0; i < VGA_NUM_AC_REGS; i++){
        (void)inb(VGA_INSTAT_READ);
        outb(VGA_AC_INDEX, i);
        outb(VGA_AC_WRITE, *regs);
        regs++;
    }
    (void)inb(VGA_INSTAT_READ);
    outb(VGA_AC_INDEX, 0x20);
}


void VGA_clear_screen(){
    unsigned int x,y;
    for(y=0; y<VGA_height; y++)
        for(x=0; x<VGA_width; x++)
            VGA_address[VGA_width*y+x]=gui_background;
}

void VGA_setPalette(){
    outb(0x03C6,0xff);
    outb(0x3c8,0);
    for(unsigned int c=0;c<256;c++){
        outb(0x3c9,palette[(c*3)]/4);
        outb(0x3c9,palette[(c*3)+1]/4);
        outb(0x3c9,palette[(c*3)+2]/4);
    }
}

void VGA_init(int width, int height, int bpp){
    VGA_width=(unsigned int)width;
    VGA_height=(unsigned int)height;
    VGA_bpp=bpp;
    
    VGA_address=(unsigned char*)0xA0000;
    
    write_registers(mode_320_200_256);
    VGA_setPalette();
    VGA_clear_screen();
    go_graphic();
}
