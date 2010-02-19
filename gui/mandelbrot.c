#include "gui.h"
#include <config.txt>
#include <kernel/kernel.h>
#include <drivers/keyboard/keyboard.h>
#include <drivers/screen/vga.h>

static char grid[2][200][320];
static int round=0;

#define by 100
#define bx 160

static void init(void){
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

static char AG(int a,int b){
  if(a<0 || b<0 || a==200 || b==320)
    return 0;
  return grid[1-round][a][b];
}

static int G(int a,int b){
  if(a<0 || b<0 || a==200 || b==320)
    return 0;
  return grid[round][a][b];
}

static char nei[MANDELBROT_COLORS]={0};

#define S(a) if(a>0)nei[a-1]++

//maps neigh. color to new color
static inline char gnu(void){
  #if MANDELBROT_COLORS == 4
  for(int i=0;i<MANDELBROT_COLORS;i++)
    if(nei[i]==2 || nei[i]==3)
      return i+1;
  for(int i=0;i<MANDELBROT_COLORS;i++)
    if(nei[i]==0)
      return i+1;
  #else
    #if MANDELBROT_COLORS == 2
  return nei[0]>nei[1]?1:2;
    #else
  return 1;
    #endif
  #endif
  return 0;
}

static void step(void){
  for(int y=0;y<200;y++)
    for(int x=0;x<320;x++){
      for(int i=0;i<MANDELBROT_COLORS;i++)
        nei[i]=0;
      S(G(y-1,x-1));
      S(G(y-1,x));
      S(G(y-1,x+1));
      S(G(y,x-1));
      S(G(y,x+1));
      S(G(y+1,x-1));
      S(G(y+1,x));
      S(G(y+1,x+1));
      char n=G(y,x);
      char neigh=0;
      for(int i=0;i<MANDELBROT_COLORS;i++)
        neigh+=nei[i];
      if(n){
        if(neigh!=2 && neigh!=3)
          n=0;
      }
      else if(neigh==3)
        n=gnu();
      grid[1-round][y][x]=n;
    }
    round=1-round;
}

//maps memory color to vga color
static inline unsigned char map(char color){
  switch(color){
    case 1:
      return 3;
    case 2:
      return 224;
    case 3:
      return 28;
    case 4:
      return 252;
    default:
      return 1;
  }
}

static void print(void){
  for(int y=0;y<200;y++)
    for(int x=0;x<320;x++)
      if(G(y,x)!=AG(y,x))//only if different!!!
        VGA_address[VGA_width*y+x]=map(G(y,x));
}

static int t;

static void randomize(void){
  int a=0;
  t+=2;
  char osc=0;
  for(int y=0;y<200;y++)
    for(int x=0;x<320;x++,a++){
      if(a==t)
        a=0;
      grid[round][y][x]=(x+y+a)%2;
      if(grid[round][y][x]){
        grid[round][y][x]+=osc;
        grid[round][y][x]%=MANDELBROT_COLORS+1;
        osc++;
        if(osc==MANDELBROT_COLORS)
          osc=0;
      }
    }
}

void gui_life(void){
  for(int y=0;y<200;y++)
    for(int x=0;x<320;x++)
      grid[0][y][x]=grid[1][y][x]=0;
  init();
  t=-1;
  int p=0;
  char g;
  while(1){
    print();
    step();
    do{
      g=getch();
      if(g=='p')
        p=1-p;
    }while(p);
    switch(g){
      case 'R':
        t=-1;
        break;
      case 'e':
        t-=2;
        randomize();
        break;
      case 'w':
        t-=4;
        randomize();
        break;
      case 'r':
        randomize();
        break;
    }
  }
}

static const int radius=2;

static char convert(unsigned char count){
  return count/4*32;
}

void draw_mandelbrot(void){
  unsigned short ix,iy;
  unsigned short count;
  float x0,y0,x,y,tx;
  unsigned short min=255,max=0;
  for(iy=0;iy<VGA_height;iy++)
    for(ix=0;ix<VGA_width;ix++){
      x0=ix;y0=iy;
      x0=(x0/VGA_width)*2.8f-2.2f;
      y0=(y0/VGA_height)*2.4f-1.2f;
      x=y=count=0;
      while(((x*x+y*y) < radius*radius) && count<255){
        tx = x*x - y*y + x0;
        y = 2*x*y + y0;
        x = tx;
        count++;
      }
      if(count==255)
        VGA_address[VGA_width*iy+ix]=0x0;
      else{
        if(count<4)
          VGA_address[VGA_width*iy+ix]=0x13;
        else
          VGA_address[VGA_width*iy+ix]=convert(count);
        if(count<min)
          min=count;
        if(count>max)
          max=count;
      }
    }
  
}