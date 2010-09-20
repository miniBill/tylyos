#include "syscalls.h"

int main(int argc, char *argv[])
{
    char test[100];
    unsigned int task=0;
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
              
              task=forkExec(test,pp);
              if(task==0) 
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
              if(findchar(test,3,0)>=0)
                  kill(task);
              writeFile(pp[1],test,n);
          }
      }
      syssleep(100);
    }
    
}
