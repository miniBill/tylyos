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

#include <config.txt>
#include <memory/memory.h>
#include <lib/string.h>
#include <kernel/kernel.h>
#include "ramFs.h"

unsigned int ramFs_private_getNodeCount(unsigned int directory)/*prendendo come parametro il primo cluster di una directory legge l'header e ritorna il numero di nodi che contiene*/
{
    unsigned int *ret=(unsigned int*)(&ramFs_clusters[ramFs_clusterSize*directory]);
    return *ret;
}

struct ramFs_node ramFs_private_getNode(unsigned int directory,unsigned int i)/*ritorna una struttura contenente i dati del nodo i*/
{
    struct ramFs_node ret;
    ret.cluster=0;/*indica errore*/
    ret.name[0]=0;
    ret.size=0;
    
    if(i>=ramFs_private_getNodeCount(directory))/*ritorna se l'indice è troppo alto*/
        return ret;
    else
    {
        if( i<(ramFs_clusterSize/(sizeof(struct ramFs_node)+4)) )/*se si trova nel primo cluster*/
        {
            char *temp=&ramFs_clusters[ramFs_clusterSize*directory];
            temp+=4;/*dimensione dell header contenuto nel primo cluster*/
            temp+=sizeof(struct ramFs_node)*i;
            
            struct ramFs_node *pointer=(struct ramFs_node*)temp;
            
            ret.cluster=pointer->cluster;
            ret.groupId=pointer->groupId;
            strcpy(pointer->name,ret.name);
            ret.permissions[0]=pointer->permissions[0];
            ret.permissions[1]=pointer->permissions[1];
            ret.permissions[2]=pointer->permissions[2];
            ret.size=pointer->size;
            ret.type=pointer->type;
            ret.userId=pointer->userId;
            
            return ret;
        }
        else
        {
            /*TODO: scorrere i clusters in modo da trovare la posizione*/
            return ret;
        }
    }
}

struct ramFs_node ramFs_private_getNodeFromName(unsigned int directory,char *name)/*ritorna una struttura contenente i dati del nodo i*/
{
    struct ramFs_node ret;
    ret.cluster=0;/*indica errore*/
    ret.name[0]=0;
    ret.size=0;
    
    for(unsigned int i=0;i<ramFs_private_getNodeCount(directory);i++)
    {
        if(strcmp(name,ramFs_private_getNode(directory,i).name)==0)
        {
            return ramFs_private_getNode(directory,i);
        }
    }
    return ret;
}

unsigned int ramFs_private_getCluster(unsigned int baseCluster,unsigned int i)/*segue la concatenazione dei clusters ritornando l'indice dell i esimo cluster*/
{/*TODO: testare*/
    unsigned int ret=baseCluster;
    for(unsigned int c=0;c<i;c++)
    {
        if(ramFs_FAT[ret]==RAMFS_FAT_EMPTY || ramFs_FAT[ret]==RAMFS_FAT_END)
        {
            kernelPanic("ramFs_private_getCluster()","the FAT table is inconsistent or the i argument wrong");
        }
        ret=ramFs_FAT[ret];
    }
    return ret;
}

struct deviceFs *newRamFs()
{
    struct deviceFs *pointer=kmalloc(sizeof(struct deviceFs));
    
    strcpy("ramFs",pointer->name);
    
    pointer->getNodeDescriptor = ramFs_getNodeDescriptor;
    pointer->getNodeInfo = ramFs_getNodeInfo;
#ifdef RAMFS_STUBS
    pointer->readFile = ramFs_readFile;
    pointer->writeFile = ramFs_writeFile;
    pointer->seek = ramFs_seek;
    pointer->readDir = ramFs_readDir;
    pointer->createFile = ramFs_createFile;
    pointer->deleteFile = ramFs_deleteFile;
    pointer->createDir = ramFs_createDir;
    pointer->deleteDir = ramFs_deleteDir;
#endif
    pointer->freeInodeInfoPointer = ramFs_freeInodeInfoPointer;
    
   
    ramFs_start=(unsigned int*)loadedModule;
    ramFs_FAT=(unsigned int*)(loadedModule+(4*2));
    
    ramFs_clusterNumber=ramFs_start[0];
    ramFs_clusterSize=ramFs_start[1];
    
    ramFs_clusters=(char*)&ramFs_FAT[ramFs_clusterNumber];
    
    printf(1,"ramFs DEBUG:\n  clusters count: %d\n  clusters size: %d Byte\n",ramFs_clusterNumber,ramFs_clusterSize);  
    printf(1,"  numero nodi in '/': %d\n",ramFs_private_getNodeCount(0));
    for(unsigned int c=0;c<ramFs_private_getNodeCount(0);c++)
    {
        struct ramFs_node nodo=ramFs_private_getNode(0,c);
        printf(1,"    %s %d Bytes cluster: %d tipo: %d\n",nodo.name,nodo.size,nodo.cluster,nodo.type);
    }
    return pointer;
}



void ramFs_getNodeDescriptor(struct deviceFs *device,struct fs_node_descriptor *descriptor,char *path)
{
    char nodeName[RAMFS_FILENAME_MAX_LENGTH];
    unsigned int depth=1;
    unsigned int directory=0;/*0 rappresenta la root directory*/
    int ret;
    descriptor->device=device;
    descriptor->inodeInfo=0;//TODO: cercare il path, allocare la struttura e passarne il puntatore
    
    printf(1,"\nGET NODE DESCRIPTOR: %s\n",path);
    
    ret=split(path,nodeName,RAMFS_FILENAME_MAX_LENGTH,'/',depth);
    depth++;
    
    while(ret>=0)/*finche' la split non ritorna -1 scorri il path*/
    {
        struct ramFs_node tmp=ramFs_private_getNodeFromName(directory,nodeName);/*cerca nella cartella il nodo*/
        
        printf(1,"ricerca:%d %s\n",depth-1,nodeName);
        
        if(tmp.cluster!=0)/*controlla se e' stato trovato*/
            printf(1,"    trovato>: %s %d\n",tmp.name,tmp.size);
        else
            return;
        
        if(tmp.type==FS_FILE)
        {
            char tmpChar[1];
            if(split(path,tmpChar,1,'/',depth+1)==-1)/*se e' l' ultimo pezzo del path*/
            {
                /*alloca l'inode info e lo ritorna*/
                printf(1,"YEAHHHHHHHHH!!!!!!!\n");
                
                struct ramFs_inodeInfo *info=kmalloc(sizeof(struct ramFs_inodeInfo));
                info->directoryCluster=directory;
                info->cluster=tmp.cluster;
                strcpy(nodeName,info->name);
                info->position=0;
                
                descriptor->inodeInfo=(void*)info;
                descriptor->type=tmp.type;
                
                return;
            }
            else
            {
                /*errore, un file viene trattato come fosse una directory*/
                return;
            }
        }
        
        if(tmp.type==FS_DIRECTORY)
        {
            directory=tmp.cluster;
            ret=split(path,nodeName,RAMFS_FILENAME_MAX_LENGTH,'/',depth);
            depth++;
            /*TODO: implementare l'apertura di un nodo di tipo directory*/
        }
    }
}
fs_returnCode ramFs_getNodeInfo(struct fs_node_descriptor *descriptor,struct fs_node_info *out)
{
    struct ramFs_inodeInfo *info=(struct ramFs_inodeInfo*)descriptor->inodeInfo;
    
    struct ramFs_node tmp=ramFs_private_getNodeFromName(info->directoryCluster,info->name);
    
    if(tmp.cluster!=0)
    {
        out->device=descriptor->device;
        out->groupId=tmp.groupId;
        strcpy(tmp.name,out->name);
        out->permissions[0]=tmp.permissions[0];
        out->permissions[1]=tmp.permissions[1];
        out->permissions[2]=tmp.permissions[3];
        out->size=tmp.size;
        out->type=tmp.type;
        out->userId=tmp.userId;
        return FS_OK;
    }
    else
        return FS_NOT_FOUND;

}

unsigned int ramFs_readFile(struct fs_node_descriptor *descriptor,char *buffer,unsigned int byteCount)
{
    //TODO: leggere i dati contenuti nel nodo, scriverli in buffer e ritornare il numero di bytes scritti
    struct ramFs_inodeInfo *info=(struct ramFs_inodeInfo*)descriptor->inodeInfo;
    unsigned int daLeggere=byteCount;
    unsigned int dimensione=ramFs_private_getNodeFromName(info->directoryCluster,info->name).size;
    unsigned int i=0,clusterI=0;;
    unsigned int cluster=info->cluster;
    
    if(info->position+byteCount > dimensione)/*se si stanno leggendo piu' byte di quelli rimasti nel file*/
    {
        daLeggere= dimensione - info->position;/*ricalcola i bytes da leggere*/
    }
    
    /*calcola il cluster da cui iniziare ed il clusterI iniziale*/
    cluster= ramFs_private_getCluster(info->cluster,info->position/ramFs_clusterSize);/*segue la concatenazione dei clusters fino a trovare l' indice del giusto cluster*/
    clusterI=info->position%ramFs_clusterSize;/*l' indice da cui partire nel cluster*/
    
    while(daLeggere>0)
    {
        buffer[i]=ramFs_clusters[(cluster*ramFs_clusterSize)+clusterI];
        i++;
        clusterI++;
        daLeggere--;
        
        if(clusterI==ramFs_clusterSize)/*se si sono letti tutti i dati del cluster*/
        {
            /*continua a leggere il cluster successivo*/
            if(ramFs_FAT[cluster] != RAMFS_FAT_EMPTY && ramFs_FAT[cluster] != RAMFS_FAT_END)/*se non c'e' un errore nel concatenamento*/
            {
                cluster=ramFs_FAT[cluster];
                clusterI=0;
            }
            else
            {
                kernelPanic("ramFs_readFile()","the FAT table is inconsistent or the node size is wrong");
            }
        }
        
    }
    
    return i;
    
}
#ifdef RAMFS_STUBS
unsigned int ramFs_writeFile(struct fs_node_descriptor *descriptor,char *buffer,unsigned int byteCount)
{
    //TODO: scrivere i dati contenuti in buffer nell' area dati del nodo e ritornare il numero di bytes scritti
}
unsigned int ramFs_seek(struct fs_node_descriptor *descriptor,int offset)
{
    //TODO: spostare il puntatore al byte da leggere/scrivere contenuto nella struttura inodeInfo
}
fs_returnCode ramFs_readDir(struct fs_node_descriptor *descriptor,struct fs_node_info *out)
{
    //TODO: scrivere in out i dati del prossimo nodo figlio del nodo descriptor
}
fs_returnCode ramFs_createFile(char *name,struct fs_node_descriptor *output,struct fs_node_descriptor fatherNodeDescriptor)
{
}
fs_returnCode ramFs_deleteFile(struct fs_node_descriptor descriptor)
{
}
fs_returnCode ramFs_createDir(char *name,struct fs_node_descriptor *output,struct fs_node_descriptor fatherNodeDescriptor)
{
}
fs_returnCode ramFs_deleteDir(struct fs_node_descriptor descriptor)
{
}
#endif
void ramFs_freeInodeInfoPointer(void *inodeInfo)
{
    kfree(inodeInfo);
}

