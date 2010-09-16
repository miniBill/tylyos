#include "syscalls.h"

int main(int argc, char *argv[])
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
              //printf("readLine: %s\n",test);
              if(strcmp(test,"exit")==0)
              {
                  syssleep(100);
                  return 0;
              }
              
              if(forkExec(test,pp))
                  task=1;
              else
                  printf("FILE NOT FOUND\n");
          }
      }
      if(task)
      {
          n=readFile(pp[0],test,100);
          if(n>0)
          {
              test[n]=0;
              printf(test);
          }
          if(n==-1)
          {
              task=0;
              closeFile(pp[1]);
          }
          n=get(test,100);
          if(n>0)
          {
              writeFile(pp[1],test,n);
          }
      }
      syssleep(100);
    }
    
}
