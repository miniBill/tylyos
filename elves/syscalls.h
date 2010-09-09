#include <kernel/syscall.h>

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


void pipe(int descriptors[2])
{
    syscallone(200,descriptors);
}
