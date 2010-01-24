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

#endif
