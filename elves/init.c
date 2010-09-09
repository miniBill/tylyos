#include "syscalls.h"

int main()
{
    char test[100];
    while(1)
    {
      if(readFile(1,test,100)>0)
          syswrite(test);
      syssleep(100);
    }
    return 0;
}
