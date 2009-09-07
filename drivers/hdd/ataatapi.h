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

#ifndef ATAATAPI_H_
#define ATAATAPI_H_

#include <lib/string.h>

#define port(q) ((controller?0x170:0x1F0)+q)

int isControllerPresent(int controller);

int isHddPresent(int controller,int hdd);

/*
Returns:
0 no hdd
1 atapi
2 error before read
3 sata
4 error after read
5 ata
*/
int identifyHdd(int controller,int hdd);

void selectHdd(int controller,int hdd);

void setMode(int controller,int dma);

#endif