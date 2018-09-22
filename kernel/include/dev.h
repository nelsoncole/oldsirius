#ifndef __dev_h__
#define __dev_h__
#include <typedef.h>
#include <drivers/pci.h>
// Esta estrutura será usanda, como saíde de informações para
// os draivers de discos de mais alto nível,
// como por exemplo o montador de partições ou VFS
typedef struct disk
{	uint32_t id;
	char 	 interface[8];
	uint8_t  type:1;
	uint8_t  removable:1;
	uint8_t  rsvd:6;
	char 	 serialnumber[20];
	char     firmwarerevision[8];
	char	 modelnumber[40]; 
	uint32_t numberofsectorl;	// 	Total number of sector (LBAl)
	uint32_t numberofsectorh;	// 	Total number of sector (LBAh)
	uint16_t logicalblockSize; 	//	Bytes per sector
    	struct disk *next;
}__attribute__((packed)) disk_t;

// ATA
int ata_initialize();
void ata_irq_handler1(void);
void ata_irq_handler2(void);

int read_sector(void *buf,uint64_t addr,uint16_t count,int dev);
int write_sector(void *buf,uint64_t addr,uint16_t count,int dev);


// USB
int ehci_init(void);


//AUDIO
int hda_init(void);


//Video
void gpu();


// NET
int nic_test();


#endif
