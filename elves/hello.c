#include <kernel/syscall.h>
#include "syscalls.h"

int main(int argc,char **argv){
  char test[]={'>','>','>','>','\n',0};
  while(1)
  {
      syswrite(test);
      syssleep(2000);
  }
 
  return 0;
}
