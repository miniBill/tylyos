#include "syscalls.h"

int main(int argc, char *argv[])
{
  for(int c=0;c<argc;c++)
      printf("%s\n",argv[c]);

  syssleep(200);
  return 0;
}
