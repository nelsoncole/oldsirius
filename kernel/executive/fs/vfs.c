#include <io.h>
#include <fs/fs.h>
#include <fs/vfs.h>
#include <executive/part.h>
#include <string.h>


#define fscall(addr,arg1,arg2,arg3,arg4)({\
uint32_t ret;\
__asm__ __volatile__("\
push %%edx;\
push %%ecx;\
push %%ebx;\
push %%eax;\
call *%k1;\
addl $0x16,%%esp":"=a"(ret):"r"(addr),"a"(arg1),"b"(arg2),"c"(arg3),"d"(arg4));ret;})

extern char* kmonth [];
struct vfscommand {
  char id;
  void *r;
  void *w;
};
struct vfscommand fstable[] = {
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
	int dev = -1;

	stpart_t  stpart = indexpart(path);

	if(stpart.error)return 1;
	if(!(part[stpart.index].p)) return 1;

	

	// TODO, Aqui devemos allocar 4MB
	char *vfsb = (char*)kalloc(8); // 32KB
	vfs_t *vfs = (vfs_t*)malloc(256);
	
	void *addr = fstable[part[stpart.index].type].r;
	retval = fscall(addr,part[stpart.index].devnum,path +stpart.pathstart,vfsb,0);
	if(retval) {
		free(vfs);
		kfree_alloc(vfsb);
		return 1;
	}

	memcpy(vfs,vfsb,256);
	
	//read sub directory
	uint32_t offset =256, length =256;
	if(vfs->flg &1) {
		printf("Read file error\n");
		retval = -1;
		
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
	kfree_alloc(vfsb);
   	return retval;
}

static _Bool nofragread()
{

	return 0;

}

static _Bool fragread(int dev,void *buffer,uint32_t lba,uint32_t count)
{

	return read_sector(buffer,lba,count,dev);

}

_Bool vfs_write(void *in,char *path)
{

	return 1;

}


int vfs_read_directory(char *out,const char *path)
{
	int i;
	uint32_t retval = 1;
	uint32_t stlba = 0;
	int dev = -1;

	stpart_t  stpart = indexpart((char*)path);

	if(stpart.error)return 1;
	if(!(part[stpart.index].p)) return 1;
	

	void *addr = fstable[part[stpart.index].type].r;
	retval = fscall(addr,part[stpart.index].devnum,path +stpart.pathstart,(char*)out,0);
	if(retval) {
		return (int) 1;
	}

	vfs_t *vfs = (vfs_t*)out;
	
	//read sub directory
	if(vfs->flg &1) {

		retval = 0;
		
	}

	// Read file

	else {
		printf("Read sub directory error\n");
		retval = -1;
	}



	return (int) (retval);

}

int rdir(const char *path) 
{



	int retval = -1;
	char *vfsb = (char*)kalloc(8);
	vfs_t *vfs = (vfs_t*)malloc(256);
	vfsdirectory_t *dir = (vfsdirectory_t*)malloc(256);
	

	if((vfs_read_directory(vfsb,path))) {
		printf ("Read rdir error\n");
		retval = -1;

	}else {

	memcpy(vfs,vfsb,256);

	uint32_t offset =256, length =256;
	int i;
	


	printf("Read directory Located: %s\nDetails:\nFilename: %s\nContents: %d items\n",path,vfs->filename,vfs->count);
	printf("Modified: %d %s %d %d:%d:%d GMT%d\n",vfs->day,kmonth[vfs->month],vfs->year,vfs->hour,vfs->minute,vfs->second,vfs->GMT);

	if(vfs->count) {

	for(i=0;i < vfs->count;i++) {

			length = (*(uint8_t*)(vfsb + offset));
			memcpy(dir,vfsb+offset,length);
			printf("%d >: %s\n",i,dir->filename);
			offset += length;

		}

	}	
	retval = 0;

	}
	free(dir);
	free(vfs);
	kfree_alloc(vfsb);
	return (int) retval;


}

int cd(const char *path) 
{



	int retval = -1;
	char *vfsb = (char*)kalloc(8);
	vfs_t *vfs = (vfs_t*)malloc(256);
	vfsdirectory_t *dir = (vfsdirectory_t*)malloc(256);
	

	if((vfs_read_directory(vfsb,path))) {
		retval = -1;

	}else {


	retval = 0;

	}

	free(vfs);
	kfree_alloc(vfsb);
	return (int) retval;


}


int open (FILE *fd,uint8_t flg) 
{




	return 0;
}
