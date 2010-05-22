#include <kernel/syscall.h>

int main(){
  char test[]={'>','>','>','>','\n',0};
  while(1)
      syswrite(test);
 
  return 0;
}
