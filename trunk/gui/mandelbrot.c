#include "gui.h"

static const short max=1000;
static const int radius=100;

void draw_mandelbrot(void){
  unsigned char ix,iy;
  unsigned short count;
  float x0,y0,x,y,tx;
  for(iy=0;iy<VGA_height;iy++)
    for(ix=0;ix<VGA_width;ix++){
      x0=ix;y0=iy;
      x0=(x0/VGA_width)*4-2;
      y0=(y0/VGA_height)*4-2;
      x=y=0;
      while(x*x+y*y < radius*radius && count<max){
        tx = x*x - y*y + x0;
        y = 2*x*y + y0;
        x = tx;
        count++;
      }
      if(count==max)
        VGA_address[VGA_width*iy+ix]=0x1;
      else
        VGA_address[VGA_width*iy+ix]=0;
    }
}