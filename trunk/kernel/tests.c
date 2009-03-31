#include "tests.h"

#include <lib/string.h>
#include <memory/memory.h>
#include "multiboot.h"

#define FAST_TESTS

typedef int (*test)(void);

int putreadtest(void){
    char output[16]="Prova put/read.";
    int i;
    put('P');
    for(i=COLUMNS+1;i<COLUMNS+6;i++)
        put(readi(i));
    write("put/read.");
    return check(output,0);
}

int pointertest(void){
    char pointer[17]="Prova puntatore.";
    write(pointer);
    return check(pointer,0);
}

int itoatest(void){
    char conversion[4]={0};
    char output[8]="123,-A0";
    int i;
    write("Prova itoa:");
    for(i=0;i<4;i++)
        conversion[i]=0;
    itoa(123,conversion);
    write(conversion);
    write(",");
    itobase(-160,16,conversion);
    write(conversion);
    write(".");
    return check(output,11);
}

int printftest(void){
    char output[13]="10,CA,a,ciao";
    write("Prova printf:");
    printf("%d,%x,%c,%s",10,0xCA,'a',"ciao");
    printf(" base stack: %x end: %x",getEBP(),getESP());
    return check(output,13);
}

int dinamictestOne(void){
    unsigned int pointera,pointerb,pointerc;
    int ret=1;
    write("Test allocazione dinamica: fase1 ");

    printf("heap start %d",mallocMemoryStart);
    pointera=(unsigned int)kmalloc(4);
    pointerb=(unsigned int)kmalloc(4);
    pointerc=(unsigned int)kmalloc(4);
/*
printf("\n%d %d %d\n",pointera,pointerb,pointerc);
*/
    if(!(pointerc-pointerb == 12 &&
            pointerb-pointera == 12))
        ret=0;

    kfree((void*)pointera);
    if(!(pointera==(unsigned int)kmalloc(4)))
    {
        printf("a\n");
        ret=0;
    }
    kfree((void*)pointerb);
    if(!(pointerb==(unsigned int)kmalloc(4)))
    {
        printf("b\n");
        ret=0;
    }
    kfree((void*)pointerc);
    if(!(pointerc==(unsigned int)kmalloc(4)))
    {
        printf("c\n");
        ret=0;
    }
    return ret;
}

int dinamictestTwo(void){
    char *dinamicFirst,*dinamicSecond;
    int i;
    for(i=0;i<27;i++)
        put(' ');
    write("fase2 ");
    dinamicFirst=(char*)kmalloc(4);
    printf("0x%x=",(unsigned int)dinamicFirst);

    dinamicSecond=dinamicFirst;
    kfree(dinamicFirst);

    dinamicFirst=(char*)kmalloc(4);

    printf("=0x%x.",(unsigned int)dinamicFirst);

    return dinamicFirst==dinamicSecond;
}

/*int hddTest(void){
	char buffer[256];
	char first;
}*/

void run(test tests[]){
    int i;
    int t=row()+1;
    for(i=0;tests[i]!=0;i++){
        NO(t);
        greendot();
        if((*tests[i])())
            OK(t++);
        else
            t++;
        writeline("");
    }
}

void doTests(void){
        /*REMEMBER TO KEEP SIZE=ITEMS+1!!!*/
#ifdef FAST_TESTS
        test tests[2]={
#else
        test tests[6]={
            putreadtest,
            pointertest,
            itoatest,
            printftest,
#endif
            dinamictestOne,
            /*dinamictestTwo*/
        };
        run(tests);
}

