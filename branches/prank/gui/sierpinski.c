#include <drivers/screen/vga.h>

void gui_sierpinski(int n){
  n*=2;
  int a[2][n];
  for(int q=0;q<n;q++)
    a[0][q]=a[1][q]=0;
  a[0][n/2]=1;
  for(int q=1;q<n;q++)
    VGA_address[q-1]=a[0][q];
  for(int i=1;i<n/2-1;i++){
    int r=i%2;
    for(int j=1;j<n-1;j++){
      a[r][j]=a[1-r][j-1]^a[1-r][j+1];
      VGA_address[VGA_width*i+j-1]=a[r][j];
    }
  }
}

void gui_grid(int m){
  int a[VGA_height][VGA_width];
  for(unsigned int i=0;i<VGA_height;i++)
    a[i][0]=0;
  for(unsigned int i=0;i<VGA_width;i++)
    a[0][i]=0;
  a[1][1]=1;
  for(unsigned int y=1;y<VGA_height;y++){
    for(unsigned int x=1;x<VGA_width;x++){
      if(x+y!=2)
        a[y][x]=(a[y-1][x-1]+a[y-1][x]+a[y][x-1])%m;
      VGA_address[VGA_width*y+x]=a[y][x];
    }
  }
}