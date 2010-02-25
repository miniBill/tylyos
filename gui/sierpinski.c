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