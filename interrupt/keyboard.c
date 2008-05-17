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

unsigned char kmode   = 0;

char ScanCodeToChar(char scode)
{
	unsigned char ch;

    	if (kmode & ALTGR)
	{
		ch=alt_map[sc];
    	}
	else if (kmode & (LSHIFT|RSHIFT|LCTRL|RCTRL))
	{
        	ch=shift_map[sc];
    	}
	else
	{
        	ch=key_map[sc];
    	}

	if (ch == 0)
	{
		return '\0';
	}
	return ch;
}
