#ifndef KERNEL_H_
#define KERNEL_H_

#include "multiboot.h"
#include "syscall.h"
#include <drivers/screen/screen.h>

void sleep ( unsigned int time );

void _kmain ( multiboot_info_t* mbd, unsigned int magic );

int check ( const char * output,int offset );

void OK ( int i );

void NO ( int i );

void greendot ( void );

void kernelPanic ( char *sender,char *message );

void do_life(void);

#endif
