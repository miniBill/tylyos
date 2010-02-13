#include "gui.h"

static const short max=100;
static const int radius=2;

void draw_mandelbrot(void){
  unsigned short ix,iy;
  unsigned short count;
  float x0,y0,x,y,tx;
  for(iy=0;iy<VGA_height;iy++)
    for(ix=0;ix<VGA_width;ix++){
      x0=ix;y0=iy;
      x0=(x0/VGA_width)*2.8f-2.2f;
      y0=(y0/VGA_height)*2.4f-1.2f;
      x=y=count=0;
      while(((x*x+y*y) < radius*radius) && count<max){
        tx = x*x - y*y + x0;
        y = 2*x*y + y0;
        x = tx;
        count++;
      }
      if(count==max)
        VGA_address[VGA_width*iy+ix]=0x0;
      else
        VGA_address[VGA_width*iy+ix]=(char)count;
    }
}