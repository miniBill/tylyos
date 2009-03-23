#ifndef STDIO_H_
#define STDIO_H_

inline unsigned char inb(int portnum);
inline unsigned short inw(unsigned short portnum);
inline void outb(int portnum, int data);
inline void outw(unsigned short portnum, unsigned short data);
void io_wait(void);

#endif
