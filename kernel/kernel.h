#ifndef KERNEL_H_
#define KERNEL_H_

#include "multiboot.h"
#include "../drivers/screen/screen.h"

void halt();

void _kmain(multiboot_info_t* mbd, unsigned int magic );

char * kpointer=(char *)0xb8000;

void kwrite(char * string){
    int k;
    for(k=0;
        string[k]!=0 && (((int)kpointer-0xb8000)/2)<(COLUMNS*ROWS);
        k++){
            *(kpointer++)=string[k];
            *(kpointer++)=7;
    }
}

void OK(int i){
    writexy(COLUMNS-4,i,"[OK]");
    cputxy(COLUMNS-3,i,Light_Green);
    cputxy(COLUMNS-2,i,Light_Green);
}

void NO(int i){
    writexy(COLUMNS-4,i,"[NO]");
    cputxy(COLUMNS-3,i,Light_Red);
    cputxy(COLUMNS-2,i,Light_Red);
}

#endif
