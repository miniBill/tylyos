#include "syscalls.h"

int main(int argc, char *argv[])
{
    if(argc>0)
        printf("(%d) %s\n",argc,argv[0]);
    else
        printf("(0)\n");
while(1);

  return 0;
}
