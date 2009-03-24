#ifndef KERNEL_H_
#define KERNEL_H_

#include "multiboot.h"
#include "../drivers/screen/screen.h"

void reboot(void);

void sleep(int time);

void _kmain(multiboot_info_t* mbd, unsigned int magic);

#endif
