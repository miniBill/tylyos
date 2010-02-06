#ifndef IO_H_
#define IO_H_

#define MAXN 32 /*lunghezza massima in cifre di un numero*/

unsigned int strlen(const char * string);

void strcpy(const char *source, char *dest);

void itoa(int a, char buff[11]);

void itobase(int a, unsigned short base, char * buff);

int printf(unsigned int console, const char* format, ...);

int findchar(const char *str,char c,unsigned int i);

void substr(char *in,char *out,unsigned int max,unsigned int start,int count);
#endif
