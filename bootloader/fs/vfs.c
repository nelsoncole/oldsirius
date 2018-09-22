/* Khole OS v0.3
 * Copyright (C) 2017,2018 (Nelson Sapalo da Silva Cole)
 */

#include <io.h>
#include <fs/fs.h>
#include <fs/vfs.h>
#include <string.h>
#include <part.h>
/*TODO
typedef struct vfs {
	uint8_t 	length;		// 0 Length Directory
	uint8_t   	flg;		// 1 (bit 0 arquivo/directory) (bit 1 block/no block) 
	uint16_t  	year;		// 2 Number of years since 0 to 65536 
	uint8_t 	month;  	// 4 Month of the year from 1 to 12 
	uint8_t  	day;		// 5 Day of the month from 1 to 31 
	uint8_t  	hour;		// 6 Hour of the day from 0 to 23 
	uint8_t  	minute; 	// 7 Minute of the hour from 0 to 59 
	uint8_t  	second; 	// 8 Second of the minute from 0 to 59 
	// Offset from Greenwich Mean Time (GMT) in number of 15 
	// min intervals from -48 (West) to + 52 (East) recorded according to 7.1.2 
	uint8_t 	GMT;		// 9
	char 		rsv[51-10]; 	// 50-10 
	uint16_t	bytePerSectrs;  // 51
	uint16_t	sectorsPerblock;// 53
	uint32_t	count;		// 55
	uint32_t	dataSize;       // 59
	uint8_t  	devnum;		// 63
	char 		filename[256-64];

}__attribute__ ((packed)) vfs_t;

typedef struct vfsdirectory {
	uint8_t len;
	char filename[254];
	//uint8_t eof;
}__attribute__ ((packed)) vfsdirectory_t;


typedef struct vfsfile {
	uint32_t lba;
	// ...	
}__attribute__ ((packed)) vfsfile_t;
*/


// *path 
//  X: (representa o disco)
//  /  (representa um dirextory)
//  String (representa o nome de um arquivo ou directory



#define fscall(addr,arg1,arg2,arg3,arg4)({\
uint32_t ret;\
__asm__ __volatile__("\
push %%edx;\
push %%ecx;\
push %%ebx;\
push %%eax;\
call *%k1;\
addl $0x16,%%esp":"=a"(ret):"r"(addr),"a"(arg1),"b"(arg2),"c"(arg3),"d"(arg4));ret;})

struct vfscommand {
  char id;
  void *r;
  void *w;
};
volatile struct vfscommand fstable[] = {
{0x00, 0,0},{0x01,read_iso9660,0},{0x02,0,0},{0x03,0,0},{0x04,0,0},
{0x05,0,0},{0x06,0,0},{0x07,0,0},{0x08,0,0},{0x09,0,0},
{0x0a,0,0},{0x0b,0,0},{0x0c,0,0},{0x0d,0,0},{0x0e,0,0},
{0x0f,0,0},
/*0x10,0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,0x1a,0x1b,0x1c,0x1d,0x1e,0x1f,
0x20,0x21,0x22,0x23,0x24,0x25,0x26,0x27,0x28,0x29,0x2a,0x2b,0x2c,0x2d,0x2e,0x2f,
0x30,0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,0x39,0x3a,0x3b,0x3c,0x3d,0x3e,0x3f,*/
};


static _Bool fragread(int dev,void *buffer,uint32_t lba,uint32_t count);
_Bool vfs_read(void *out,char *path)
{
	int i;
	uint32_t retval = 1;
	uint32_t stlba = 0;
	uint32_t count = 0;
	uint32_t dev = -1;
	stpart_t  stpart = indexpart(path);
	if(stpart.error)return 1;
	if(!(part[stpart.index].p)) return 1;

	

	// TODO, Aqui devemos allocar 4MB
	char *vfsb = (char*)malloc(0x1000);
	vfs_t *vfs = (vfs_t*)malloc(256);

	
	void *addr = fstable[part[stpart.index].type].r;
	retval = fscall(addr,(uint32_t)part[stpart.index].devnum,(char*)path +stpart.pathstart,(char*)vfsb,0);
	if(retval) {
		free(vfs);
		free(vfsb);
		return 1;
	}


	memcpy(vfs,vfsb,256);

	//read sub directory
	uint32_t offset =256, length =256;



	if(vfs->flg &1) {
		retval = 256;
		for(i=0;i <= count;i++) {
			memcpy(out,vfsb,length);
			offset += length = (*(uint8_t*)(vfsb + offset));
		}

		retval = 0;
		
	}

	// Read file

	else {
		dev = vfs->devnum;
		stlba = (*(uint32_t*)(vfsb + 256));

		count = (vfs->dataSize/vfs->bytePerSectrs);
		if((vfs->dataSize%vfs->bytePerSectrs))count +=1;


		if(vfs->flg &2) { // Sem fragmento


			retval = fragread(dev,out,stlba,count);


		}

		else { // fragmentado
			printf("Disco fragmentado\n");
			while(true);
		}
	}

	free(vfs);
	free(vfsb);
   	return retval;
}

static _Bool nofragread()
{

	return 1;

}

static _Bool fragread(int dev,void *buffer,uint32_t lba,uint32_t count)
{

	return read_sector(buffer,lba,count,dev);

}

_Bool vfs_write(void *in,char *path)
{

	return 1;

}


int *rdir(void *out,char *path)
{


	return (path);

}
