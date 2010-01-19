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
#include <memory/memory.h>
#include <lib/string.h>

void initDeviceFsManager()
{
    openNodeNumber=0;
    mountPointsNumber=0;
    /*TODO: inizializzare rootDeviceFs*/
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
        return rootDeviceFs;
    }
}

unsigned int openFile ( char *path,char mode )
{
    unsigned int id;
    id=getUnusedOpenNodeId();

    if ( id==0 ) /*raggiunto il numero massimo di nodi aperti*/
        return 0;

    struct fs_node_descriptor *nuovoNodo=kmalloc ( sizeof ( struct fs_node_descriptor ) );/*alloca un nuovo descrittore*/
    nuovoNodo->id=id;
    nuovoNodo->mode=mode;
    nuovoNodo->device=getDeviceFromPath ( path );/*cerca nei mount points quale device gestisce il path*/

    openNodes[openNodeNumber]=nuovoNodo;
    openNodeNumber++;

    return id;
}
