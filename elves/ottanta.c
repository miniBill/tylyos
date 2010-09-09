#include "syscalls.h"

int main(){
  char test[100];
  int file=0;
  int p[2];
  pipe(p);
while(1)
{
/*
  file= openFile("/config/test.txt",'r');

  readFile(file,test,100);
  syswrite(test);

  closeFile(file);
  syssleep(5000);
*/
  
  writeFile(p[0],"pipe",4);
  readFile(p[1],test,100);
  syswrite(test);
  syssleep(5000);
}
    return 0;
}
