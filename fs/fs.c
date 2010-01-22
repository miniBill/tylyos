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
*/

#include <fs/fs.h>
#include <fs/ramFs/ramFs.h>
#include <memory/memory.h>
#include <lib/string.h>

void initDeviceFsManager()
{
    openNodeNumber=0;
    mountPointsNumber=0;
    
    rootDeviceFs= newRamFs();
}

unsigned int getUnusedOpenNodeId()/*TODO: testare il funzionamento*/
{
    if ( openNodeNumber==MAX_OPEN_NODES ) /*se si e' raggiunto il limite di nodi aperti*/
        return 0;

    unsigned int id=1;
    while ( 1 )
    {
        char trovato=0;
        for ( unsigned int c=0;c<openNodeNumber;c++ )
        {
            if ( openNodes[c]->id==id )
                trovato=1;
        }

        if ( trovato==0 )
            return id;

        id++;
    }
}

struct deviceFs *getDeviceFromPath ( char *path )/*TODO: testare il funzionamento*/
{
    unsigned int max=0;
    struct deviceFs *pointer=0;
    for ( unsigned int c=0;c<mountPointsNumber;c++ )/*passa tutti i mount points*/
    {
        unsigned int matched=0;
        unsigned i=0;
        if ( strlen ( mountPoints[c]->path ) <= strlen ( path ) )/*se il mount point e' piu' corto del path*/
        {
            while ( mountPoints[c]->path[i]==path[i] && path[i]!=0 && mountPoints[c]->path[i]!=0 )/*controlla quanti caratteri ha in comune con il path*/
            {
                matched++;
                i++;
            }
            if ( matched>max && matched == strlen ( mountPoints[c]->path ) )/*se sono stati trovati piu' caratteri in comune rispetto all' ultimo trovato e il mount e' stato comunque trovato per intero nel path*/
            {
                max=matched;
                pointer=mountPoints[c]->device;
            }
        }
    }
    
    if(pointer!=0)
    {
        return pointer;
    }
    else
    {
        return rootDeviceFs;/*se nessun mount point fa riferimento a quel path utilizza il device della directory root*/
    }
}

unsigned int openFile ( char *path,char mode )/*TODO: inserire un controllo sulla modalita' di apertura*/
{
    unsigned int id;
    id=getUnusedOpenNodeId();

    if ( id==0 ) /*raggiunto il numero massimo di nodi aperti*/
        return 0;

    struct fs_node_descriptor *nuovoNodo=kmalloc ( sizeof ( struct fs_node_descriptor ) );/*alloca un nuovo descrittore*/
    nuovoNodo->id=id;
    nuovoNodo->mode=mode;
    nuovoNodo->device=getDeviceFromPath ( path );/*cerca nei mount points quale device gestisce il path*/
    
    nuovoNodo->device->getNodeDescriptor(nuovoNodo->device,nuovoNodo,path);/*carica in nuovoNodo il puntatore alle informazioni dell inode*/
    struct fs_node_info info = nuovoNodo->device->getNodeInfo(nuovoNodo);
    
    if(nuovoNodo->inodeInfo==0 || !(info.type & FS_FILE) )/*se il nodo non e' stato trovato o non e' un file*/
    {
        kfree(nuovoNodo);
        return 0;
    }
    
    /*salva il nodo appena aperto nella lista*/
    openNodes[openNodeNumber]=nuovoNodo;
    openNodeNumber++;

    return id;/*ritorna l'id in modo che possa essere usato per richiamare read write e close*/
}

void closeFile(File file)/*TODO: testare*/
{
    struct fs_node_descriptor *pointer=0;
    
    for(unsigned int c=0;c<openNodeNumber;c++)/*passa tutti i nodi aperti*/
    {
        if(openNodes[c]->id == file)/*se l'id corrisponde*/
        {
            pointer=openNodes[c];/*salva il puntatore per dopo*/
            /*cancella dalla lista*/
            while(c+1<openNodeNumber)
            {
                openNodes[c]=openNodes[c+1];
                c++;
            }
            openNodeNumber--;
        }
    }
    
    if(pointer==0)
        return;
    
    pointer->device->freeInodeInfoPointer(pointer->inodeInfo);/*dealloca la struttura dell inode*/
    kfree(pointer);/*dealloca il descrittore*/
}

unsigned int readFile(File file,char *buffer,unsigned int byteCount)/*TODO: testare*/
{
    struct fs_node_descriptor *pointer=0;
    /*cerca il descrittore del file aperto*/
    for(unsigned int c=0;c<openNodeNumber;c++)
    {
        if(openNodes[c]->id==file)
        {
            pointer=openNodes[c];
        }
    }
    
    if(pointer==0 || !(pointer->type&FS_FILE))/*se non e' aperto o non e' un file*/
        return 0;
    
    return pointer->device->readFile(pointer,buffer,byteCount);
}

unsigned int writeFile(File file, char* buffer, unsigned int byteCount)/*TODO: testare*/
{
    struct fs_node_descriptor *pointer=0;
    /*cerca il descrittore del file aperto*/
    for(unsigned int c=0;c<openNodeNumber;c++)
    {
        if(openNodes[c]->id==file)
        {
            pointer=openNodes[c];
        }
    }
    
    if(pointer==0 || !(pointer->type&FS_FILE))/*se non e' aperto o non e' un file*/
        return 0;
    
    return pointer->device->writeFile(pointer,buffer,byteCount);
}

unsigned int seek(File file,int offset)/*TODO: testare*/
{
    struct fs_node_descriptor *pointer=0;
    /*cerca il descrittore del file aperto*/
    for(unsigned int c=0;c<openNodeNumber;c++)
    {
        if(openNodes[c]->id==file)
        {
            pointer=openNodes[c];
        }
    }
    
    if(pointer==0 || !(pointer->type&FS_FILE))/*se non e' aperto o non e' un file*/
        return 0;
    
    return pointer->device->seek(pointer,offset);
}

fs_returnCode addMountPoint(char *path,struct deviceFs *device)
{
    if(mountPointsNumber==MAX_MOUNT)
        return FS_GENERIC_ERROR;
    
    struct mountPoint *pointer=kmalloc(sizeof(struct mountPoint));
    strcpy(pointer->path,path);
    pointer->device=device;
    
    mountPoints[mountPointsNumber]=pointer;
    mountPointsNumber++;
    
    return FS_OK;
}