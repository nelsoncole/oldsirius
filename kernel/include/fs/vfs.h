#ifndef __vfs_h__
#define __vfs_h__


typedef struct vfsdirectory {
	uint8_t len;
	char filename[255];
	//uint8_t eof;
}__attribute__ ((packed)) vfsdirectory_t;


typedef struct vfsfile {
	uint32_t lba;
	// ...	
}__attribute__ ((packed)) vfsfile_t;
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




typedef struct fs 
{
	char *base;
	int  cnt;
	int  flag;
	char *tmpfname;

}FS;



#define FOPEN_MAX	(20)
#define NUMBER_OF_FILES (20)

/*
 * FILE:
 *     Estrutura padrão para arquivos.    
 *     
 */

typedef struct _iobuf
{
	char *_ptr;       //Current position of file pointer (absolute address).
	int   _cnt;       // number of available characters in buffer 
	char *_base;      //Pointer to the base of the file. the buffer
	int   _flag;      //Flags (see FileFlags). the state of the stream
	int   _file;      //UNIX System file descriptor
	int   _charbuf;   
	int   _bufsiz;
	char *_tmpfname;
}FILE;
//Fluxo padrão:
FILE *stdin;
FILE *stdout;
FILE *stderr;

FILE *_io_table[NUMBER_OF_FILES];

#define stdin  (_io_table[0])	
#define stdout 	(_io_table[1])
#define stderr 	(_io_table[2])


#endif
