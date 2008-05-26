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

#include "gui.h"
void DrawRectangleExt(int x,int y,int width,int height,char color)
{
    int c,i;
    DrawRectangle(x,y,width,height);
    for(c=0;c<=width;c++)
        for(i=0;i<=height;i++)
            cputxy(x+c,y+i,color);
}

void DrawRectangle(int x,int y,int width,int height)
{
    int c;
    for(c=0;c<width;c++)
    {
        putxy(x+c,y,(char)BORDER_ORIZONTAL);
    }
    putxy(x,y,(char)BORDER_CORNER_HI_LEFT);
    putxy(x+width,y,(char)BORDER_CORNER_HI_RIGHT);
    for(c=1;c<height;c++)
    {
        putxy(x,y+c,(char)BORDER_VERTICAL);
        putxy(x+width,y+c,(char)BORDER_VERTICAL);
    }
    for(c=0;c<width;c++)
    {
        putxy(x+c,y+height,(char)BORDER_ORIZONTAL);
    }
    putxy(x,y+height,(char)BORDER_CORNER_LOW_LEFT);
    putxy(x+width,y+height,(char)BORDER_CORNER_LOW_RIGHT);
}

