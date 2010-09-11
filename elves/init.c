#include "syscalls.h"

int main()
{
    char test[100];
    while(1)
    {
     int n=get(test,100);
      if(n>0)
      {
          printf(test);
          if(test[0]=='Q')//TEST
              exit(0);
      }
      syssleep(100);
    }
}
