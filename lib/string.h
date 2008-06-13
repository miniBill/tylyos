#ifndef IO_H_
#define IO_H_

#define MAXN 32 /*lunghezza massima in cifre di un numero*/

int strapp(char* dest,const char* format,unsigned int p);/*HACK*/
/*int strapp(char* dest,char* format,void* p);*/

int strlen(const char * string);

void itoa(int a,char buff[12]);

void itobase(int a,int base,char buff[12]);

#endif
