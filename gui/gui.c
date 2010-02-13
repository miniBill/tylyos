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

#include "gui.h"
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



/**
* CREATE THE REGISTER ARRAY TAKEN FROM http://wiki.osdev.org/VGA_Hardware
*/
unsigned char g_640x480x16[] =
{
    /* MISC */
    0xE3,
    /* SEQ */
    0x03, 0x01, 0x08, 0x00, 0x06,
    /* CRTC */
    0x5F, 0x4F, 0x50, 0x82, 0x54, 0x80, 0x0B, 0x3E,
    0x00, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0xEA, 0x0C, 0xDF, 0x28, 0x00, 0xE7, 0x04, 0xE3,
    0xFF,
    /* GC */
    0x00, 0x00, 0x00, 0x00, 0x03, 0x00, 0x05, 0x0F,
    0xFF,
    /* AC */
    0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x14, 0x07,
    0x38, 0x39, 0x3A, 0x3B, 0x3C, 0x3D, 0x3E, 0x3F,
    0x01, 0x00, 0x0F, 0x00, 0x00
};

unsigned char mode_320_200_256[]={
    /* MISC
    *
    * 0x63 => 01100011
    * 7 6 5 4 3 2 1 0
    * 1 1 0 0 0 1 1 0
    * VSP HSP - - CS CS ERAM IOS
    * 7,6 - 480 lines
    * 5,4 - free
    * 3,2 - 28,322 MHZ Clock
    * 1 - Enable Ram
    * 0 - Map 0x3d4 to 0x3b4
    */
    0x63,
    /* SEQ */
    /**
    * index 0x00 - Reset
    * 0x03 = 11
    * Bits 1,0 Synchronous reset
    */
    0x03,
    /**
    * index 0x01
    * Clocking mode register
    * 8/9 Dot Clocks
    */
    0x01,
    /**
    * Map Mask Register, 0x02
    * 0x0F = 1111
    * Enable all 4 Maps Bits 0-3
    * chain 4 mode
    */
    0x0F,
    /**
    * map select register, 0x03
    * no character map enabled
    */
    0x00,
    /**
    * memory mode register 0x04
    * enables ch4,odd/even,extended memory
    */
    0x0E,
    /* CRTC */
    0x5F, 0x4F, 0x50, 0x82, 0x54, 0x80, 0xBF, 0x1F,
    0x00, 0x41, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x9C, 0x0E, 0x8F, 0x28,   0x40, 0x96, 0xB9, 0xA3,
    0xFF,
    /* GC */
    0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x05, 0x0F,
    0xFF,
    /* AC */
    0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
    0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F,
    0x41, 0x00, 0x0F, 0x00,   0x00
};

void write_registers(unsigned char *regs){
    unsigned i;
    
    /* write MISCELLANEOUS reg */
    outb(VGA_MISC_WRITE, *regs);
    regs++;
    /* write SEQUENCER regs */
    for(i = 0; i < VGA_NUM_SEQ_REGS; i++)
    {
        outb(VGA_SEQ_INDEX, i);
        outb(VGA_SEQ_DATA, *regs);
        regs++;
    }
    /* unlock CRTC registers */
    outb(VGA_CRTC_INDEX, 0x03);
    outb(VGA_CRTC_DATA, inb(VGA_CRTC_DATA) | 0x80);
    outb(VGA_CRTC_INDEX, 0x11);
    outb(VGA_CRTC_DATA, inb(VGA_CRTC_DATA) & ~0x80);
    /* make sure they remain unlocked */
    regs[0x03] |= 0x80;
    regs[0x11] &= ~0x80;
    /* write CRTC regs */
    for(i = 0; i < VGA_NUM_CRTC_REGS; i++)
    {
        outb(VGA_CRTC_INDEX, i);
        outb(VGA_CRTC_DATA, *regs);
        regs++;
    }
    /* write GRAPHICS CONTROLLER regs */
    for(i = 0; i < VGA_NUM_GC_REGS; i++)
    {
        outb(VGA_GC_INDEX, i);
        outb(VGA_GC_DATA, *regs);
        regs++;
    }
    /* write ATTRIBUTE CONTROLLER regs */
    for(i = 0; i < VGA_NUM_AC_REGS; i++)
    {
        (void)inb(VGA_INSTAT_READ);
        outb(VGA_AC_INDEX, i);
        outb(VGA_AC_WRITE, *regs);
        regs++;
    }
    
    /* lock 16-color palette and unblank display */
    (void)inb(VGA_INSTAT_READ);
    outb(VGA_AC_INDEX, 0x20);
}

/**
* Clears the VGA screen
*/
void VGA_clear_screen(){
    unsigned int x=0;
    unsigned int y=0;
    
    for(y=0; y<VGA_height; y++)
        for(x=0; x<VGA_width; x++)
            VGA_address[VGA_width*y+x]=13;
}

static void draw_face(void){
  unsigned int x=0;
  unsigned int y=0;
  for(y=40;y<70;y++)
    for(x=40;x<100;x++){
      VGA_address[VGA_width*y+x]=0x1;
      VGA_address[VGA_width*y+x+180]=0x1;
    }

  for(y=70;y<130;y++)
    for(x=230-y;x<90+y;x++)
      VGA_address[VGA_width*y+x]=0x1;
  for(y=150;y<190;y++)
    for(x=40;x<280;x++)
      VGA_address[VGA_width*y+x]=0x1;

}

void VGA_writeChar(char ch,unsigned int _x,unsigned int _y)
{
    unsigned int c=0;
    for(unsigned int y=0; y<8; y++)
    {
        for(unsigned int x=0; x<8; x++)
        {
            if( getBitExt(g_8x8_font,((8*8)*ch)+c)==1)
            {
                if(_x+(8-x)<VGA_width && _y+y<VGA_height)
                    VGA_address[VGA_width*(_y+y)+(_x+(8-x))]=80;
            }
            c++;
        }
    }
}

void VGA_writeString(char *s,unsigned int x,unsigned int y)
{
    unsigned int count=strlen(s);
    for(unsigned int c=0;c<count;c++)
        VGA_writeChar(s[c],x+7*c,y);
}

void VGA_setPalette()
{
    for(unsigned int c=0;c<256;c++)
    {
        outb(0x3c8,c);
        outb(0x3c9,palette[(c*3)]);
        outb(0x3c9,palette[(c*3)+1]);
        outb(0x3c9,palette[(c*3)+2]);
    }
}

/**
* Note here the vga struct must have the width 320 and height of 200
* color mode is 256
*/
void VGA_init(int width, int height, int bpp){
    //setup the vga struct
    VGA_width=(unsigned int)width;
    VGA_height=(unsigned int)height;
    VGA_bpp=bpp;
    VGA_address=(unsigned char*)0xA0000;
    
    //enables the mode 13 state
    write_registers(mode_320_200_256);
    
    /*modifica la plaette*/
    VGA_setPalette();
    
    //clears the screen
    VGA_clear_screen();

    draw_face();
}