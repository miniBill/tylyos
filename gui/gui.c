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

#include "../drivers/screen/screen.h"

#include "gui.h"

void fillRectangle(unsigned int console, int x, int y, int width, int height, char color) {
  int c, i;
  for (c = x;c <= (width + x);c++)
    for (i = y;i <= (height + y);i++)
      put_color_xy(color, console, c, i);
}

void drawRectangle(unsigned int console, int x, int y, int width, int height, char color) {
  /* 8--1--2
   * |     |
   * 7     3
   * |     |
   * 6--5--4
   */
  int c;
  for (c = (x + 1);c < (width + x);c++) {
      /*1*/
      put_xy((char) BORDER_ORIZONTAL, console, c, y);
      /*5*/
      put_xy((char) BORDER_ORIZONTAL, console, c, y + height);
    }
  /*2*/
  put_xy((char) BORDER_CORNER_HI_RIGHT, console, x + width, y);
  for (c = (y + 1);c < (height + y);c++) {
      /*3*/
      put_xy((char) BORDER_VERTICAL, console, x + width, c);
      /*7*/
      put_xy((char) BORDER_VERTICAL, console, x, c);
    }
  /*4*/
  put_xy((char) BORDER_CORNER_LOW_RIGHT, console, x + width, y + height);
  /*6*/
  put_xy((char) BORDER_CORNER_LOW_LEFT, console, x, y + height);
  /*8*/
  put_xy((char) BORDER_CORNER_HI_LEFT, console, x, y);
  fillRectangle(console, x, y, width, height, color);
}
