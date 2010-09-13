/* Copyright (C) 2008 Luca Salmin
 * Copyright (C) 2008 Leonardo Taglialegne <leonardotaglialegne+tylyos@gmail.com>
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

/*
file contenente tutte le funzioni base per accedere al file system indipendentemente dal dispositivo fisico e dal tipo di file system
*/
#ifndef FS_H
#define FS_H

#define FS_FILENAME_MAX_LENGTH 128

#define FS_FILE       0x01 /* 0001*/
#define FS_DIRECTORY  0x02 /* 0010*/
#define FS_SYMLINK    0x03 /* 0011*/
#define FS_PIPE       0x04 /* 0100*/

#include <drivers/screen/screen.h>

typedef char fs_returnCode;

#define FS_OK               0x00
#define FS_NOT_FOUND        0x01
#define FS_GENERIC_ERROR    0x02
#define FS_END              0x03/*usato per readDir*/

typedef int File;

/*
struttura contenente tutte le informazioni di un nodo
deve essere usata solo temporaneamente come valore di ritorno delle funzioni
il device infatti ha le proprie strutture in cui tiene i dati dei propri nodi e non c'è qundi bisogno di tenere questa struttura in ram
*/
struct fs_node_info
{
    char name[FS_FILENAME_MAX_LENGTH];/*nome del file*/
    char permissions[3];/*i permessi relativi al proprietario, al gruppo ed agli altri utenti*/
    unsigned int userId;/*lo user id del proprietario del file*/
    unsigned int groupId;/*l' id del gruppo a cui appartiene il file*/
    char type;/*il tipo del nodo, vedere le definizioni*/
    unsigned int size;/*dimensione del file in bytes*/
    struct deviceFs *device;/*device che è stato identificato come gestore del nodo*/
};

File openFile(unsigned int procID,char *path,char mode);/*cerca il device e l'inode su cui si trova il file, alloca la truttura e ne ritorna l'id per i successivi utilizzi*/
void closeFile(unsigned int procID,File file);/*dealloca il fs_node_descriptor allocato con la precedente open*/
void pipe(unsigned int procID,File descriptors[2]);/*alloca due descrittori, uno in lettura ed uno in scrittura, di tipo PIPE*/

unsigned int readFile(unsigned int procID,File file,char *buffer,unsigned int byteCount);
unsigned int writeFile(unsigned int procID,File file,char *buffer,unsigned int byteCount);
unsigned int seek(unsigned int procID,File file,int offset);
unsigned int fileSize(unsigned int procID,File file);

File openDir(unsigned int procID,char *path);
void closeDir(unsigned int procID,File dir);

fs_returnCode readDir(unsigned int procID,File dir,struct fs_node_info *out);

fs_returnCode createFile(char *path);
fs_returnCode createDir(char *path);

fs_returnCode deleteFile(char *path);
fs_returnCode deleteDir(char *path);



/*
struttura usata per identificare univocamente un nodo che deve però essere stato già aperto
l'identificazione univoca è data dalla coppia device:inode che forniscono al kernel il modo di identificare univocamente il nodo
*/
struct fs_node_descriptor
{
  struct deviceFs *device;/*device che è stato identificato come gestore del nodo*/
  void *inodeInfo;/*puntatore alla struttura allocata dal device contenente le informazioni del nodo*/
  char mode;/*il modo con cui e' stato aperto*/
  File id;/*id univoco del nodo, viene generato automaticamente all apertura del file o della directory*/
  unsigned int procID;/*l'id del task a cui e' assegnato*/
  char type;
};

#define MAX_OPEN_NODES 50


struct fs_node_descriptor *openNodes[MAX_OPEN_NODES];
unsigned int openNodeNumber;/*il numero di nodi aperti contenuti nell array openNodes*/

/*ogni device allochera' un istanza di questa struttura inserendo i puntatori alle proprie funzioni*/
struct deviceFs
{
  char name[500];/*in modo da mappare il device nella cartella /dev*/
  void *additionalDataStruct;/*puntatore ad una struttura aggiuntiva usata per salvare i dati in base al tipo di device*/
 
  void (*getNodeDescriptor)(struct deviceFs *device,struct fs_node_descriptor *descriptor,char *path);/*scrive nella struttura il device e l'inode*/
  fs_returnCode (*getNodeInfo)(struct fs_node_descriptor *descriptor,struct fs_node_info *out);/*ritorna le informazioni riguardanti ad un nodo*/
  unsigned int (*readFile)(struct fs_node_descriptor *descriptor,char *buffer,unsigned int byteCount);
  unsigned int (*writeFile)(struct fs_node_descriptor *descriptor,char *buffer,unsigned int byteCount);
  unsigned int (*seek)(struct fs_node_descriptor *descriptor,int offset);
  fs_returnCode (*readDir)(struct fs_node_descriptor *descriptor,struct fs_node_info *out);
  fs_returnCode (*createFile)(char *name,struct fs_node_descriptor *output,struct fs_node_descriptor fatherNodeDescriptor);/*scrive nel parametro output le informazioni del nodo*/
  fs_returnCode (*deleteFile)(struct fs_node_descriptor descriptor);
  fs_returnCode (*createDir)(char *name,struct fs_node_descriptor *output,struct fs_node_descriptor fatherNodeDescriptor);
  fs_returnCode (*deleteDir)(struct fs_node_descriptor descriptor);
  void (*freeInodeInfoPointer)(void *inodeInfo);/*dealloca la struttura che era stata allocata in getNodeDescriptor, serve in quanto ogni deviceFs usa strutture di dimensione diversa*/
};

struct deviceFs *rootDeviceFs;/*puntatore al device che monta la root del file system */

struct deviceFs *getDeviceFromPath(char *path);/*scorre la lista dei mount points e ritorna il device che gestisce il path*/

/*struttura usata per matchare i path e trovare al primo colpo il device che gestisce un nodo avendone il path, verrà caricata in fase di boot da un file*/
struct mountPoint
{
  char path[5000];
  struct deviceFs *device;
};

#define MAX_MOUNT 20

struct mountPoint *mountPoints[MAX_MOUNT];
unsigned int mountPointsNumber;/*numero di mount presenti nell array mountPoints*/

void initDeviceFsManager();
unsigned int getUnusedOpenNodeId(unsigned int procID);
fs_returnCode addMountPoint(char *path,struct deviceFs *device);

/*permette di assegnare ad un descittore aperto un novo id e di assegnarlo ad un altro task*/
void moveNodeDescriptor(unsigned int procID,File id,unsigned int newProcID,File newId);

void copyNodeDescriptor(unsigned int procID,File id,unsigned int newProcID,File newId);

void closeAllNodeDescriptors(unsigned int procID);

/*modifica l'id di un file descriptor, in caso il nuovo fd sia gia' usato elimina il vecchio*/
File movFileDescriptorTo(unsigned int procID,File file,File newfile);


#endif
