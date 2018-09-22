#ifndef __sfs_h__
#define __sfs_h__
#include <typedef.h>

// Bolock Volume Descriptor
typedef struct sfs 
{					// offset 	size 		descriptor
	char vid[8];			// 0		8		Volume ID  
	uint32_t rid;   		// 8		4		Revision ID
	char rsvd0[64-12];       	// 12           52		reserved
	uint32_t numberofsectorl;	// 64 		4		Total number of sector (LBAl)
	uint32_t numberofsectorh;	// 68		4		Total number of sector (LBAh)
	uint32_t numberofsectorR;	// 72		4		Number of sectors reserved
	uint16_t logicalblockSize; 	// 76		2		Bytes per sector
	uint16_t sectorperblock;	// 78		2		Sector Per Block
	uint16_t devnumber;		// 80		2		Device number
	uint32_t bitmapl;		// 82		4		Bitmap	(LBAl)
	uint32_t bitmaph;		// 86		4		Bitmap	(LBAh)		
	uint32_t bitmaplength;		// 90		4		Bitmap Size, em sectors
	uint32_t bootsector;		// 94		4		Boot Sector (for stage1) (LBA)
	uint16_t bootlength;		// 98		2		Boot size, em sectors
	char rootdirectory[64];		// 100		64		Root directory entry
	uint32_t datal;			// 164		4		Data Area (LBAl)
	uint32_t datah;			// 168		4		Data Area (LBAh)
	char rsvd1[512-172];		// 172		--		reserved
}__attribute__ ((packed)) sfs_t;

// Directory structure
typedef struct sfsdirectory 
{					// offset	size	descriptor
	uint8_t lengthdirectory;	// 0		1	directory size
	uint32_t LBAl;			// 1		4	I-node (LBAl)
	uint32_t LBAh;			// 5		4	I-node (LBAh)
	uint32_t datalength;		// 9		4	Data length (size of extent). 
	uint8_t  numofyear;		// 13		1	Number of years since 2018 
	uint8_t  month;  		// 14		1	Month of the year from 1 to 12 
	uint8_t  day;			// 15		1	Day of the month from 1 to 31 
	uint8_t  hour;			// 16		1	Hour of the day from 0 to 23 
	uint8_t  minute; 		// 17		1	Minute of the hour from 0 to 59 
	uint8_t  second; 		// 18		1	Second of the minute from 0 to 59 
	uint8_t GMT;			// 19		1	Offset from Greenwich Mean Time (GMT) in number of 15 
					// min intervals from -48 (West) to + 52 (East) recorded according to 7.1.2 
	uint8_t flag;			// 20		1 	flags 
	uint8_t lengthoffilename;	// 21		1	filename size
	char	rsvd0[32-22];		// 22		--	reserved 
	char filename[256-32];		// 32		--	filename ID (223)		

}__attribute__ ((packed)) sfsdirectory_t;

#endif
