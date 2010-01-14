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
#ifndef FS_H
#define FS_H

#define FS_FILENAME_MAX_LENGTH 128

#define FS_FILE       0x01 /* 0001*/
#define FS_DIRECTORY  0x02 /* 0010*/
#define FS_SYMLINK    0x03 /* 0011*/
#define FS_PIPE       0x04 /* 0100*/
#define FS_MOUNTPOINT 0x08 /* 1000 in modo da poter esseere settato in aggiunta agli altri flags*/

struct deviceFs
{
  char name[500];/*in modo da mappare il device nella cartella /dev*/
  /*TODO: inserire i puntatori alle funzioniper lettura scrittura apertura e chiusura*/
};

/*
struttura contenente tutte le informazioni di un nodo
deve essere usata solo temporaneamente come valore di ritorno delle funzioni
il device infatti ha le proprie strutture in cui tiene i dati dei propri nodi e non c'è qundi bisogno di tenere questa struttura in ram
*/
struct fs_node
{
  char name[FS_FILENAME_MAX_LENGTH];/*nome del file*/
  char permissions[3];/*i permessi relativi al proprietario, al gruppo ed agli altri utenti*/
  unsigned int userId;/*lo user id del proprietario del file*/
  unsigned int groupId;/*l' id del gruppo a cui appartiene il file*/
  char type;/*il tipo del nodo, vedere le definizioni*/
  unsigned int inode;/*numero univoco utilizzato dal device per identificare il nodo*/
  unsigned int size;/*dimensione del file in bytes*/
  struct deviceFs *device;/*device che è stato identificato come gestore del nodo*/
};

/*
struttura usata per identificare univocamente un nodo che deve però essere stato già aperto
l'identificazione univoca è data dalla coppia device:inode che forniscono al kernel il modo di identificare univocamente il nodo
*/
struct fs_node_descriptor
{
  struct deviceFs *device;/*device che è stato identificato come gestore del nodo*/
  unsigned int inode;/*id del nodo sul device, questo id è unico a livello di device, può quindi ripetersi ma su device diversi in quanto insieme al device identifica univocamente un nodo*/
  unsigned int id;/*id univoco del nodo, viene generato automaticamente all apertura del file o della directory*/
};

#endif