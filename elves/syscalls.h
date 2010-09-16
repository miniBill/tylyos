#include <kernel/syscall.h>

#define STANDARD_INPUT 1
#define STANDARD_OUTPUT 2

typedef int File;

#define MAXN 32 /*lunghezza massima in cifre di un numero*/

int main(int argc, char *argv[]);

void exit(int status)
{
    syscallone(1,status);
}

void _start()
{
    int argc;
    char **argv;
    syscallone(5,0);
    asm( "mov %%eax, %0;"
          :"=r"(argc)
       );
    asm( "mov %%ebx, %0;"
          :"=r"(argv)
       );
    exit(main(argc,argv));
}

void exec(char *path)
{
    syscallone(3,path);
}

File mov2(File file,File newfile)
{
    File ret;
    syscalltwo(4,file,newfile);
    asm( "mov %%eax, %0;"
          :"=r"(ret)
       );
    return ret;
}

int fork()
{
    int ret;
    syscallone(2,0);
    asm( "mov %%eax, %0;"
          :"=r"(ret)
       );
    return ret;
}

int strcmp(const char *a, const char *b)
{
    while(*a && *b) {
        if(*a++ != *b++) return 1;
    }
    if(*a || *b) return 1;
    return 0;
}



File openFile(char *path,char mode)
{
    int ret;
    syscalltwo(252,path,mode);
    asm( "mov %%eax, %0;"
          :"=r"(ret)
       );
    return ret;
}

void closeFile(File file)
{
    syscallone(253,file);
}

int readFile(int file,char *buffer,unsigned int byteCount)
{
    int ret;
    syscallthree(254,file,buffer,byteCount);
    asm( "mov %%eax, %0;"
          :"=r"(ret)
       );
    return ret;
}

int writeFile(int file,char *buffer,unsigned int byteCount)
{   
    int ret;
    syscallthree(201,file,buffer,byteCount);
    asm( "mov %%eax, %0;"
          :"=r"(ret)
       );
    return ret;
}

unsigned int strlen(char *str)
{
    unsigned int ret=0;
    while(str[ret]!=0)
        ret++;
    return ret;

}

int findchar(char *str, char c, unsigned int i) {
  unsigned int l = strlen(str);
  i++;
  if (l == 0)//prevention >> debug
    return -1;
  unsigned int x = 0;
  do {
      if (str[x++] == c)
        i--;
    }
  while (x < l /*don't go over*/ && i != 0 /*returns on the (i+1)th occurence*/);
  if (i != 0)
    return -1;
  return x -1;
}

void substr(char *in, char *out, unsigned int max, unsigned int start, int count) {
  unsigned int l = strlen(in);
  if (count < 0)
    count = l - start;
  if (start >= l) {
      out[0] = 0;
      return;//these aren't the droids you're looking for
    }
  if ((unsigned int)count > max)//again, prevention >> debug
    count = max;
  unsigned int i;
  for (i = 0;count > 0;i++, count--)
    out[i] = in[start + i];
  out[i] = 0;//out could be full of garbage
}

int split(char *in, char *out, unsigned int max, char c, unsigned int i) {
  if (i == 0) {
      int index = findchar(in, c, 0);
      if (index < 0)
        return index;
      substr(in, out, max, 0, index);
      return strlen(out);
    }
  int index = findchar(in, c, i - 1);
  if (index < 0) {
      out[0] = 0;
      return index;
    }
  int medius = findchar(in, c, i);
  if (medius < 0)
    medius++;
  substr(in, out, max, index + 1, medius - index - 1);
  return strlen(out);
}

void strcpy(char *source, char *dest) {
  unsigned int c;
  unsigned int l=strlen(source);
  for (c = 0;c < l;c++)
    dest[c] = source[c];
  dest[c] = 0;
}


void uitobase(unsigned int a, unsigned short base, char * buff) {
  if (a == 0) {
      buff[0] = '0';
      buff[1] = 0;
    }
  else {
      int l;
      char temp[MAXN] = {0};
      buff[0] = 0;
      for(l = 0;a && (l < MAXN) ; a /= base, l++)
        temp[l] = "0123456789ABCDEF"[a % base];
      for(int i = l - 1, l = buff[0] == '-';i >= 0;i--)
        buff[l++] = temp[i];
      buff[l] = 0;
    }
}

void itobase(int a,unsigned short base, char * buff){
  if(a<0){
    a=-a;
    buff[0]='-';
    uitobase(a,base,buff+1);
  }
  else
    uitobase(a,base,buff);
}

void itoa(int a, char buff[11]) {
  itobase(a, 10, buff);
}

void put(char c)
{
    char tmp[2];
    tmp[0]=c;
    tmp[1]=0;
    writeFile(STANDARD_OUTPUT,tmp,1);
}


unsigned int printf(char* format, ...) {
  unsigned  int size = 0;
  char ** arg = (char**) & format;
  char buf[33];//Longest string will bit a binary int32, so 33 chars are enough
  unsigned int i;
  arg++;/*jump "format"*/
  unsigned int fl=strlen(format);
  for (i = 0;i < fl;i++) {
      if (format[i] == '%') {
          switch (format[i+1]) {
            case '%':
              put('%');
              size++;
              break;
            case 'd':
              itoa(* ((int *) arg++), buf);
              goto number;
              break;
            case 'u':
              uitobase(* ((int *) arg++), 10, buf);
              goto number;
              break;
            case 'x':
              uitobase(* ((int *) arg++), 16, buf);
              goto number;
              break;
            case 'o':
              uitobase(* ((int *) arg++), 8, buf);
              goto number;
              break;
            case 'b':
              uitobase(* ((int *) arg++), 2, buf);
              goto number;
              break;
            case 'c':
              put(* ((char *) arg++));
              size++;
              break;
            case 's':
              writeFile(STANDARD_OUTPUT,(char*) *arg,strlen((char*) *arg));    /*watch out: Deep Magic*/
              size += strlen((char *) * arg++);
              break;
number:
              writeFile(STANDARD_OUTPUT,buf,strlen(buf));
              size += strlen(buf);
              break;
            default:
              itoa(* ((int *) arg++), buf);
              writeFile(STANDARD_OUTPUT,"Malformed format string!",strlen("Malformed format string!"));
              goto number;
              break;
            }

          i++;
        }
      else {
          put(format[i]);
          size++;
        }
    }
  return size;
}


unsigned int get(char *buffer,unsigned int bufferSize)
{
    return readFile(STANDARD_INPUT,buffer,bufferSize);
}


void pipe(File descriptors[2])
{
    syscallone(200,descriptors);
}

unsigned int readLine(char *buffer,unsigned int bufferSize)
{
    unsigned int i=0;
    char c[2];
    buffer[0]=0;
    while(1)
    {   
        if(get(c,1))
        {  
            c[1]=0;
            switch(c[0])
            {
            case '\0':
                break;
            case '\n':
                printf("\n");
                return i;
                break;
            case '\b':
                if(i>0)
                {
                    printf("\b");
                    i-=1;
                    buffer[i]=0;
                }
                break;
            default:
                if(i<bufferSize-1)
                {  
                    printf(c);
                    buffer[i]=c[0];
                    buffer[i+1]=0;
                    i+=1;
                }
                break;
            }

        }
    }
}

char exist(char *path)
{
    int i=findchar(path,' ', 0);

    if(i>=0)
        path[i]=0;

    File r=openFile(path,5);
    closeFile(r);

    if(i>=0)
        path[i]=' ';

    if(r==-1)
        return 0;
    else
        return 1;
}

unsigned int forkExec(char *path,File _p[2])
{
    if(!exist(path))
        return 0; 
    File pipein[2];/*read 0,write 1*/
    File pipeout[2];/*read 0,write 1*/
    pipe(pipein);
    pipe(pipeout);
    unsigned int r=fork();
    if(r==0)
    {
        closeFile(pipein[1]);
        closeFile(pipeout[0]);
        mov2(pipein[0],STANDARD_INPUT);
        mov2(pipeout[1],STANDARD_OUTPUT);
        exec(path);
        exit(0);
    }
    closeFile(pipein[0]);
    closeFile(pipeout[1]);
    _p[0]=pipeout[0];
    _p[1]=pipein[1];

    return r;
}
