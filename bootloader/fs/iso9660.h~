#ifndef __ISO9660_H__
#define __ISO9660_H__
#include <typedef.h>
#define CDFS_VDSPT_TYPE_BOOTRECORD 	0   // Boot Record
#define CDFS_VDSPT_TYPE_PRIMARY		1   // Primary Volume Descriptor
#define CDFS_VDSPT_TYPE_SUPPLEMENT	2   // Supplementary Volume Descriptor
#define CDFS_VDSPT_TYPE_PARTITION	3   // Volume Partition Descriptor
#define CDFS_VDSPT_TYPE_TERMINATOR	255 // Volume Descriptor Set Terminator.
#define CDFS_IDENTIFIER "CD001"

typedef struct cdfsdata {
union {
	
	struct {

		char SysId[32];
		char Id[32];
		char BooSysUse[1977];

	}__attribute__ ((packed)) boot;

	struct {
		char Unused0;
		char SysID[32];
		char ID[32];
		char Unused1[8];
		uint32_t SpaceSizeLSB;
		uint32_t SpaceSizeMSB;
		char Unused2[32];
		uint16_t SetSizeLSB;
		uint16_t SetSizeMSB;
		uint16_t SeqNumLSB;
		uint16_t SeqNumMSB;
		uint16_t LogBlockSizeLSB;  // Logical Block Size
		uint16_t LogBlockSizeMSB;  
		uint32_t PTableSizeLSB;  // Path Table Size
		uint32_t PTableSizeMSB;
		uint32_t LocOfLPTable; // Location of Occurrence of Type L Path Table
		uint32_t LocOfOLPTable;// Location of Optional Occurrence of Type L Path Table
		uint32_t LocOfMPTable; // Location of Occurrence of Type M Path Table
		uint32_t LocOfOMPTable;// Location of Optional Occurrence of Type M Path Table
		char RootDrectory[34]; // Directory Record for Root Directory (34 Bytes)
		char SetID[128];
		char PubID[128];
		char dataPreID[128];
		char AppID[128];
		char CopyrightFileID[38];
		char AbstractFileID[36];
		char BibliographicFileID[37];
		char CreationDateTime[17]; //TODO
		char ModificationDateTime[17]; //TODO
		char ExpirationDateTime[17]; //TODO
		char EffectiveDateTime[17]; //TODO
		char FileStructVersion;
		char Unused3;
		char AppUsed[512];
		char rsv[653];
	}__attribute__ ((packed)) primary;
	
	
}__attribute__ ((packed)) specific;	
}__attribute__ ((packed)) cdfsdata_t;
typedef struct cdfs{
	char Type;
	char Identifier[5];
	char Version; 
	cdfsdata_t data;

}__attribute__ ((packed)) cdfs_t;
typedef struct cdfspathtable {	
	char LenDirID;
	char ExtAttRLen;
	uint32_t LocOfExt; 	// Location of Extent (LBA)
	uint16_t ParDirNum;	// Parent Directory Number (65536)
	char DirID;		// Directory Identifier  

}__attribute__ ((packed)) cdfspathtable_t;

typedef struct cdfsdirectory {
	uint8_t LenDirR;
	uint8_t ExtAttRLen;
	// This field shall specify as a 32-bit number the Logical Block Number 
	// of the first Logical Block allocated to the Extent. (LBA)
	uint32_t LocOfExtLSB;
	uint32_t LocOfExtMSB;
	// Data length (size of extent) in both-endian format.
	uint32_t DataLenLBS; 
	uint32_t DataLenMSB;
	uint8_t  NumOfYears;	// Number of years since 1900 
	uint8_t  Month;  	// Month of the year from 1 to 12 
	uint8_t  Day;		// Day of the month from 1 to 31 
	uint8_t  Hour;		// Hour of the day from 0 to 23 
	uint8_t  Minute; 	// Minute of the hour from 0 to 59 
	uint8_t  Second; 	// Second of the minute from 0 to 59 
	// Offset from Greenwich Mean Time (GMT) in number of 15 
	// min intervals from -48 (West) to + 52 (East) recorded according to 7.1.2 
	uint8_t GMT;
	uint8_t FileFlg;
	uint8_t FileUnitSize;
	uint8_t InterleaveGapSize;
	uint16_t VSeqNumLBS;// 16 bit both-endian format. 
	uint16_t VSeqNumMBS;
	uint8_t LenOfFileID;
	char FileID[256-33];


}__attribute__ ((packed)) cdfsdirectory_t;




#endif



