#include <drivers/hdd/ata.h>
#include <lib/string.h>

void test_iso(void){
	unsigned int controller, hdd;
	for(controller=0;controller<2;controller++)
	if(isControllerPresent(controller))
	for(hdd=0;hdd<2;hdd++)
	if(isHddPresent(controller,hdd)){
		/*mi interessa il 16 settore, per settori da 2048 byte, e per "blocchi" da 256 byte*/
		int sector=2048*15/256;
		unsigned char buffer[256];
		printf("Testing (%x)%x:%x",0,controller,hdd,0);
		readSector(controller,hdd,sector,buffer);
		printf("%c%c%c%c%c%c\n",&buffer[0],&buffer[1],&buffer[2],&buffer[3],&buffer[4],&buffer[5]);
	}
}

