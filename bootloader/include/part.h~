#ifndef __PART_H__
#define __PART_H__
#include <typedef.h>

#define PART_EMPTY	0x00
#define PART_FAT32	0x0c // FAT32 Windows 95 LBA
#define PART_CD001	0x01


volatile struct part{

    uint8_t	p:1;
    uint8_t	rw:1;
    uint8_t	rsv:6;
    char    	volumeID[4];
    uint8_t     devnum;
    uint8_t     type;
    uint32_t    totalSpace;
    uint32_t    freeSpace;
    uint32_t    busySpace;
    char     	name[32];

}__attribute__ ((packed)) part[32];

typedef volatile struct stpart {
	uint8_t index;
	uint8_t pathstart;
	uint8_t error;
	uint8_t rsv;
} __attribute__ ((packed)) stpart_t;
//part.c
void part_initialize();
int mout_dev(int dev);
stpart_t indexpart(char *s);






#endif
