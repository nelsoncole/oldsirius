#include <io.h>
#include <string.h>
#include <memory.h>

// esta macro conver seg:offset para flat
#define __ptr_cov32(p)\
( (16 * (p >>16 &0xFFFF)) + (p &0xFFFF) )

uint32_t used_page_directory = 0;
uint32_t used_page_tables = 0;
extern uint8_t bootdevice_num;
void mboot_info(uint32_t arg)
{

	char *mboot = (char*)arg;
	char *tmp = (char*)0;
	uint32_t data = 0;
	int i;


	// Base Address Register
	(*(uint32_t*)(mboot + 0 )) = 0x454C4F43; 	// Magic
	(*(uint32_t*)(mboot + 4 )) = 0x101000; 		// Boot Device
	(*(uint32_t*)(mboot + 8 )) = 0x101100; 		// PCI Device
	(*(uint32_t*)(mboot + 12)) = 0x102000; 		// BIOS VESA (video end audio)
	(*(uint32_t*)(mboot + 16)) = 0x103000; 		// Physical Memory Map
	(*(uint32_t*)(mboot + 20)) = 0x200000; 		// Page directory
	(*(uint32_t*)(mboot + 24)) = 0x201000; 		// Page tables
	// Virtual Memory
	(*(uint32_t*)(mboot + 4084)) = 256 *1024; 	// Total Page Table
	(*(uint32_t*)(mboot + 4088)) = used_page_directory;
	(*(uint32_t*)(mboot + 4092)) = used_page_tables;

	// Boot Device Info
	// BIOSes Get Device Parameters (INT 13h FN 48h)
	char *bootDevice = (char*)0x40000;
	(*(uint32_t*)(mboot + 0x1000)) = bootdevice_num &0xff;
	(*(uint32_t*)(mboot + 0x1004)) = (*(uint32_t*)(bootDevice +36)); // Host bus typ
	(*(uint32_t*)(mboot + 0x1008)) = (*(uint32_t*)(bootDevice +40)); // Interface type low
	(*(uint32_t*)(mboot + 0x100C)) = (*(uint32_t*)(bootDevice +44)); // Interface type Higth
	// Device Parameter Table Extension (DPTE)
	// Pointer to the Device Parameter Table Extension (DPTE), flat 32-bits.
	tmp =(char*) (16 * ((*(uint32_t*)(bootDevice+26)) >>16 &0xFFFF) + ((*(uint32_t*)(bootDevice+26)) &0xFFFF));
	(*(uint32_t*)(mboot + 0x1010)) = (uint32_t*)tmp;
	// I/O port end Control base address
	// // Align  word 
	(*(uint32_t*)(mboot + 0x1014)) = (*(uint32_t*)(tmp + 0));
	// Device register, BIOS Vendor Specific, IRQ information end Block count for ATA READ/WRITE MULTIPLE commands
	// Align byte
	(*(uint32_t*)(mboot + 0x1018)) = (*(uint32_t*)(tmp + 4)); 
	// Optional Boot Device Info
	// Optional Controller Interface type IDE or AHCI
	(*(uint32_t*)(mboot + 0x101C)) =0;
	// Optional Device type (0 = ATA, 1 = ATAPI)
	// Optional Channel Number
	// Optional device Number (0 = master, 1 = slave)
	// Optional SATA Number or Port Number
	// Align byte
	data = (((bootdevice_num &0xE0) == 0x80 )? 0 : 1);
	if ((*(uint16_t*)(tmp + 0)) == 0x01F0 ) data += 0 <<8;
	else if ((*(uint16_t*)(tmp + 0)) == 0x0170 ) data += 1 <<8;
	data += (((*(uint8_t*)(tmp + 4)) &0x10)? 1 : 0) <<16;
	uint8_t satan = 0;
	switch (mboot_ata_interface_type) {
		case 0: // IDE
			data += ( ((data >> 8 &0xff)*2) + (data >> 16 &0xff)) << 24; // port number
			(*(uint32_t*)(mboot + 0x101C)) = 0x20454449; // Signature "IDE " 
		break;
		case 1:	// ATCI (SATA)
			if(!(*(uint8_t*)(tmp + 1))) satan = ((*(uint8_t*)(tmp + 3)) + (data >>16 &0xff));
			else if((*(uint8_t*)(tmp + 1)) == 3) satan = (*(uint8_t*)(tmp + 6));
			else if((*(uint8_t*)(tmp + 1)) == 4) satan = (*(uint8_t*)(tmp + 0));

			data += satan << 24; // Sata number
			(*(uint32_t*)(mboot + 0x101C)) = 0x41554153; // Signature "SATA"
			break;
		default:
			printf("Disk controller interface type, not found!\n ");
		break;

	}

	(*(uint32_t*)(mboot + 0x1020)) = data;


	// BIOS VESA Video and Audio
	char *vesa = (char*)0x40200;

	memcpy(mboot + 0x2000,vesa,1024);



	// Physical Memoey Map (INT 0x15 AX,E820h)
	// 8 bytes RAM SIZE 
	(*(uint32_t*)(mboot + 0x3000 )) = (1024*64*1024); //64MB

	for (i = 0; i < (used_page_directory*32); i++)
	(*(uint32_t*)(mboot + 0x3000 + (i*4) +8)) = 0xFFFFFFFF;




}


// Usado em partition init
uint32_t mboot_device () 
{
	uint32_t data = 0;
	// Boot Device Info
	// BIOSes Get Device Parameters (INT 13h FN 48h)
	char *bootDevice = (char*)0x40000;
	// Device Parameter Table Extension (DPTE)
	// Pointer to the Device Parameter Table Extension (DPTE), flat 32-bits.
	char *tmp =(char*) (16 * ((*(uint32_t*)(bootDevice+26)) >>16 &0xFFFF) + ((*(uint32_t*)(bootDevice+26)) &0xFFFF));
	// Optional Device type (0 = ATA, 1 = ATAPI)
	// Optional Channel Number
	// Optional device Number (0 = master, 1 = slave)
	// Optional SATA Number or Port Number
	// Align byte
	data = (((bootdevice_num &0xE0) == 0x80 )? 0 : 1);
	if ((*(uint16_t*)(tmp + 0)) == 0x01F0 ) data += 0 << 8;
	else if ((*(uint16_t*)(tmp + 0)) == 0x0170 ) data += 1 <<8;
	data += (((*(uint8_t*)(tmp + 4)) &0x10)? 1 : 0) <<16;
	uint8_t satan = 0;
	switch (mboot_ata_interface_type) {
		case 0: // IDE
			data += ( ((data >> 8 &0xff)*2) + (data >> 16 &0xff)) << 24; // port number 
		break;
		case 1:	// ATCI (SATA)
			if(!(*(uint8_t*)(tmp + 1))) satan = ((*(uint8_t*)(tmp + 3)) + (data >>16 &0xff));
			else if((*(uint8_t*)(tmp + 1)) == 3) satan = (*(uint8_t*)(tmp + 6));
			else if((*(uint8_t*)(tmp + 1)) == 4) satan = (*(uint8_t*)(tmp + 0));

			data += satan << 24; // Sata number
			break;
		default:
			printf("Disk controller interface type, not found!\n ");
		break;

	}


	return data;
}

