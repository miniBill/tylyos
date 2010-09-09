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

int main(){
  char test[100];
  int file=0;
  int p[2];
  pipe(p);
while(1)
{
/*
  file= openFile("/config/test.txt",'r');

  readFile(file,test,100);
  syswrite(test);

  closeFile(file);
  syssleep(5000);
*/
  
  writeFile(p[0],"pipe",4);
  readFile(p[1],test,100);
  syswrite(test);
  syssleep(5000);
}
    return 0;
}
