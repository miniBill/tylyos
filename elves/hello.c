#include <kernel/syscall.h>

int main(){
  char test[]={'>','>','>','>','\n',0};
  while(1)
  {
      syswrite(test);
      syssleep(2000);
  }
 
  return 0;
}
