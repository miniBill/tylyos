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

int main(){
  char test[]={'T','e','s','t',' ','\n',0};
  int file=0;
while(1)
{
  file= openFile("/hello",'r');
  test[4]='a'+file;
  syswrite(test);

  closeFile(file);
  syssleep(5000);
}
    return 0;
}
