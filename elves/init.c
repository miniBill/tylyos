#include "syscalls.h"

int main()
{
    char test[101];
    File pp[2];
    forkExec("/shell",pp);
    while(1)
    {
      int n=get(test,100);
      if(n>0)
      {
          writeFile(pp[1],test,n);
      }
      n=readFile(pp[0],test,100);
      if(n>0)
      {
          test[n]=0;
          printf(test);
      }
      if(n==-1)
      {
          closeFile(pp[1]);
          printf("respawning shell\n");
          forkExec("/shell",pp);
      }
      //syssleep(100);
    }
}
