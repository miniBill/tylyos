#ifndef KERNEL_H_
#define KERNEL_H_

#include "multiboot.h"
#include "../drivers/screen/screen.h"

void halt();

void _kmain(/*multiboot_info_t* mbd, unsigned int magic */);

void kwrite(char * string);

void OK(int i);

void NO(int i);

#endif
