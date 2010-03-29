#pragma once

void VGA_init(int width, int height, int colors);

unsigned int VGA_width;
unsigned int VGA_height;
unsigned int VGA_bpp;

unsigned char *VGA_address;
