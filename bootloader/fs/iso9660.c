#include <io.h>
#include <string.h>
#include <fs/fs.h>
#include "iso9660.h"


// Globais
cdfs_t *cdfs;
static cdfsdirectory_t *cdfsearch(char *filename1,char *filename2,cdfsdirectory_t *cdfsdirectory, char *dir);
static uint32_t cdfstrncmp(char *s1, const char *s2, size_t len);
static uint32_t pathcpy(char *dest,const char *src);
static char patheof(const char *src);

static uint32_t cdfstriipname(char *dest, const char *src, int end);

int read_iso9660(int devnum, const char *path, void *vfs,uint8_t flg)
{
	uint32_t offset = 0;
	uint32_t vfsoffset =0;
	uint8_t len = 0;
	int ret_val = 0;
	uint8_t pathID = 0;
	uint32_t pathoffset = 1;
	char *filename1 = (char*)malloc(256);
	char *filename2 =(char*)malloc(256);
	char *dir =(char*)malloc(0x1000);
	int i;
	char eof;


	cdfs = (cdfs_t*)malloc(0x1000);
	cdfsdirectory_t *cdfsdirectory = (cdfsdirectory_t*)malloc(sizeof(cdfsdirectory_t));


	if((read_sector(cdfs,16,1,devnum))!=0) {
		printf("ISO 9660 Read disk error\n");
		ret_val = -1;
		goto end;
	}

	strncpy((char*)cdfsdirectory,(char*)cdfs->data.specific.primary.RootDrectory,34);
	
	// Read Root Directory
	if((read_sector(dir,cdfsdirectory->LocOfExtLSB ,\
	cdfsdirectory->DataLenLBS/cdfs->data.specific.primary.LogBlockSizeLSB,devnum))!=0){
		printf("ISO 9660 Read disk error\n");
		ret_val = -1;
		goto end;

	}

	
	while(true){

		eof = patheof(path + pathoffset -1);
		if((!(eof)) || (eof == '/') ) {


			//Directorio raiz call vfs, jmp end

			(*(uint8_t*)(vfs + 1))  = 1; // flg = directory type
			(*(uint16_t*)(vfs + 2)) = cdfsdirectory->NumOfYears +1900;
			(*(uint8_t*)(vfs + 4))  = cdfsdirectory->Month;
			(*(uint8_t*)(vfs + 5))  = cdfsdirectory->Day;
			(*(uint8_t*)(vfs + 6))  = cdfsdirectory->Hour;
			(*(uint8_t*)(vfs + 7))  = cdfsdirectory->Minute;
			(*(uint8_t*)(vfs + 8))  = cdfsdirectory->Second;
			(*(uint8_t*)(vfs + 9))  = cdfsdirectory->GMT;
			(*(uint16_t*)(vfs + 51)) = cdfs->data.specific.primary.LogBlockSizeLSB;
			(*(uint16_t*)(vfs + 53))  = 0;
			(*(uint32_t*)(vfs + 55))  = 0; //Count
			(*(uint32_t*)(vfs + 59))  = 0;
	   		(*(uint8_t*)(vfs + 63))  = devnum;


			cdfstriipname((char*)vfs + 64,cdfsdirectory->FileID +cdfsdirectory->LenOfFileID,\
			cdfsdirectory->LenDirR - cdfsdirectory->LenOfFileID);

					
			// Read sub directory		
			if((read_sector(dir,cdfsdirectory->LocOfExtLSB ,\
			cdfsdirectory->DataLenLBS/cdfs->data.specific.primary.LogBlockSizeLSB,devnum))!=0) {
				printf("ISO 9660 Read disk error\n");
				ret_val = -1;
				goto end;

			}

			
			offset = 0;
			len = 0;
			while(true) {

				offset += len = (*(uint8_t*)(dir + offset));
	
				memcpy(cdfsdirectory,dir + offset,len);
				

				if(!(cdfsdirectory->LenDirR)) {
					break;
				}
				else if((cdfsdirectory->FileFlg&1) && (!(flg)))
					continue;
				else if(!(cdfsdirectory->LenOfFileID))
					continue;
				else if(!(cdfsdirectory->FileID[0])) // 0
					continue;

				else if((cdfsdirectory->FileID[0]) == 1) // '1'
					continue;


				len =cdfstriipname((char*)vfs + 256 + vfsoffset +1,cdfsdirectory->FileID +cdfsdirectory->LenOfFileID,\
				cdfsdirectory->LenDirR - cdfsdirectory->LenOfFileID);

				(*(uint8_t*)(vfs + 256 + vfsoffset)) = len +2;
				vfsoffset += len + 2;


				(*(uint32_t*)(vfs + 55)) += 1; // count++

				
			}

					//(*(uint8_t*)(vfs + 256 + vfsoffset + 3)) = 0x80; eof
					ret_val = 0;
					goto end;
	

		}



		pathoffset += pathID = pathcpy(filename1,path + pathoffset); 

		//strmacov(cdfstring,filename);
		cdfsdirectory = cdfsearch(filename1/*cdfstring */,filename2,cdfsdirectory,dir);


		if((cdfsdirectory->FileFlg&0x60)) {
			ret_val = -1;
			goto end;
		}


		else if((!(cdfsdirectory->FileFlg&2))) {


			// File Read call vfs jmp end
			if((patheof(path + pathoffset -1))) {
				//EOF
				ret_val = -1;
				goto end;
			}

		
			else if((cdfsdirectory->FileFlg&1) && (!flg)) {
				// File hidden not permition 
				ret_val = -1;
				goto end;


			}


			(*(uint8_t*)(vfs + 1))  = 2; // flg = file type + sem block
			(*(uint16_t*)(vfs + 2)) = cdfsdirectory->NumOfYears +1900;
			(*(uint8_t*)(vfs + 4))  = cdfsdirectory->Month;
			(*(uint8_t*)(vfs + 5))  = cdfsdirectory->Day;
			(*(uint8_t*)(vfs + 6))  = cdfsdirectory->Hour;
			(*(uint8_t*)(vfs + 7))  = cdfsdirectory->Minute;
			(*(uint8_t*)(vfs + 8))  = cdfsdirectory->Second;
			(*(uint8_t*)(vfs + 9))  = cdfsdirectory->GMT;
			(*(uint16_t*)(vfs + 51)) = cdfs->data.specific.primary.LogBlockSizeLSB;
			(*(uint16_t*)(vfs + 53))  = 0; // sem block
			(*(uint32_t*)(vfs + 55))  = 1; //Count
			(*(uint32_t*)(vfs + 59))  = cdfsdirectory->DataLenLBS;
	   		(*(uint8_t*)(vfs + 63))  = devnum;

			cdfstriipname((char*)vfs + 64,cdfsdirectory->FileID +cdfsdirectory->LenOfFileID,\
			cdfsdirectory->LenDirR - cdfsdirectory->LenOfFileID);

			(*(uint32_t*)(vfs + 256)) = cdfsdirectory-> LocOfExtLSB;
			

			ret_val = 0;
			goto end;
		}
		else if((!(cdfsdirectory->FileFlg&1)) && (cdfsdirectory->FileFlg&2) && (flg == 0)) {
			// Read directory
			if((read_sector(dir,cdfsdirectory->LocOfExtLSB ,\
			cdfsdirectory->DataLenLBS/cdfs->data.specific.primary.LogBlockSizeLSB,devnum))!=0) {
				printf("ISO 9660 Read disk error\n");
				ret_val = -1;
				goto end;
			}
			continue;
	
		}
		else if((cdfsdirectory->FileFlg&1) && (cdfsdirectory->FileFlg&2) && (flg == 1)) {
			// Read hidden directory
			if((read_sector(dir,cdfsdirectory->LocOfExtLSB ,\
			cdfsdirectory->DataLenLBS/cdfs->data.specific.primary.LogBlockSizeLSB,devnum))!=0) {
				printf("ISO 9660 Read disk error\n");
				ret_val = -1;
				goto end;
			}
			continue;
	
		}else {

				ret_val = -1;
				goto end;

		}



	}//end wihle


end:
	free(cdfsdirectory);
	free(cdfs);
	free(dir);
	free(filename2);
	free(filename1);
	return (ret_val);

}



static cdfsdirectory_t *cdfsearch(char *filename1,char *filename2,cdfsdirectory_t *cdfsdirectory, char *dir)
{
	uint32_t offset = 0;
	uint8_t len = 0;

	while(true) {

		offset += len = (*(uint8_t*)(dir + offset));
		memcpy(cdfsdirectory,dir + offset,len);

		cdfstriipname(filename2,cdfsdirectory->FileID +cdfsdirectory->LenOfFileID,\
		cdfsdirectory->LenDirR - cdfsdirectory->LenOfFileID);

		if(!(cdfsdirectory->LenDirR)) {
			printf("ISO 9660 File not found\n");
			cdfsdirectory->FileFlg |= 0x60;			
			break;

		}

		// SUBDIRECTOTY
		else if((cdfsdirectory->FileFlg &2)) {

			if(!(strcmp(filename1,filename2/*cdfsdirectory->FileID*/))) {
				cdfsdirectory->FileFlg &= ~0x60;	
				break;
		
			}
			continue; 


		}

		// FILE
		else if(!(cdfsdirectory->FileFlg &2)) { 
	
			if(!(strcmp(filename1,filename2/*cdfstrncmp(filename,cdfsdirectory->FileID,cdfsdirectory->LenOfFileID*/))) {
				cdfsdirectory->FileFlg &= ~0x60;	
				break;
			}
			continue;


		}


	}

	return (cdfsdirectory);

}


static uint32_t cdfstrncmp(char *s1, const char *s2, size_t len)
{
	int i;
	if(!(len))return -1;

	for(i = 0; i < len; i++) {
	
		if(s1[i] == s2[i]);
		else if(s2[i] == ';');
		else if(s2[i] == '1');
		else return -1;		


	}

	return 0;
}


static uint32_t pathcpy(char *dest,const char *src)
{
    int i =0;
    if(!(src[i]))return 0;

    while ((src[i] != '/') && (src[i])){
        dest[i] = src[i];
        i++;
    }
    dest[i] ='\0';
    return (i+1);
            
}
static char patheof(const char *src)
{

	if( (!(src[1])) && (src[0] == '/') )return '/';
	else if(!(src[0]))return 0;
	return 0xff;
}


// This function copy name RRIP
static uint32_t cdfstriipname(char *dest, const char *src, int end)
{
	uint32_t count = 0;
	int i;

	for(i=0;i < 5 ;i++) {
		if((src[i] != 'R') && (src[i+1] != 'R')) {
			return 0;
		} 

		if((src[i] == 'R') && (src[i+1] == 'R')) {

			break;
		}
	}

	for (;;i++) {

		if((src[i] == 'N') && (src[i+1] == 'M')) {
			i +=5;			
			break;
		}
		
		if(i == end)return 0;
	}
	
   	while (((*(uint32_t*)(src +count +i))&0xffffff) &~0x245850/*(src[count +i] != 'P') && (src[count + i +1] != 'X') && (src[count + i +2] != '$')*/) {
        	dest[count] = src[count +i];
        	count++;

		if(count == end)break;

    	}
    	dest[count] ='\0';
	return (count);
} 
