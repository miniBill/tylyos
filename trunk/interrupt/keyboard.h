/* Copyright (C) 2008 Luca Salmin
 * Copyright (C) 2008 Leonardo Taglialegne <leonardotaglialegne+clearos@gmail.com>
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

#ifndef KEYBOARD_H_
#define KEYBOARD_H_

char ScanCodeToChar(char scode);

/* tastiera italiana */
#define KBD_IT

#define LSHIFT   0x01
#define RSHIFT   0x02
#define LCTRL    0x04
#define RCTRL    0x08
#define ALT      0x10
#define ALTGR    0x20
#define CAPS     0x40
#define CAPSDOWN 0x80

char * key_map;
char * shift_map;
char * alt_map;

#endif

