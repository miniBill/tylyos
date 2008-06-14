#ifndef IO_H_
#define IO_H_

#define MAXN 32 /*lunghezza massima in cifre di un numero*/

int strlen(const char * string);

void itoa(int a,char buff[11]);

void itobase(int a,int base,char * buff);

int printf(const char* format, int val);
#endif
