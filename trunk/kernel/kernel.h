#ifndef KERNEL_H_
#define KERNEL_H_

#include "multiboot.h"
#include "../drivers/screen/screen.h"

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
    putxy(COLUMNS-4,i,'[');
    putxy(COLUMNS-3,i,'O');
    putxy(COLUMNS-2,i,'K');
    putxy(COLUMNS-1,i,']');
    cwritexy(COLUMNS-3,i,Light_Green);
    cwritexy(COLUMNS-2,i,Light_Green);
}
#endif
