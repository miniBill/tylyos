#include "drivers/screen/screen.h"
#include "kernel/kernel.h"
#include "drivers/keyboard/keyboard.h"

static int grid[2][ROWS-1][COLUMNS];
static int round=0;

#define by 14
#define bx 40

void init(void){
  grid[0][by][bx]=1;
  grid[0][by][bx+1]=1;
  grid[0][by][bx+2]=1;
  grid[0][by][bx+4]=1;
  grid[0][by+1][bx]=1;
  grid[0][by+2][bx+3]=1;
  grid[0][by+2][bx+4]=1;
  grid[0][by+3][bx+1]=1;
  grid[0][by+3][bx+2]=1;
  grid[0][by+3][bx+4]=1;
  grid[0][by+4][bx]=1;
  grid[0][by+4][bx+2]=1;
  grid[0][by+4][bx+4]=1;
}

int G(int a,int b){
  if(a<0 || b<0 || a==ROWS-1 || b==COLUMNS)
    return 0;
  return grid[round][a][b];
}

void step(void){
  for(int y=0;y<ROWS-1;y++)
    for(int x=0;x<COLUMNS;x++){
      int neigh=G(y-1,x-1)+G(y-1,x)+G(y-1,x+1)+
      G(y,x-1)+G(y,x+1)+
      G(y+1,x-1)+G(y+1,x)+G(y+1,x+1);
      int n=G(y,x);
      if(n){
	if(neigh!=2 && neigh!=3)
	  n=0;
      }
      else if(neigh==3)
	  n=1;
      grid[1-round][y][x]=n;
    }
  round=1-round;
}

static void print(void){
  set_cursor(0,0);
  for(int y=0;y<ROWS-1;y++)
    for(int x=0;x<COLUMNS;x++)
      put_xy(G(y,x),0,x,y);
}

static void randomize(void){
  int a=0;
  static int t=1;
  t+=2;
  for(int y=0;y<ROWS-1;y++)
    for(int x=0;x<COLUMNS;x++,a++)
      grid[round][y][x]=(x+y+a%t)%2;
}

void life(void){
  for(int y=0;y<ROWS-1;y++)
    for(int x=0;x<COLUMNS;x++){
      grid[0][y][x]=grid[1][y][x]=0;
      put_physical_color_xy(White|Back_Black,x,y);
    }
  init();
  int p=0;
  while(1){
    print();
    step();
    char g;
    do{
      sleep(100);
      g=getch();
      if(g=='p')
	p=1-p;
    }while(p);
    if(g=='r')
      randomize();
  }
}