#include <kernel/syscall.h>

typedef unsigned int File;


void exit(int status)
{
    syscallone(1,status);
}

void exec(char *path)
{
    syscallone(3,path);
}

File mov2(File file,File newfile)
{
    File ret;
    syscalltwo(4,file,newfile);
    asm( "mov %%eax, %0;"
          :"=r"(ret)
       );
    return ret;
}

int fork()
{
    int ret;
    syscallone(2,0);
    asm( "mov %%eax, %0;"
          :"=r"(ret)
       );
    return ret;
}


int openFile(char *path,char mode)
{
    int ret;
    syscalltwo(252,path,mode);
    asm( "mov %%eax, %0;"
          :"=r"(ret)
       );
    return ret;
}

void closeFile(int file)
{
    syscallone(253,file);
}

unsigned int readFile(int file,char *buffer,unsigned int byteCount)
{
    int ret;
    syscallthree(254,file,buffer,byteCount);
    asm( "mov %%eax, %0;"
          :"=r"(ret)
       );
    return ret;
}

unsigned int writeFile(int file,char *buffer,unsigned int byteCount)
{   
    int ret;
    syscallthree(201,file,buffer,byteCount);
    asm( "mov %%eax, %0;"
          :"=r"(ret)
       );
    return ret;
}

unsigned int strlen(char *str)
{
    unsigned int ret=0;
    while(str[ret]!=0)
        ret++;
    return ret;
}

void printf(char *buffer)
{
    writeFile(2,buffer,strlen(buffer));
}

unsigned int get(char *buffer,unsigned int bufferSize)
{
    return readFile(1,buffer,bufferSize);
}


void pipe(File descriptors[2])
{
    syscallone(200,descriptors);
}

unsigned int readLine(char *buffer,unsigned int bufferSize)
{
    unsigned int i=0;
    char c[2];
    buffer[0]=0;
    while(1)
    {   
        if(get(c,1))
        {  
            switch(c[0])
            {
            case '\n':
                printf("\n");
                return i;
                break;
            case '\b':
                if(i>0)
                {
                    printf("\b");
                    i-=1;
                    buffer[i]=0;
                }
                break;
            default:
                if(i<bufferSize-2)
                {  
                    printf(c);
                    buffer[i]=c[0];
                    buffer[i+1]=0;
                    i+=1;
                }
                break;
            }

        }
    }
}
