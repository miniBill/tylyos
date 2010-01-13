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

#ifndef RAMFS_H
#define RAMFS_H



#define RAMFS_BLOCK_SIZE 512

/*struttura he rappresenta un blocco in memoria, i blocchi sono concetenati e rappresentano i dati di un file*/
struct ramfs_block
{
    struct ramfs_block *next;/*prossimo blocco*/
    unsigned short used;/*numero di byte contenuti nel blocco*/
    char dati[RAMFS_BLOCK_SIZE];/*buffer che contiene i dati*/
} __attribute__ ( ( packed ) );

#endif
