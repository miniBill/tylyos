#include "syscalls.h"

int main()
{
while(1);
    char test[100];
    while(1)
    {
      int n=get(test,100);
      if(n>0)
      {  
          printf(test);
      }
      syssleep(100);
    }
    
}
