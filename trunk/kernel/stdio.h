#ifndef STDIO_H_
#define STDIO_H_

#define MAXN 32 /*lunghezza massima in cifre di un numero*/

int strapp(char* dest,char* format,void* p);

unsigned char inb(int portnum);
void outb(int data, int portnum);

#endif

