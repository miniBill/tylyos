#include "syscalls.h"

int main()
{
    char test[100];
    while(1)
    {
      printf("# >:");
      int n=readLine(test,100);
      if(n>0)
      {  
          printf("readLine: ");
          printf(test);
          printf("\n");
      }
      syssleep(100);
    }
    
}
