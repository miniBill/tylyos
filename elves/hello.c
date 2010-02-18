#include <kernel/syscall.h>

int main(){
  char test[]={'T','e','s','t','\n',0};
  syswrite(test);
  return 0;
}
