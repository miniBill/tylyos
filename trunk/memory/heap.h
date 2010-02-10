
#ifndef MEMORY_HEAP_H
#define MEMORY_HEAP_H

void *calloc ( unsigned int num, unsigned int size );
void* kmalloc ( unsigned int byte );
void kfree ( void *pointer );


/*struttura lista allocazioni*/
struct memoryArea
{
    unsigned int size;/*dimensione in byte*/
    struct memoryArea *next;/*indirizzo header della prossima area*/
} __attribute__ ( ( packed ) );

struct memoryArea *kmallocList;/*liste aree allocate e libere*/

#endif