#include <kernel/syscall.h>

int main(){
  char test[]={'T','e','s','t',' ','\n',0};
  unsigned int count=0;
while(1)
{
  test[4]='a'+count++;
  syswrite(test);
  if(count==26)
    count=0;
}
    return 0;
}
