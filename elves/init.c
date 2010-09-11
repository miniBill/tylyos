#include "syscalls.h"

int main()
{
    char test[100];
    while(1)
    {
     int n=get(test,100);
      if(n>0)
      {
          printf(test);
          if(test[0]=='Q')//TEST
              exit(0);
          if(test[0]=='F')//TEST
          {
              int r=fork();
              if(r==0)
              {
                  printf("io sono il vecchio task e mi killo\n");
                  exit(0);
              }
              else
              {
                  printf("io sono il fork\n");
              }
          }
      }
      syssleep(100);
    }
}
