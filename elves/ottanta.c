#include "syscalls.h"

int main(){
while(1)
{
  syswrite("ottanta\n"); 
  syssleep(2000);
}
    return 0;
}
