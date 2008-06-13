#ifndef KERNEL_H_
#define KERNEL_H_

#include "multiboot.h"
#include "../drivers/screen/screen.h"

void halt();

void _kmain(void);

#endif
