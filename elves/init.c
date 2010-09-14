#include "syscalls.h"

int main()
{
    char test[100];
    File pipein[2];/*read 0,write 1*/
    File pipeout[2];/*read 0,write 1*/
    pipe(pipein);
    pipe(pipeout);
    unsigned int r=fork();
    if(r==0)
    {  
        mov2(pipein[0],STANDARD_INPUT); 
        mov2(pipeout[1],STANDARD_OUTPUT);
        exec("/shell");
    }
    while(1)
    {
      int n=get(test,100);
      if(n>0)
      {
          writeFile(pipein[1],test,strlen(test));
      }
      n=readFile(pipeout[0],test,100);
      if(n>0)
      {
          printf(test);
      }
      //syssleep(100);
    }
}
