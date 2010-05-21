#include <kernel/syscall.h>

int main(){
  char test[]={'T','e','s','t','\n',0};
while(1)
  syswrite(test);
    return 0;
}
