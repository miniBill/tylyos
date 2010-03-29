#include <drivers/hdd/ata.h>
#include <drivers/hdd/ataatapi.h>
#include <lib/string.h>

//pHACK : passing 0 as console argument to printf, not good

void test_iso ( void )
{
    unsigned int controller, hdd;
    for ( controller=0;controller<2;controller++ )
        if ( isControllerPresent ( controller ) )
            for ( hdd=0;hdd<2;hdd++ )
                if ( isHddPresent ( controller,hdd ) )
                {
                    /*mi interessa il 16 settore, per settori da 2048 byte, e per "blocchi" da 256 byte*/
                    /*HACK: intanto prendo il primo settore :)*/
                    int sector=2048*15/512;
                    int counter=0;/*byte non nulli*/
                    unsigned char buffer[512];
                    printf (0, "Testing (%x:%x):\"",controller,hdd );//pHACK
                    while ( 1 )
                    {
                        int i;
                        readSector ( controller,hdd,sector,buffer );
                        for ( i=0;i<512;i++ )
                            if ( buffer[i]!=0 )
                            {
                                counter++;
                                if ( counter>8 )
                                {
                                    printf (0, "\"" );//pHACK
                                    break;
                                }
                                printf (0, "%x,%x;",512*sector+i,buffer[i] );//pHACK
                            }
                        sector++;
                        printf (0, "." );//pHACK
                    }
                }
}

