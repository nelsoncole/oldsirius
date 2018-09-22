#include <io.h>
#include <string.h>
#include <fs/fs.h>
#include <executive/part.h>


//TODO esta variavel foi erdada do boot
uint32_t part_next_id  = 1;
uint32_t part_vaga = 1;
extern uint32_t pmboot_info;
uint32_t bootdevice = -1;

// d0:, d1:, d2:, d10:, ... dn:

char *dID[] ={
"d0: ","d1: ","d2: ","d3: ","d4: ","d5: ","d6: ","d7: ","d8: ","d9: ",
"d10:","d11:","d12:","d13:","d14:","d15:","d16:","d17:","d18:","d19:",
};


static uint32_t MoutDisk(int dev,int type)
{
	char *buffer =(char*)kalloc(1);

	uint32_t retval = 0;


	if(type) {
		if((read_sector(buffer,16,1,dev))) {
			printf("Read disk%d error\n",dev);
			retval = 0;
		}

		if(!(strncmp(buffer +1,"CD001",5)) && (buffer[0] == 1)) {

			if ((bootdevice >>24 &0xff) == dev) {
			strncpy(part[0].volumeID,dID[0],4);
			part[0].p = 1;
			part[0].type = PART_CD001;
			part[0].devnum = dev;
			printf("Partition Type: CDFS (ISO 9660 Volume Primary) Volume ID %s\n",part[0].volumeID);
			retval = PART_CD001;



			}


			else {
			strncpy(part[part_vaga].volumeID,dID[part_vaga],4);
			part[part_vaga].p = 1;
			part[part_vaga].type = PART_CD001;
			part[part_vaga].devnum = dev;
			printf("Partition Type: CDFS (ISO 9660 Volume Primary) Volume ID %s\n",part[part_vaga].volumeID);
			retval = PART_CD001;


			part_vaga = part_next_id;


			}
		}

    	
	}
	else if(!type) {

		if((read_sector(buffer,0,1,dev))!=0) {
			printf("Read disk%d error\n",dev);
			retval = 0;
		}
		
		//MBR

		part_vaga = part_next_id;

		printf ("Partition Type: (MBR)\n");

	}
	kfree_alloc(buffer);

	return (retval);
}

static int MoutPart(int dev, int part)
{

	return 0;

	
}

void part_initialize()
{

	char *mboot = (char*)pmboot_info;
	char *mbootDevice = (char*) (*(uint32_t*)(mboot + 4 ));
	bootdevice = (*(uint32_t*)(mbootDevice + 0x20));



	printf("Partition initialize\n");
      	memset(part,0,32);

    	MoutDisk((bootdevice >>24 &0xff),(bootdevice &0xff)); // Mount disk boot
	part_next_id++;

	// Check partition d0
	if(!(part[0].p))
		printf("Partition (d0:), not present\n");


}

stpart_t indexpart(char *src)
{
	stpart_t retval;

	if((src[0] == 'd') && (src[2] == ':')) {
		retval.index = (src[1] - 48);
		retval.pathstart = 3;
		retval.error = 0;
	}
	else if((src[0] == 'd') && (src[3] == ':')) {
		retval.index = ((src[1] - 48) * 10) + ((src[2] - 48));
		retval.pathstart = 4;
		retval.error = 0;
	}
	else { 

		retval.error = 1; // error
	}
	return retval;
}
