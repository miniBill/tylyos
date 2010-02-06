#include <sys/types.h>
#include <dirent.h>
#include <errno.h>
#include <vector>
#include <string.h>
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <fstream>

#define RAMFS_FILENAME_MAX_LENGTH 128

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

#define RAMFS_FAT_EMPTY 0xFFFFFFFF
#define RAMFS_FAT_END   0xFFFFFFFE

#define FS_FILE       0x01 /* 0001*/
#define FS_DIRECTORY  0x02 /* 0010*/
#define FS_SYMLINK    0x03 /* 0011*/
#define FS_PIPE       0x04 /* 0100*/

using namespace std;

unsigned int clusterSize;
unsigned int clusterNumber;

unsigned int imageSize;

char *fileSystem;
unsigned int *FAT;
char *clusters;

unsigned int rootDirectory;/*indice del cluster dove si trova la root*/

int conta(string s)
{
    int n=0;
    for(int c=0;c<s.length();c++)
        if(s.c_str()[c]=='/')
            n++;
    return n;
}

#include <sys/types.h> 

#include <sys/stat.h> 

int FileSize( string szFileName ) 
{ 
    struct stat fileStat; 
    int err = stat( szFileName.c_str(), &fileStat ); 
    if (0 != err) return 0; 
    return fileStat.st_size; 
}

/*scrive nella directory il numero di files che contiene*/
void writeDirectoryHeader(unsigned int cluster,unsigned int numFile)
{
    unsigned int *h=(unsigned int*)&clusters[clusterSize*cluster];
    *h=numFile;
}
/*ritorna il numero di files che una directory contiene*/
unsigned int readDirectoryHeader(unsigned int cluster)
{
    unsigned int *h=(unsigned int*)&clusters[clusterSize*cluster];
    return *h;
}

/*alloca e ritorna l'indice di un nuovo cluster*/
unsigned int getFreeCluster()
{
    unsigned int c=0;
    while(FAT[c]!=RAMFS_FAT_EMPTY && c<clusterNumber)
        c++;
    
    if(c<clusterNumber)
    {
        FAT[c]=RAMFS_FAT_END;/*segna che e' usato*/
        return c;
    }
    else
    {
        cout<<"file system pieno";
        exit(1);
    }
}

/*scrive in una directory gia' esistente un nuovo nodo*/
void addNodeToDirectory(unsigned int cluster,struct ramFs_node nodeStruct)
{
    unsigned int numeroNodi=readDirectoryHeader(cluster);
    
    if(4 + (numeroNodi*sizeof(struct ramFs_node)) + sizeof(struct ramFs_node) <= clusterSize)/*se c'e' spazio nel primo cluster della directory*/
    {
        struct ramFs_node *nuovoNodo;
        nuovoNodo=(struct ramFs_node*)((&clusters[clusterSize*cluster]) + 4);
        nuovoNodo=&nuovoNodo[numeroNodi];
        
        nuovoNodo->cluster=nodeStruct.cluster;
        nuovoNodo->groupId=nodeStruct.groupId;
        strcpy(nuovoNodo->name,nodeStruct.name);
        nuovoNodo->permissions[0]=nodeStruct.permissions[0];
        nuovoNodo->permissions[1]=nodeStruct.permissions[1];
        nuovoNodo->permissions[2]=nodeStruct.permissions[2];
        nuovoNodo->size=nodeStruct.size;
        nuovoNodo->type=nodeStruct.type;
        nuovoNodo->userId=nodeStruct.userId;
        
        /*aggiorna il numero di nodi*/
        writeDirectoryHeader(cluster,numeroNodi+1);
    }
    else
    {
        cout<<"cluster directory pieno, implementare questo caso";
        exit(1);
    }
}

void writeFile(unsigned int startCluster,string path)
{
    ifstream fileIn;
    fileIn.open(path.c_str());
    
    char *data=new char[clusterSize];
    
    unsigned int oldCluster=startCluster;
    while(!fileIn.eof())
    {
        fileIn.read(data,clusterSize);
        for(int c=0;c<clusterSize;c++)
        {
            clusters[(oldCluster*clusterSize) + c]=data[c];
        }
        if(!fileIn.eof())
        {
            unsigned int newCluster=getFreeCluster();
            FAT[oldCluster]=newCluster;/*concatena*/
            oldCluster=newCluster;
        }
    }
    fileIn.close();
}

int getdir (string dir,unsigned int parent)
{
    DIR *dp;
    struct dirent *dirp;
    if((dp  = opendir(dir.c_str())) == NULL)
    {
        cerr << "Error(" << errno << ") opening " << dir << endl;
        return errno;
    }
   
    
    string space="";
    int ct=conta(string(dir))+1;//evito di calcolarlo ad ogni ciclo :)
    for(int c=0;c<ct;c++)
        space=space+"    ";
    
    while ((dirp = readdir(dp)) != NULL)
    {
        if(dirp->d_type!=DT_DIR)
        {
            /*se e' un file*/
           
            
            /*alloca un nuovo nodo di tipo file*/
            unsigned int newFile;
            newFile=getFreeCluster();
            writeDirectoryHeader(newFile,0);
            
            /*inserisci la nuova directory come figlio della directory in cui si trova*/
            struct ramFs_node newFileStruct;
            newFileStruct.cluster=newFile;
            strcpy(newFileStruct.name,dirp->d_name);
            newFileStruct.groupId=0;
            newFileStruct.userId=0;
            newFileStruct.size= FileSize(dir+"/"+string(dirp->d_name)) ;
            newFileStruct.type=FS_FILE;
            
            cout<< space <<string(dirp->d_name)<<" "<< newFileStruct.size <<" bytes"<<endl;
            
            /*scrive i dati contenuti nel file*/
            writeFile(newFile,dir+"/"+string(dirp->d_name));
            
            addNodeToDirectory(parent,newFileStruct);
            
            
        }
        else
        {
            if(string(dirp->d_name)!="." && string(dirp->d_name)!=".." && string(dirp->d_name)!=".svn")
            {
                /*se e' una directory*/
                cout<<space<<"["<<string(dirp->d_name)<<"]"<<endl;
                
                /*alloca un nuovo nodo di tipo directory*/
                unsigned int newDir;
                newDir=getFreeCluster();
                writeDirectoryHeader(newDir,0);
                
                /*inserisci la nuova directory come figlio della directory in cui si trova*/
                struct ramFs_node newDirStruct;
                newDirStruct.cluster=newDir;
                strcpy(newDirStruct.name,dirp->d_name);
                newDirStruct.groupId=0;
                newDirStruct.userId=0;
                newDirStruct.size=0;/*non usato perche' e' una directory*/
                newDirStruct.type=FS_DIRECTORY;
                
                addNodeToDirectory(parent,newDirStruct);
                
                getdir(dir+"/"+string(dirp->d_name),newDir);
            }
        }
    }
    closedir(dp);
    
    return 0;
}

int main()
{
    cout<<"dimensione cluster: ";
    cin>>clusterSize;
    cout<<"numero clusters: ";
    cin>>clusterNumber;
    
    imageSize=(clusterSize*clusterNumber)+(clusterNumber*4)+(4*2);
    
    cout<<"---"<<endl<<"capacita' file system: "<<clusterSize*clusterNumber<<"Bytes"<<endl<<"dimensione immagine: "<<imageSize<<"Bytes"<<endl<<"---"<<endl;
    
    fileSystem=(char*)malloc(imageSize);
    cout<<"buffer allocato"<<endl;
    
    cout<<"scrittura header"<<endl;
    unsigned int *header=(unsigned int*)fileSystem;
    header[0]=clusterNumber;
    header[1]=clusterSize;
    
    cout<<"inizializzazione FAT"<<endl;
    FAT=&header[2];
    for(int c=0;c<clusterNumber;c++)
        FAT[c]=RAMFS_FAT_EMPTY;
    
    cout<<"creazione root directory"<<endl;
    clusters=(char*)&FAT[clusterNumber];
    FAT[0]=RAMFS_FAT_END;
    rootDirectory=0;
    
    cout<<"inizio inserimento nodi:"<<endl; 
    string dir = string("root");

    getdir(dir,rootDirectory);
    
    
  
    /*codice di debug*/
    unsigned int *filesNellaRoot=(unsigned int*)clusters;
    cout<<"nodi nella root: "<<*filesNellaRoot<<endl;
    for(int c=0;c<*filesNellaRoot;c++)
    {
        struct ramFs_node *boh=(struct ramFs_node*)(clusters + 4 + (sizeof(struct ramFs_node)*c));
        cout<<"    cluster: "<<boh->cluster<<endl;
        cout<<"    group: "<<boh->groupId<<endl;
        cout<<"    name: "<<boh->name<<endl;
        cout<<"    size: "<<boh->size<<endl;
        switch(boh->type)
        {
            case FS_FILE:
                cout<<"    type: file"<<endl;
            break;
            case FS_DIRECTORY:
                cout<<"    type: directory"<<endl;
            break;
        }
        cout<<"    user: "<<boh->userId<<endl<<endl;
        
    }
    
    /*scrive il buffer su file*/
    ofstream out("out.img",ios::out|ios::binary);
    out.write(fileSystem,imageSize);
    out.close();
    
    
   

    return 0;
}
