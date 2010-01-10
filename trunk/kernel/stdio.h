#ifndef STDIO_H_
#define STDIO_H_

unsigned char inb(int portnum);
unsigned short inw(unsigned short portnum);
void outb(int portnum, int data);
void outw(unsigned short portnum, unsigned short data);
void io_wait(void);

#endif
