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

#include <fs/fs.h>

struct deviceFs *newRamFs();/*ritorna un puntatore ad una nuova struttura deviceFs*/

void ramFs_getNodeDescriptor(struct deviceFs *device,struct fs_node_descriptor *descriptor,char *path);/*scrive nella struttura il device e l'inode*/
fs_returnCode ramFs_getNodeInfo(struct fs_node_descriptor *descriptor,struct fs_node_info *out);/*ritorna le informazioni riguardanti ad un nodo*/
unsigned int ramFs_readFile(struct fs_node_descriptor *descriptor,char *buffer,unsigned int byteCount);
unsigned int ramFs_writeFile(struct fs_node_descriptor *descriptor,char *buffer,unsigned int byteCount);
unsigned int ramFs_seek(struct fs_node_descriptor *descriptor,int offset);
fs_returnCode ramFs_readDir(struct fs_node_descriptor *descriptor,struct fs_node_info *out);
fs_returnCode ramFs_createFile(char *name,struct fs_node_descriptor *output,struct fs_node_descriptor fatherNodeDescriptor);/*scrive nel parametro output le informazioni del nodo*/
fs_returnCode ramFs_deleteFile(struct fs_node_descriptor descriptor);
fs_returnCode ramFs_createDir(char *name,struct fs_node_descriptor *output,struct fs_node_descriptor fatherNodeDescriptor);
fs_returnCode ramFs_deleteDir(struct fs_node_descriptor descriptor);
void ramFs_freeInodeInfoPointer(void *inodeInfo);/*dealloca la struttura che era stata allocata in getNodeDescriptor, serve in quanto ogni deviceFs usa strutture di dimensione diversa*/

#define RAMFS_FILENAME_MAX_LENGTH 128
#define RAMFS_FAT_EMPTY 0xFFFFFFFF
#define RAMFS_FAT_END   0xFFFFFFFE

struct ramFs_node
{
    char name[RAMFS_FILENAME_MAX_LENGTH];/*nome del file*/
    char permissions[3];/*i permessi relativi al proprietario, al gruppo ed agli altri utenti*/
    unsigned int userId;/*lo user id del proprietario del file*/
    unsigned int groupId;/*l' id del gruppo a cui appartiene il file*/
    char type;/*il tipo del nodo, vedere le definizioni*/
    unsigned int size;/*dimensione del file in bytes*/
    
    unsigned int cluster;/*indice che specifica i cluster con cui inizia il nodo*/
};

unsigned int *ramFs_start;/*indirizzo di partenza del ramFs*/
unsigned int ramFs_clusterNumber;
unsigned int ramFs_clusterSize;
unsigned int *ramFs_FAT;/*indirizzo della FAT*/
char *ramFs_clusters;/*indirizzo dove partono i clusters*/

#endif
