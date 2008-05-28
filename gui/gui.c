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

#include "../drivers/screen/screen.h"

#include "gui.h"

void fillRectangle(int x,int y,int width,int height,char color){
    int c,i;
    for(c=x;c<=(width+x);c++)
        for(i=y;i<=(height+y);i++)
            cputxy(c,i,color);
}

void drawRectangle(int x,int y,int width,int height,char color){
    /* 8--1--2
     * |     |
     * 7     3
     * |     |
     * 6--5--4
     */
    int c;
    for(c=(x+1);c<(width+x);c++){
        /*1*/
        putxy(c,y,(char)BORDER_ORIZONTAL);
        /*5*/
        putxy(c,y+height,(char)BORDER_ORIZONTAL);
    }
    /*2*/
    putxy(x+width,y,(char)BORDER_CORNER_HI_RIGHT);
    for(c=(y+1);c<(height+y);c++){
        /*3*/
        putxy(x+width,c,(char)BORDER_VERTICAL);
        /*7*/
        putxy(x,c,(char)BORDER_VERTICAL);
    }
    /*4*/
    putxy(x+width,y+height,(char)BORDER_CORNER_LOW_RIGHT);
    /*6*/
    putxy(x,y+height,(char)BORDER_CORNER_LOW_LEFT);
    /*8*/
    putxy(x,y,(char)BORDER_CORNER_HI_LEFT);
    fillRectangle(x,y,width,height,color);
}
