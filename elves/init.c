#include "syscalls.h"

int main()
{
    char test[100];
    while(1)
    {
     int n=readFile(1,test,100);
      if(n>0)
          writeFile(2,test,n);
      syssleep(100);
    }
    return 0;
}
