#ifndef STDIO_H_
#define STDIO_H_

unsigned char inb(int portnum);
void outb(int data, int portnum);
void io_wait(void);

#endif
