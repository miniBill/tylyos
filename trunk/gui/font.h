#ifndef FONT_H
#define FONT_H

unsigned char g_8x8_font[2048]={
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x7E, 0x81, 0xA5, 0x81, 0xBD, 0x99, 0x81, 0x7E,
    0x7E, 0xFF, 0xDB, 0xFF, 0xC3, 0xE7, 0xFF, 0x7E,
    0x6C, 0xFE, 0xFE, 0xFE, 0x7C, 0x38, 0x10, 0x00,
    0x10, 0x38, 0x7C, 0xFE, 0x7C, 0x38, 0x10, 0x00,
    0x38, 0x7C, 0x38, 0xFE, 0xFE, 0x92, 0x10, 0x7C,
    0x00, 0x10, 0x38, 0x7C, 0xFE, 0x7C, 0x38, 0x7C,
    0x00, 0x00, 0x18, 0x3C, 0x3C, 0x18, 0x00, 0x00,
    0xFF, 0xFF, 0xE7, 0xC3, 0xC3, 0xE7, 0xFF, 0xFF,
    0x00, 0x3C, 0x66, 0x42, 0x42, 0x66, 0x3C, 0x00,
    0xFF, 0xC3, 0x99, 0xBD, 0xBD, 0x99, 0xC3, 0xFF,
    0x0F, 0x07, 0x0F, 0x7D, 0xCC, 0xCC, 0xCC, 0x78,
    0x3C, 0x66, 0x66, 0x66, 0x3C, 0x18, 0x7E, 0x18,
    0x3F, 0x33, 0x3F, 0x30, 0x30, 0x70, 0xF0, 0xE0,
    0x7F, 0x63, 0x7F, 0x63, 0x63, 0x67, 0xE6, 0xC0,
    0x99, 0x5A, 0x3C, 0xE7, 0xE7, 0x3C, 0x5A, 0x99,
    0x80, 0xE0, 0xF8, 0xFE, 0xF8, 0xE0, 0x80, 0x00,
    0x02, 0x0E, 0x3E, 0xFE, 0x3E, 0x0E, 0x02, 0x00,
    0x18, 0x3C, 0x7E, 0x18, 0x18, 0x7E, 0x3C, 0x18, 
    0x66, 0x66, 0x66, 0x66, 0x66, 0x00, 0x66, 0x00, 
    0x7F, 0xDB, 0xDB, 0x7B, 0x1B, 0x1B, 0x1B, 0x00, 
    0x3E, 0x63, 0x38, 0x6C, 0x6C, 0x38, 0x86, 0xFC, 
    0x00, 0x00, 0x00, 0x00, 0x7E, 0x7E, 0x7E, 0x00, 
    0x18, 0x3C, 0x7E, 0x18, 0x7E, 0x3C, 0x18, 0xFF,
    0x18, 0x3C, 0x7E, 0x18, 0x18, 0x18, 0x18, 0x00,
    0x18, 0x18, 0x18, 0x18, 0x7E, 0x3C, 0x18, 0x00,
    0x00, 0x18, 0x0C, 0xFE, 0x0C, 0x18, 0x00, 0x00, 
    0x00, 0x30, 0x60, 0xFE, 0x60, 0x30, 0x00, 0x00, 
    0x00, 0x00, 0xC0, 0xC0, 0xC0, 0xFE, 0x00, 0x00, 
    0x00, 0x24, 0x66, 0xFF, 0x66, 0x24, 0x00, 0x00, 
    0x00, 0x18, 0x3C, 0x7E, 0xFF, 0xFF, 0x00, 0x00, 
    0x00, 0xFF, 0xFF, 0x7E, 0x3C, 0x18, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x18, 0x3C, 0x3C, 0x18, 0x18, 0x00, 0x18, 0x00,
    0x6C, 0x6C, 0x6C, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x6C, 0x6C, 0xFE, 0x6C, 0xFE, 0x6C, 0x6C, 0x00, 
    0x18, 0x7E, 0xC0, 0x7C, 0x06, 0xFC, 0x18, 0x00, 
    0x00, 0xC6, 0xCC, 0x18, 0x30, 0x66, 0xC6, 0x00,
    0x38, 0x6C, 0x38, 0x76, 0xDC, 0xCC, 0x76, 0x00,
    0x30, 0x30, 0x60, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x18, 0x30, 0x60, 0x60, 0x60, 0x30, 0x18, 0x00, 
    0x60, 0x30, 0x18, 0x18, 0x18, 0x30, 0x60, 0x00,
    0x00, 0x66, 0x3C, 0xFF, 0x3C, 0x66, 0x00, 0x00, 
    0x00, 0x18, 0x18, 0x7E, 0x18, 0x18, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x18, 0x30,
    0x00, 0x00, 0x00, 0x7E, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x18, 0x00,
    0x06, 0x0C, 0x18, 0x30, 0x60, 0xC0, 0x80, 0x00,
    0x7C, 0xCE, 0xDE, 0xF6, 0xE6, 0xC6, 0x7C, 0x00, 
    0x30, 0x70, 0x30, 0x30, 0x30, 0x30, 0xFC, 0x00, 
    0x78, 0xCC, 0x0C, 0x38, 0x60, 0xCC, 0xFC, 0x00, 
    0x78, 0xCC, 0x0C, 0x38, 0x0C, 0xCC, 0x78, 0x00, 
    0x1C, 0x3C, 0x6C, 0xCC, 0xFE, 0x0C, 0x1E, 0x00, 
    0xFC, 0xC0, 0xF8, 0x0C, 0x0C, 0xCC, 0x78, 0x00, 
    0x38, 0x60, 0xC0, 0xF8, 0xCC, 0xCC, 0x78, 0x00,
    0xFC, 0xCC, 0x0C, 0x18, 0x30, 0x30, 0x30, 0x00,
    0x78, 0xCC, 0xCC, 0x78, 0xCC, 0xCC, 0x78, 0x00, 
    0x78, 0xCC, 0xCC, 0x7C, 0x0C, 0x18, 0x70, 0x00, 
    0x00, 0x18, 0x18, 0x00, 0x00, 0x18, 0x18, 0x00, 
    0x00, 0x18, 0x18, 0x00, 0x00, 0x18, 0x18, 0x30, 
    0x18, 0x30, 0x60, 0xC0, 0x60, 0x30, 0x18, 0x00, 
    0x00, 0x00, 0x7E, 0x00, 0x7E, 0x00, 0x00, 0x00, 
    0x60, 0x30, 0x18, 0x0C, 0x18, 0x30, 0x60, 0x00,
    0x3C, 0x66, 0x0C, 0x18, 0x18, 0x00, 0x18, 0x00, 
    0x7C, 0xC6, 0xDE, 0xDE, 0xDC, 0xC0, 0x7C, 0x00, 
    0x30, 0x78, 0xCC, 0xCC, 0xFC, 0xCC, 0xCC, 0x00, 
    0xFC, 0x66, 0x66, 0x7C, 0x66, 0x66, 0xFC, 0x00, 
    0x3C, 0x66, 0xC0, 0xC0, 0xC0, 0x66, 0x3C, 0x00, 
    0xF8, 0x6C, 0x66, 0x66, 0x66, 0x6C, 0xF8, 0x00, 
    0xFE, 0x62, 0x68, 0x78, 0x68, 0x62, 0xFE, 0x00, 
    0xFE, 0x62, 0x68, 0x78, 0x68, 0x60, 0xF0, 0x00,
    0x3C, 0x66, 0xC0, 0xC0, 0xCE, 0x66, 0x3A, 0x00, 
    0xCC, 0xCC, 0xCC, 0xFC, 0xCC, 0xCC, 0xCC, 0x00, 
    0x78, 0x30, 0x30, 0x30, 0x30, 0x30, 0x78, 0x00, 
    0x1E, 0x0C, 0x0C, 0x0C, 0xCC, 0xCC, 0x78, 0x00, 
    0xE6, 0x66, 0x6C, 0x78, 0x6C, 0x66, 0xE6, 0x00, 
    0xF0, 0x60, 0x60, 0x60, 0x62, 0x66, 0xFE, 0x00, 
    0xC6, 0xEE, 0xFE, 0xFE, 0xD6, 0xC6, 0xC6, 0x00,
    0xC6, 0xE6, 0xF6, 0xDE, 0xCE, 0xC6, 0xC6, 0x00, 
    0x38, 0x6C, 0xC6, 0xC6, 0xC6, 0x6C, 0x38, 0x00, 
    0xFC, 0x66, 0x66, 0x7C, 0x60, 0x60, 0xF0, 0x00, 
    0x7C, 0xC6, 0xC6, 0xC6, 0xD6, 0x7C, 0x0E, 0x00, 
    0xFC, 0x66, 0x66, 0x7C, 0x6C, 0x66, 0xE6, 0x00,
    0x7C, 0xC6, 0xE0, 0x78, 0x0E, 0xC6, 0x7C, 0x00, 
    0xFC, 0xB4, 0x30, 0x30, 0x30, 0x30, 0x78, 0x00,
    0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xFC, 0x00, 
    0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0x78, 0x30, 0x00,
    0xC6, 0xC6, 0xC6, 0xC6, 0xD6, 0xFE, 0x6C, 0x00, 
    0xC6, 0xC6, 0x6C, 0x38, 0x6C, 0xC6, 0xC6, 0x00, 
    0xCC, 0xCC, 0xCC, 0x78, 0x30, 0x30, 0x78, 0x00,
    0xFE, 0xC6, 0x8C, 0x18, 0x32, 0x66, 0xFE, 0x00,
    0x78, 0x60, 0x60, 0x60, 0x60, 0x60, 0x78, 0x00,
    0xC0, 0x60, 0x30, 0x18, 0x0C, 0x06, 0x02, 0x00,
    0x78, 0x18, 0x18, 0x18, 0x18, 0x18, 0x78, 0x00, 
    0x10, 0x38, 0x6C, 0xC6, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 
    0x30, 0x30, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x78, 0x0C, 0x7C, 0xCC, 0x76, 0x00, 
    0xE0, 0x60, 0x60, 0x7C, 0x66, 0x66, 0xDC, 0x00, 
    0x00, 0x00, 0x78, 0xCC, 0xC0, 0xCC, 0x78, 0x00,
    0x1C, 0x0C, 0x0C, 0x7C, 0xCC, 0xCC, 0x76, 0x00,
    0x00, 0x00, 0x78, 0xCC, 0xFC, 0xC0, 0x78, 0x00, 
    0x38, 0x6C, 0x64, 0xF0, 0x60, 0x60, 0xF0, 0x00, 
    0x00, 0x00, 0x76, 0xCC, 0xCC, 0x7C, 0x0C, 0xF8, 
    0xE0, 0x60, 0x6C, 0x76, 0x66, 0x66, 0xE6, 0x00, 
    0x30, 0x00, 0x70, 0x30, 0x30, 0x30, 0x78, 0x00, 
    0x0C, 0x00, 0x1C, 0x0C, 0x0C, 0xCC, 0xCC, 0x78, 
    0xE0, 0x60, 0x66, 0x6C, 0x78, 0x6C, 0xE6, 0x00,
    0x70, 0x30, 0x30, 0x30, 0x30, 0x30, 0x78, 0x00, 
    0x00, 0x00, 0xCC, 0xFE, 0xFE, 0xD6, 0xD6, 0x00, 
    0x00, 0x00, 0xB8, 0xCC, 0xCC, 0xCC, 0xCC, 0x00, 
    0x00, 0x00, 0x78, 0xCC, 0xCC, 0xCC, 0x78, 0x00, 
    0x00, 0x00, 0xDC, 0x66, 0x66, 0x7C, 0x60, 0xF0, 
    0x00, 0x00, 0x76, 0xCC, 0xCC, 0x7C, 0x0C, 0x1E, 
    0x00, 0x00, 0xDC, 0x76, 0x62, 0x60, 0xF0, 0x00, 
    0x00, 0x00, 0x7C, 0xC0, 0x70, 0x1C, 0xF8, 0x00,
    0x10, 0x30, 0xFC, 0x30, 0x30, 0x34, 0x18, 0x00, 
    0x00, 0x00, 0xCC, 0xCC, 0xCC, 0xCC, 0x76, 0x00, 
    0x00, 0x00, 0xCC, 0xCC, 0xCC, 0x78, 0x30, 0x00, 
    0x00, 0x00, 0xC6, 0xC6, 0xD6, 0xFE, 0x6C, 0x00, 
    0x00, 0x00, 0xC6, 0x6C, 0x38, 0x6C, 0xC6, 0x00, 
    0x00, 0x00, 0xCC, 0xCC, 0xCC, 0x7C, 0x0C, 0xF8, 
    0x00, 0x00, 0xFC, 0x98, 0x30, 0x64, 0xFC, 0x00,
    0x1C, 0x30, 0x30, 0xE0, 0x30, 0x30, 0x1C, 0x00, 
    0x18, 0x18, 0x18, 0x00, 0x18, 0x18, 0x18, 0x00, 
    0xE0, 0x30, 0x30, 0x1C, 0x30, 0x30, 0xE0, 0x00, 
    0x76, 0xDC, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x10, 0x38, 0x6C, 0xC6, 0xC6, 0xFE, 0x00,
    0x7C, 0xC6, 0xC0, 0xC6, 0x7C, 0x0C, 0x06, 0x7C, 
    0x00, 0xCC, 0x00, 0xCC, 0xCC, 0xCC, 0x76, 0x00,
    0x1C, 0x00, 0x78, 0xCC, 0xFC, 0xC0, 0x78, 0x00, 
    0x7E, 0x81, 0x3C, 0x06, 0x3E, 0x66, 0x3B, 0x00,
    0xCC, 0x00, 0x78, 0x0C, 0x7C, 0xCC, 0x76, 0x00, 
    0xE0, 0x00, 0x78, 0x0C, 0x7C, 0xCC, 0x76, 0x00, 
    0x30, 0x30, 0x78, 0x0C, 0x7C, 0xCC, 0x76, 0x00,
    0x00, 0x00, 0x7C, 0xC6, 0xC0, 0x78, 0x0C, 0x38,
    0x7E, 0x81, 0x3C, 0x66, 0x7E, 0x60, 0x3C, 0x00,
    0xCC, 0x00, 0x78, 0xCC, 0xFC, 0xC0, 0x78, 0x00,
    0xE0, 0x00, 0x78, 0xCC, 0xFC, 0xC0, 0x78, 0x00, 
    0xCC, 0x00, 0x70, 0x30, 0x30, 0x30, 0x78, 0x00, 
    0x7C, 0x82, 0x38, 0x18, 0x18, 0x18, 0x3C, 0x00, 
    0xE0, 0x00, 0x70, 0x30, 0x30, 0x30, 0x78, 0x00, 
    0xC6, 0x10, 0x7C, 0xC6, 0xFE, 0xC6, 0xC6, 0x00, 
    0x30, 0x30, 0x00, 0x78, 0xCC, 0xFC, 0xCC, 0x00, 
    0x1C, 0x00, 0xFC, 0x60, 0x78, 0x60, 0xFC, 0x00,
    0x00, 0x00, 0x7F, 0x0C, 0x7F, 0xCC, 0x7F, 0x00,
    0x3E, 0x6C, 0xCC, 0xFE, 0xCC, 0xCC, 0xCE, 0x00, 
    0x78, 0x84, 0x00, 0x78, 0xCC, 0xCC, 0x78, 0x00, 
    0x00, 0xCC, 0x00, 0x78, 0xCC, 0xCC, 0x78, 0x00, 
    0x00, 0xE0, 0x00, 0x78, 0xCC, 0xCC, 0x78, 0x00, 
    0x78, 0x84, 0x00, 0xCC, 0xCC, 0xCC, 0x76, 0x00, 
    0x00, 0xE0, 0x00, 0xCC, 0xCC, 0xCC, 0x76, 0x00, 
    0x00, 0xCC, 0x00, 0xCC, 0xCC, 0x7C, 0x0C, 0xF8,
    0xC3, 0x18, 0x3C, 0x66, 0x66, 0x3C, 0x18, 0x00, 
    0xCC, 0x00, 0xCC, 0xCC, 0xCC, 0xCC, 0x78, 0x00, 
    0x18, 0x18, 0x7E, 0xC0, 0xC0, 0x7E, 0x18, 0x18, 
    0x38, 0x6C, 0x64, 0xF0, 0x60, 0xE6, 0xFC, 0x00, 
    0xCC, 0xCC, 0x78, 0x30, 0xFC, 0x30, 0xFC, 0x30, 
    0xF8, 0xCC, 0xCC, 0xFA, 0xC6, 0xCF, 0xC6, 0xC3, 
    0x0E, 0x1B, 0x18, 0x3C, 0x18, 0x18, 0xD8, 0x70, 
    0x1C, 0x00, 0x78, 0x0C, 0x7C, 0xCC, 0x76, 0x00,
    0x38, 0x00, 0x70, 0x30, 0x30, 0x30, 0x78, 0x00, 
    0x00, 0x1C, 0x00, 0x78, 0xCC, 0xCC, 0x78, 0x00, 
    0x00, 0x1C, 0x00, 0xCC, 0xCC, 0xCC, 0x76, 0x00, 
    0x00, 0xF8, 0x00, 0xB8, 0xCC, 0xCC, 0xCC, 0x00, 
    0xFC, 0x00, 0xCC, 0xEC, 0xFC, 0xDC, 0xCC, 0x00, 
    0x3C, 0x6C, 0x6C, 0x3E, 0x00, 0x7E, 0x00, 0x00, 
    0x38, 0x6C, 0x6C, 0x38, 0x00, 0x7C, 0x00, 0x00,
    0x18, 0x00, 0x18, 0x18, 0x30, 0x66, 0x3C, 0x00, 
    0x00, 0x00, 0x00, 0xFC, 0xC0, 0xC0, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0xFC, 0x0C, 0x0C, 0x00, 0x00, 
    0xC6, 0xCC, 0xD8, 0x36, 0x6B, 0xC2, 0x84, 0x0F, 
    0xC3, 0xC6, 0xCC, 0xDB, 0x37, 0x6D, 0xCF, 0x03,
    0x18, 0x00, 0x18, 0x18, 0x3C, 0x3C, 0x18, 0x00, 
    0x00, 0x33, 0x66, 0xCC, 0x66, 0x33, 0x00, 0x00,
    0x00, 0xCC, 0x66, 0x33, 0x66, 0xCC, 0x00, 0x00, 
    0x22, 0x88, 0x22, 0x88, 0x22, 0x88, 0x22, 0x88,
    0x55, 0xAA, 0x55, 0xAA, 0x55, 0xAA, 0x55, 0xAA, 
    0xDB, 0xF6, 0xDB, 0x6F, 0xDB, 0x7E, 0xD7, 0xED, 
    0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18,
    0x18, 0x18, 0x18, 0x18, 0xF8, 0x18, 0x18, 0x18,
    0x18, 0x18, 0xF8, 0x18, 0xF8, 0x18, 0x18, 0x18,
    0x36, 0x36, 0x36, 0x36, 0xF6, 0x36, 0x36, 0x36,
    0x00, 0x00, 0x00, 0x00, 0xFE, 0x36, 0x36, 0x36, 
    0x00, 0x00, 0xF8, 0x18, 0xF8, 0x18, 0x18, 0x18, 
    0x36, 0x36, 0xF6, 0x06, 0xF6, 0x36, 0x36, 0x36, 
    0x36, 0x36, 0x36, 0x36, 0x36, 0x36, 0x36, 0x36, 
    0x00, 0x00, 0xFE, 0x06, 0xF6, 0x36, 0x36, 0x36, 
    0x36, 0x36, 0xF6, 0x06, 0xFE, 0x00, 0x00, 0x00, 
    0x36, 0x36, 0x36, 0x36, 0xFE, 0x00, 0x00, 0x00,
    0x18, 0x18, 0xF8, 0x18, 0xF8, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0xF8, 0x18, 0x18, 0x18, 
    0x18, 0x18, 0x18, 0x18, 0x1F, 0x00, 0x00, 0x00, 
    0x18, 0x18, 0x18, 0x18, 0xFF, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0xFF, 0x18, 0x18, 0x18, 
    0x18, 0x18, 0x18, 0x18, 0x1F, 0x18, 0x18, 0x18, 
    0x00, 0x00, 0x00, 0x00, 0xFF, 0x00, 0x00, 0x00, 
    0x18, 0x18, 0x18, 0x18, 0xFF, 0x18, 0x18, 0x18,
    0x18, 0x18, 0x1F, 0x18, 0x1F, 0x18, 0x18, 0x18, 
    0x36, 0x36, 0x36, 0x36, 0x37, 0x36, 0x36, 0x36, 
    0x36, 0x36, 0x37, 0x30, 0x3F, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x3F, 0x30, 0x37, 0x36, 0x36, 0x36, 
    0x36, 0x36, 0xF7, 0x00, 0xFF, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0xFF, 0x00, 0xF7, 0x36, 0x36, 0x36, 
    0x36, 0x36, 0x37, 0x30, 0x37, 0x36, 0x36, 0x36, 
    0x00, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0x00, 0x00,
    0x36, 0x36, 0xF7, 0x00, 0xF7, 0x36, 0x36, 0x36, 
    0x18, 0x18, 0xFF, 0x00, 0xFF, 0x00, 0x00, 0x00, 
    0x36, 0x36, 0x36, 0x36, 0xFF, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0xFF, 0x00, 0xFF, 0x18, 0x18, 0x18, 
    0x00, 0x00, 0x00, 0x00, 0xFF, 0x36, 0x36, 0x36, 
    0x36, 0x36, 0x36, 0x36, 0x3F, 0x00, 0x00, 0x00, 
    0x18, 0x18, 0x1F, 0x18, 0x1F, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x1F, 0x18, 0x1F, 0x18, 0x18, 0x18, 
    0x00, 0x00, 0x00, 0x00, 0x3F, 0x36, 0x36, 0x36, 
    0x36, 0x36, 0x36, 0x36, 0xFF, 0x36, 0x36, 0x36,
    0x18, 0x18, 0xFF, 0x18, 0xFF, 0x18, 0x18, 0x18, 
    0x18, 0x18, 0x18, 0x18, 0xF8, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x1F, 0x18, 0x18, 0x18, 
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 
    0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0,
    0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 
    0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x76, 0xDC, 0xC8, 0xDC, 0x76, 0x00,
    0x00, 0x78, 0xCC, 0xF8, 0xCC, 0xF8, 0xC0, 0xC0,
    0x00, 0xFC, 0xCC, 0xC0, 0xC0, 0xC0, 0xC0, 0x00,
    0x00, 0x00, 0xFE, 0x6C, 0x6C, 0x6C, 0x6C, 0x00,
    0xFC, 0xCC, 0x60, 0x30, 0x60, 0xCC, 0xFC, 0x00,
    0x00, 0x00, 0x7E, 0xD8, 0xD8, 0xD8, 0x70, 0x00,
    0x00, 0x66, 0x66, 0x66, 0x66, 0x7C, 0x60, 0xC0,
    0x00, 0x76, 0xDC, 0x18, 0x18, 0x18, 0x18, 0x00,
    0xFC, 0x30, 0x78, 0xCC, 0xCC, 0x78, 0x30, 0xFC,
    0x38, 0x6C, 0xC6, 0xFE, 0xC6, 0x6C, 0x38, 0x00,
    0x38, 0x6C, 0xC6, 0xC6, 0x6C, 0x6C, 0xEE, 0x00,
    0x1C, 0x30, 0x18, 0x7C, 0xCC, 0xCC, 0x78, 0x00,
    0x00, 0x00, 0x7E, 0xDB, 0xDB, 0x7E, 0x00, 0x00,
    0x06, 0x0C, 0x7E, 0xDB, 0xDB, 0x7E, 0x60, 0xC0,
    0x38, 0x60, 0xC0, 0xF8, 0xC0, 0x60, 0x38, 0x00,
    0x78, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0x00,
    0x00, 0x7E, 0x00, 0x7E, 0x00, 0x7E, 0x00, 0x00,
    0x18, 0x18, 0x7E, 0x18, 0x18, 0x00, 0x7E, 0x00,
    0x60, 0x30, 0x18, 0x30, 0x60, 0x00, 0xFC, 0x00,
    0x18, 0x30, 0x60, 0x30, 0x18, 0x00, 0xFC, 0x00,
    0x0E, 0x1B, 0x1B, 0x18, 0x18, 0x18, 0x18, 0x18,
    0x18, 0x18, 0x18, 0x18, 0x18, 0xD8, 0xD8, 0x70,
    0x18, 0x18, 0x00, 0x7E, 0x00, 0x18, 0x18, 0x00,
    0x00, 0x76, 0xDC, 0x00, 0x76, 0xDC, 0x00, 0x00,
    0x38, 0x6C, 0x6C, 0x38, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x18, 0x18, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x18, 0x00, 0x00, 0x00,
    0x0F, 0x0C, 0x0C, 0x0C, 0xEC, 0x6C, 0x3C, 0x1C,
    0x58, 0x6C, 0x6C, 0x6C, 0x6C, 0x00, 0x00, 0x00,
    0x70, 0x98, 0x30, 0x60, 0xF8, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x3C, 0x3C, 0x3C, 0x3C, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

unsigned short g_4x4_font[256]={
  0x0000,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,//7
  0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,//15
  0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,//23
  0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,//31
  0x0000/* */,0x4044/*!*/,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0x0044/*'*/,//39
  0x2442/*(*/,0x4224/*)*/,0x4F65/***/,0xFFFF,0x8400/*,*/,0x0600/*-*/,0x4000/*.*/,0x8421/*/*/,//47
  0x6996/*0*/,0xF2A6/*1*/,0xF296/*2*/,0xF17F/*3*/,0x1F99/*4*/,0x7F8F/*5*/,0xF9F8/*6*/,0x111F/*7*/,//55
  0x7DBE/*8*/,0xFFFF,0x0404/*:*/,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,//63

  0xFFFF,0x9F96/*A*/,0xADAC/*B*/,0xF88F/*C*/,0xE99E/*D*/,0xFFFF,0xFFFF,0xF98F/*G*/,//71
  0x9F99/*H*/,0xF44F/*I*/,0xFFFF,0x9ACB/*K*/,0xF888/*L*/,0x99BF/*M*/,0xFFFF,0xFFFF,//79
  0x8F9F/*P*/,0xFFFF,0xaF9F/*R*/,0xFFFF,0x444F/*T*/,0xF999/*U*/,0x6999/*V*/,0xFFFF,//87
  0xFFFF,0x4F99/*Y*/,0xFFFF,0x6446/*[*/,0x1248/*\*/,0x6226/*]*/,0x00A4/*^*/,0xF000/*_*/,//95
  0xFFFF,0x9F96/*a*/,0xADAC/*b*/,0xF88F/*c*/,0xE99E/*d*/,0xF8EF/*e*/,0x8E8F/*f*/,0xF98F/*g*/,//103
  0x9F99/*h*/,0xF44F/*i*/,0xFFFF,0x9ACB/*k*/,0xF888/*l*/,0x99BF/*m*/,0x99BD/*n*/,0xF99F/*o*/,//111
  0x8F9F/*p*/,0xFFFF,0xaF9F/*r*/,0x7F8F/*s*/,0x444F/*t*/,0xF999/*u*/,0x6999/*v*/,0xFB99/*w*/,//119
  0x9669/*x*/,0x4F99/*y*/,0xF42F/*z*/,0xFFFF,0x4444/*|*/,0xFFFF,0xFFFF,0xFFFF,
  
  0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,
  0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,
  0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,
  0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,
  0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,
  0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,
  0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,
  0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,

  0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,
  0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,
  0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,
  0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,
  0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,
  0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,
  0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,
  0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,
};

#endif