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

#include <config.txt>
#include <fs/fs.h>
#include <fs/ramFs/ramFs.h>
#include <memory/memory.h>
#include <lib/string.h>
#include <fs/pipe.h>

void initDeviceFsManager()
{
    openNodeNumber=0;
    mountPointsNumber=0;
    rootDeviceFs= newRamFs();
}

unsigned int getUnusedOpenNodeId(unsigned int procID)/*TODO: testare il funzionamento*/
{
    if ( openNodeNumber==MAX_OPEN_NODES ) /*se si e' raggiunto il limite di nodi aperti*/
        return 0;

    int id=1;
    while ( 1 )
    {
        char trovato=0;
        for ( unsigned int c=0;c<openNodeNumber;c++ )
        {
            if ( openNodes[c]->id==id && openNodes[c]->procID==procID )
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

File openFile (unsigned int procID, char *path,char mode )/*TODO: inserire un controllo sulla modalita' di apertura*/
{
    unsigned int id;
    id=getUnusedOpenNodeId(procID);

    if ( id==0 ) /*raggiunto il numero massimo di nodi aperti*/
        return -1;

    struct fs_node_descriptor *nuovoNodo=kmalloc ( sizeof ( struct fs_node_descriptor ) );/*alloca un nuovo descrittore*/
    nuovoNodo->id=id;
    nuovoNodo->type=FS_FILE;
    nuovoNodo->procID=procID;
    nuovoNodo->mode=mode;
    nuovoNodo->device=getDeviceFromPath ( path );/*cerca nei mount points quale device gestisce il path*/
    
    nuovoNodo->device->getNodeDescriptor(nuovoNodo->device,nuovoNodo,path);/*carica in nuovoNodo il puntatore alle informazioni dell inode*/
    struct fs_node_info info;
    nuovoNodo->device->getNodeInfo(nuovoNodo,&info);
    /*TODO: ATTENZIONEEEE, RICORDARSI DI SPLITTARE IL PATH IN QUALCHE MODO IN BASE AL MOUNT POINT!!!!!!*/
    
    if(nuovoNodo->inodeInfo==0 || info.type != FS_FILE )/*se il nodo non e' stato trovato o non e' un file*/
    {
        kfree(nuovoNodo);
        return -1;
    }
    
    /*salva il nodo appena aperto nella lista*/
    openNodes[openNodeNumber]=nuovoNodo;
    openNodeNumber++;

    return id;/*ritorna l'id in modo che possa essere usato per richiamare read write e close*/
}

void closeFile(unsigned int procID,File file)/*TODO: testare*/
{
    struct fs_node_descriptor *pointer=0;
    
    for(unsigned int c=0;c<openNodeNumber;c++)/*passa tutti i nodi aperti*/
    {
        if(openNodes[c]->id == file && openNodes[c]->procID==procID)/*se l'id corrisponde*/
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
    if(pointer->type!=FS_PIPE)  
        pointer->device->freeInodeInfoPointer(pointer->inodeInfo);/*dealloca la struttura dell inode*/
    else/*se e' una pipe deve togliere anche l'altro descrittore*/
    {
        for(unsigned int c=0;c<openNodeNumber;c++)/*passa tutti i nodi aperti*/
        {   
            if(openNodes[c]->inodeInfo==pointer->inodeInfo)/*se punta alla stessa pipe*/
            {
                kfree(openNodes[c]);
                /*cancella dalla lista*/
                while(c+1<openNodeNumber)
                {
                    openNodes[c]=openNodes[c+1];
                    c++;
                }
                openNodeNumber--;
            }
        }
    }
    kfree(pointer->inodeInfo);
    kfree(pointer);/*dealloca il descrittore*/
}

void pipe(unsigned int procID,File descriptors[2])/*alloca due descrittori, uno in lettura ed uno in scrittura, di tipo PIPE*/
{
    /*TODO: scrivere la funzione*/
    /*alloca i due descrittori, uno in lettura ed uno in scrittura*/
    unsigned int idLettura,idScrittura;



    struct fs_node_descriptor *nodoLettura=kmalloc ( sizeof ( struct fs_node_descriptor ) );/*alloca un nuovo descrittore*/
    struct fs_node_descriptor *nodoScrittura=kmalloc ( sizeof ( struct fs_node_descriptor ) );/*alloca un nuovo descrittore*/
    nodoLettura->type=FS_PIPE;
    nodoScrittura->type=FS_PIPE;
    nodoLettura->procID=procID;
    nodoScrittura->procID=procID;
    /*TODO: implementare i controlli su lettura e scrittura*/
    nodoLettura->mode=0;
    nodoScrittura->mode=0;
    /*li collega alla pipe*/
    struct pipe *p=createPipe();
    nodoLettura->inodeInfo=(void*)p;
    nodoScrittura->inodeInfo=(void*)p;


    /*genera l'id ed inserisce nella lista il descrittore*/
    idLettura=getUnusedOpenNodeId(procID);
printf(1,">>>>>%d\n",idLettura);
    if ( idLettura==0 ) /*raggiunto il numero massimo di nodi aperti*/
    {
        kfree(nodoLettura);
        kfree(nodoScrittura);
        return;
    }
    nodoLettura->id=idLettura;

    openNodes[openNodeNumber]=nodoLettura;
    openNodeNumber++;

    /*genera l'id ed inserisce nella lista il descrittore*/
    idScrittura=getUnusedOpenNodeId(procID);
printf(1,">>>>>%d\n",idScrittura);
    if ( idScrittura==0 ) /*raggiunto il numero massimo di nodi aperti*/
    {
        kfree(nodoLettura);
        kfree(nodoScrittura);
        return;
    }
    nodoScrittura->id=idScrittura;

    openNodes[openNodeNumber]=nodoScrittura;
    openNodeNumber++;

    descriptors[0]=nodoLettura->id;
    descriptors[1]=nodoScrittura->id;
}

unsigned int readFile(unsigned int procID,File file,char *buffer,unsigned int byteCount)/*TODO: testare*/
{
    struct fs_node_descriptor *pointer=0;
    /*cerca il descrittore del file aperto*/
    for(unsigned int c=0;c<openNodeNumber;c++)
    {
        if(openNodes[c]->id==file && openNodes[c]->procID==procID)
        {
            pointer=openNodes[c];
        }
    }
    

    if(pointer==0)/*se non e' aperto*/
        return 0;
      
    if(pointer->type&FS_FILE)
        return pointer->device->readFile(pointer,buffer,byteCount);

    if(pointer->type&FS_PIPE)
        return readOnPipe((struct pipe*)pointer->inodeInfo,buffer,byteCount);   
    return 0;
}

unsigned int writeFile(unsigned int procID,File file, char* buffer, unsigned int byteCount)/*TODO: testare*/
{
    struct fs_node_descriptor *pointer=0;
    /*cerca il descrittore del file aperto*/
    for(unsigned int c=0;c<openNodeNumber;c++)
    {
        if(openNodes[c]->id==file && openNodes[c]->procID==procID)
        {
            pointer=openNodes[c];
        }
    }
    
    if(pointer==0)/*se non e' aperto*/
        return 0;
    
    if(pointer->type&FS_FILE)
        return pointer->device->writeFile(pointer,buffer,byteCount);

    if(pointer->type&FS_PIPE)
        return writeOnPipe((struct pipe*)pointer->inodeInfo,buffer,byteCount);

    return 0;
}

unsigned int seek(unsigned int procID,File file,int offset)/*TODO: testare*/
{
    struct fs_node_descriptor *pointer=0;
    /*cerca il descrittore del file aperto*/
    for(unsigned int c=0;c<openNodeNumber;c++)
    {
        if(openNodes[c]->id==file && openNodes[c]->procID==procID)
        {
            pointer=openNodes[c];
        }
    }
    

    if(pointer==0)/*se non e' aperto*/
        return 0;
      
    if(pointer->type&FS_FILE)
        return pointer->device->seek(pointer,offset);

    return 0;
}

unsigned int fileSize(unsigned int procID,File file)
{
    struct fs_node_descriptor *pointer=0;
    /*cerca il descrittore del file aperto*/
    for(unsigned int c=0;c<openNodeNumber;c++)
    {
        if(openNodes[c]->id==file && openNodes[c]->procID==procID)
        {
            pointer=openNodes[c];
        }
    }
    
    if(pointer==0 || !(pointer->type&FS_FILE))/*se non e' aperto o non e' un file*/
        return 0;
    
    struct fs_node_info info;
    pointer->device->getNodeInfo(pointer,&info);
    return info.size;
}

File openDir(unsigned int procID,char *path)
{
    unsigned int id;
    id=getUnusedOpenNodeId(procID);
    
    if ( id==0 ) /*raggiunto il numero massimo di nodi aperti*/
        return -1;
    
    struct fs_node_descriptor *nuovoNodo=kmalloc ( sizeof ( struct fs_node_descriptor ) );/*alloca un nuovo descrittore*/
    nuovoNodo->id=id;
    nuovoNodo->procID=procID;
    nuovoNodo->device=getDeviceFromPath ( path );/*cerca nei mount points quale device gestisce il path*/
    
    nuovoNodo->device->getNodeDescriptor(nuovoNodo->device,nuovoNodo,path);/*carica in nuovoNodo il puntatore alle informazioni dell inode*/
    struct fs_node_info info;
    nuovoNodo->device->getNodeInfo(nuovoNodo,&info);
    /*TODO: ATTENZIONEEEE, RICORDARSI DI SPLITTARE IL PATH IN QUALCHE MODO IN BASE AL MOUNT POINT!!!!!!*/
    
    if(nuovoNodo->inodeInfo==0 || info.type != FS_DIRECTORY )/*se il nodo non e' stato trovato o non e' un file*/
    {
        kfree(nuovoNodo);
        return -1;
    }
    
    /*salva il nodo appena aperto nella lista*/
    openNodes[openNodeNumber]=nuovoNodo;
    openNodeNumber++;
    
    return id;/*ritorna l'id in modo che possa essere usato per richiamare read write e close*/
}

fs_returnCode readDir(unsigned int procID,File dir,struct fs_node_info *out)
{
    struct fs_node_descriptor *pointer=0;
    /*cerca il descrittore della cartella*/
    for(unsigned int c=0;c<openNodeNumber;c++)
    {
        if(openNodes[c]->id==dir && openNodes[c]->procID==procID)
        {
            pointer=openNodes[c];
        }
    }
    
    if(pointer==0 || pointer->type!=FS_DIRECTORY)/*se non e' aperto o non e' una directory*/
        return FS_NOT_FOUND;
    
    return pointer->device->readDir(pointer,out);
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

/*permette di assegnare ad un descittore aperto un novo id e di assegnarlo ad un altro task*/
void moveNodeDescriptor(unsigned int procID,File id,unsigned int newProcID,File newId)
{
    struct fs_node_descriptor *pointer=0;
    /*cerca il descrittore aperto*/
    for(unsigned int c=0;c<openNodeNumber;c++)
    {
        if(openNodes[c]->id==id && openNodes[c]->procID==procID)
        {
            pointer=openNodes[c];
        }
    }

    if(pointer==0)/*se non e' aperto*/
        return;

    pointer->id=newId;
    pointer->procID=newProcID;
}

void closeAllNodeDescriptors(unsigned int procID)
{
    char t;

    do/*passa la lista e finche' ne trova elimina idescrittori legati al task specificato*/
    {
        t=0;
        for(unsigned int c=0;c<openNodeNumber;c++)
        {
            if( openNodes[c]->procID==procID )
            {
                closeFile(openNodes[c]->procID,openNodes[c]->id);
                t=1;
                break;
            }
        }
    }while(t);
}
