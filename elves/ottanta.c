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

int main(){
  char test[100];
  int file=0;
while(1)
{
  file= openFile("/config/test.txt",'r');

  readFile(file,test,100);
  syswrite(test);

  closeFile(file);
  syssleep(5000);
}
    return 0;
}
