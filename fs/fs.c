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

/*
file contenente tutte le funzioni base per accedere al file system indipendentemente dal dispositivo fisico e dal tipo di file system
la strutture fs_node_descriptor verrà usata per identificare un file od una artella aperta, ne verrà quindi passato l'id alle funzioni write,read ecc.
a loro volta le funzioni base richiameranno le funzioni del device specifico passando l'inode che identifica il nodo su quel device.
per trovare il device e l'inode a partire da un path verrà usata la funzione getNode del rootDevice che seguirà il path e se necessario richiamerà a sua volta la stessa funzione dei davice montati fintanto che una non ritorni l'inode ed il puntatore del device che lo gestisce.
*/

#include <fs/fs.h>