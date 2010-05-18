/* Copyright (C) 2008 Luca Salmin
 *
 * This file is part of TylyOS.
 *
 * TylyOS is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * TylyOS is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with TylyOS.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef TASK_H
#define TASK_H

#define KERNEL_STACK_SIZE 5000
#define TASK_STACK_SIZE 5000

#define CURRENT_TSS_INDEX 5/*indice della gdt in cui si trova il descrittore*/
#define NEW_TSS_INDEX 6/*indice della gdt in cui si trova il descrittore*/
#define KERNEL_INTERRUPT_TSS_INDEX 7/*TODO: eliminare dalla gdt in quanto non è vitale*/
#define GARBAGE_TSS_INDEX 8
#define ISR_TSS_INDEX 10/*indice di partenza dei tss delle isr, da quì in poi non usare gli indici*/

unsigned short currentTSSselector,newTSSselector;
unsigned short kernelInterruptTSSselector;/*usato per i task gate degli interrupt*/

char stackKernel[KERNEL_STACK_SIZE];/*stack usato per il kernel una volta avviati i task*/

struct tss
{
    unsigned int        link;
    unsigned int        esp0;
    unsigned short      ss0, __ss0h;
    unsigned int        esp1;
    unsigned short      ss1, __ss1h;
    unsigned int        esp2;
    unsigned short      ss2, __ss2h;
    unsigned int        cr3;
    unsigned int        eip;
    unsigned int        eflags;
    unsigned int        eax, ecx, edx, ebx;
    unsigned int        esp;
    unsigned int        ebp;
    unsigned int        esi;
    unsigned int        edi;
    unsigned short      es, __esh;
    unsigned short      cs, __csh;
    unsigned short      ss, __ssh;
    unsigned short      ds, __dsh;
    unsigned short      fs, __fsh;
    unsigned short      gs, __gsh;
    unsigned short      ldtr, __ldtrh;
    unsigned short      trace, io_map_addr;
}  __attribute__ ( ( packed ) );

struct tss kernelTSS,kernelInterruptTSS;

#define MAX_TASK_NAME_LEN 20
struct taskStruct
{
    struct tss TSS;
    struct pagina *listaPagine;
    unsigned int procID;
    char stato;
    char nome[MAX_TASK_NAME_LEN];
    char privilegi;
    /*segnento codice*/
    unsigned int codeSegmentBase;
    unsigned int codeSegmentSize;
    /*segmento dati*/
    unsigned int dataSegmentBase;
    unsigned int dataSegmentSize;
    /*segnento stack*/
    unsigned int stackSegmentBase;
    unsigned int stackSegmentSize;
};

struct taskListElement
{
    struct taskListElement *next;
    struct taskStruct *task; 
};

struct taskListElement *taskListRoot; /*puntatore al primo elemento della lista dei task*/


void initTaskManagement();

/*setta un selettore di segmento TSS*/
/*num: indice nella GDT*/
/*base: base segmento*/
/*access: MEM_TSS_BUSY|MEM_TSS|MEM_KERNEL|MEM_RING1|MEM_RING2|MEM_USER|MEM_PRESENT|MEM_NOT_PRESENT*/
void TSSset(int num, unsigned int base, unsigned char access);

/*ritorna un procID libero*/
unsigned int getNewProcID();

/*funzione da usare per eseguire un task, il parametro path serve a dafinire il file in formato elf da cui caricare il task*/
int exec(char *path,char privilegi);
/*alloca le strutture ed aggiunge il task alla lista dei processi*/
/*ritorna l'id del task*/
int addTask(char nome[MAX_TASK_NAME_LEN],char privilegi);
int removeTask(unsigned int procID);
struct taskStruct *getTask(unsigned int procID);

/*funzione da utilizzare per l'allocazione di una nuova pagina per un task*/
struct pagina *allocaNuovaPagina ( unsigned int procID,unsigned int indirizzoLog );

/*funzione da utilizzare per la deallocazione di una pagina di un task*/
unsigned int deallocaPagina ( unsigned int procID,unsigned int indirizzoLog );

/*la funzione ritorna l'istanza di pagina richiesta, se la pagina non esiste ritorna 0*/
struct pagina *getPagina(unsigned int procID,unsigned int indirizzoLog);

/*funzione che si assicura che nel range definito da parametro un processo abbia delle pagine utilizzabili*/
unsigned int allocMemory(unsigned int procID, unsigned int baseLogicAddr,unsigned int size);

/*funzione per copiare dei dati in memoria ad un task non correntemente mappato*/
void memcpyToTask( char * source, unsigned int count, char * dest, unsigned int procID );

#endif

