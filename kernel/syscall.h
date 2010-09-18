#pragma once

#define syscallzero(a) asm("int $0x80"::"a"(a))
#define syscallone(a,b) asm("int $0x80"::"a"(a),"b"(b))
#define syscalltwo(a,b,c) asm("int $0x80"::"a"(a),"b"(b),"c"(c))
#define syscallthree(a,b,c,d) asm("int $0x80"::"a"(a),"b"(b),"c"(c),"d"(d))

#define reboot() syscallzero(88)
#define syswrite(a) syscallone(250,a)
#define syssleep(a) syscallone(251,a)
