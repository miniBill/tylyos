#include "syscalls.h"

void forkExec(char *path,File _p[2])
{
    File pipein[2];/*read 0,write 1*/
    File pipeout[2];/*read 0,write 1*/
    pipe(pipein);
    pipe(pipeout);
    unsigned int r=fork();
    if(r==0)
    {  
        mov2(pipein[0],STANDARD_INPUT);
        mov2(pipeout[1],STANDARD_OUTPUT);
        exec(path);
    }
    _p[0]=pipeout[0];
    _p[1]=pipein[1];
}

int main()
{
    char test[100];
    char task=0;
    File pp[2];
    int n;
    while(1)
    {
      if(!task)
      {
          printf("# >:");
          n=readLine(test,100);
          if(n>0)
          {  
              printf("readLine: %s\n",test);
              forkExec(test,pp);
              task=1;
          }
      }
      if(task)
      {
          n=readFile(pp[0],test,100);
          if(n>0)
          {
              printf(test);
          }
          n=get(test,100);
          if(n>0)
          {
              writeFile(pp[1],test,strlen(test));
          }
      }
      syssleep(100);
    }
    
}
