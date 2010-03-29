/* Copyright (C) 2008 Leonardo Taglialegne <leonardotaglialegne+tylyos@gmail.com>
* Copyright (C) 2008 Luca Salmin
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

#include "tests.h"

#include <lib/string.h>
#include <memory/memory.h>
#include <fs/iso/iso.h>
#include "multiboot.h"

#include <config.txt>

typedef int (*test)(void);

int pointertest ( void )
{
    char pointer[17] = "Prova puntatore.";
    write ( pointer ,0);
    return check ( pointer, 0 );
}

int printftest ( void )
{
    char output[17] = "10,CA,a,ci,-1,16";
    write ("Prova printf:" ,0);
    printf (0, "%d,%x,%c,%s,%d,%u", 10, 0xCA, 'a', "ci", -1, -0xFFFFFFF0);
    printf (0, " EBP: %x ESP: %x", getEBP(), getESP() );
    return check ( output, 13 );
}

int findchartest(void){
  char output[7]="0 5 -1";
  printf(0,"Prova findchar: %d %d %d",findchar("ottavo",'o',0),findchar("ottavo",'o',1),findchar("ottavo",'o',2));
  return check(output,16);
}

int substrtest(void){
  char output[5]="lisa";
  char buff[5]={0};
  substr("casali",buff,4,4,-1);
  substr("casali",buff+2,2,2,2);
  printf(0,"Prova substr: %s",buff);
  return check(output,14);
}

int splittest(void){
  char output[25]="1:\"r\"1:\"t\"1:\"l\"0:\"\"-1:\"\"";
  char buff[4]={0};
  int i=0;
  i=split("rotolo",buff,3,'o',0);
  printf(0,"Prova splittest: %d:\"%s\"",i,buff);
  buff[0]=buff[1]=buff[2]=buff[3]==0;
  i=split("rotolo",buff,3,'o',1);
  printf(0,"%d:\"%s\"",i,buff);
  buff[0]=buff[1]=buff[2]=buff[3]==0;
  i=split("rotolo",buff,3,'o',2);
  printf(0,"%d:\"%s\"",i,buff);
  buff[0]=buff[1]=buff[2]=buff[3]==0;
  i=split("rotolo",buff,3,'o',3);
  printf(0,"%d:\"%s\"",i,buff);
  buff[0]=buff[1]=buff[2]=buff[3]=0;
  i=split("rotolo",buff,3,'o',4);
  printf(0,"%d:\"%s\"",i,buff);
  return check(output,17);
}

int sprintftest(void){
  char output[16] = "10,CA,a,ciao,13";
  char buff[16]={0};
  int test = 13;
  write ("Prova sprintf:" ,0);
  sprintf(buff,16, "%d,%x,%c,%s,%d", 10, 0xCA, 'a', "ciao", test );
  printf(0,"%s",buff);
  return check ( output, 14 );
}

int dinamictestOne ( void )
{
    unsigned int pointera, pointerb, pointerc;
    int ret = 1;
    printf (0, "Test allocazione dinamica: fase1 " );

    printf (0, "\n    kernel heap start: 0x%x", HEAP_START );
    //printf (0, "\n    reserved kernel heap size:  %dMB and %dKB", ( userMemoryStart - mallocMemoryStart ) / 1000000, ( ( userMemoryStart - mallocMemoryStart ) % 1000000 ) / 1000 );
    printf (0, "\n    user memory start: 0x%x", user_start );
    pointera = ( unsigned int ) kmalloc ( 4 );
    pointerb = ( unsigned int ) kmalloc ( 4 );
    pointerc = ( unsigned int ) kmalloc ( 4 );
    /*
    printf("\n%d %d %d\n",pointera,pointerb,pointerc);
    */
    if ( ! ( pointerc - pointerb == 12 &&
             pointerb - pointera == 12 ) )
        ret = 0;

    kfree ( ( void* ) pointera );
    if ( ! ( pointera == ( unsigned int ) kmalloc ( 4 ) ) )
    {
        printf (0, "a\n" );
        ret = 0;
    }
    kfree ( ( void* ) pointerb );
    if ( ! ( pointerb == ( unsigned int ) kmalloc ( 4 ) ) )
    {
        printf (0, "b\n" );
        ret = 0;
    }
    kfree ( ( void* ) pointerc );
    if ( ! ( pointerc == ( unsigned int ) kmalloc ( 4 ) ) )
    {
        printf (0, "c\n" );
        ret = 0;
    }
    kfree ( ( void* ) pointera );
    kfree ( ( void* ) pointerb );
    kfree ( ( void* ) pointerc );
    return ret;
}

int dinamictestTwo ( void )
{
    char *dinamicFirst, *dinamicSecond;
    int i;
    for ( i = 0;i < 27;i++ )
        printf (0, " " );
    printf (0, "fase2 " );
    dinamicFirst = ( char* ) kmalloc ( 4 );
    printf (0, "0x%x=", ( unsigned int ) dinamicFirst );

    dinamicSecond = dinamicFirst;
    kfree ( dinamicFirst );

    dinamicFirst = ( char* ) kmalloc ( 4 );

    printf (0, "=0x%x.", ( unsigned int ) dinamicFirst );

    return dinamicFirst == dinamicSecond;
}

/*int hddTest(void){
 char buffer[256];
 char first;
}*/

int isoTest ( void )
{
    test_iso();
    return 1;
}

void run ( test tests[] )
{
    int i;
    int t = row(0);
    for ( i = 0;tests[i] != 0;i++ )
    {
        NO ( t );
        greendot();
        if ( ( *tests[i] ) () )
            OK ( t++ );
        else
            t++;
        nl(0);
    }
}

void doTests ( void )
{
    /*REMEMBER TO KEEP SIZE=ITEMS+1!!!*/
#ifdef FAST_TESTS
    test tests[2] = {
#else
    test tests[8] = {
        pointertest,
        printftest,
        substrtest,
        findchartest,
        splittest,
        sprintftest,
#endif
        /*isoTest,*/
        dinamictestOne,
        /*dinamictestTwo,*/
    };
    run ( tests );
}

