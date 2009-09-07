#include <drivers/hdd/ata.h>
#include <drivers/hdd/ataatapi.h>
#include <lib/string.h>

void test_iso(void){
	unsigned int controller, hdd;
	for(controller=0;controller<2;controller++)
	if(isControllerPresent(controller))
	for(hdd=0;hdd<2;hdd++)
	if(isHddPresent(controller,hdd)){
		/*mi interessa il 16 settore, per settori da 2048 byte, e per "blocchi" da 256 byte*/
		/*HACK: intanto prendo il primo settore :)*/
		int sector=2048*15/512;
		int counter=0;/*byte non nulli*/
		unsigned char buffer[512];
		printf("Testing (%x:%x):\"",controller,hdd);
		while(1){
			int i;
			readSector(controller,hdd,sector,buffer);
			for(i=0;i<512;i++)
				if(buffer[i]!=0){
					counter++;
					if(counter>8){
						printf("\"");
						break;
					}
					printf("%x,%x;",512*sector+i,buffer[i]);
				}
			sector++;
			printf(".");
		}
	}
}

