#ifndef __DEV_H__
#define __DEV_H__
#include <typedef.h>

uint8_t mboot_ata_interface_type;
// ATA
int   ata_initialize();
void  ata_irq_handler1(void);
void  ata_irq_handler2(void);

int read_sector(void *buf,uint64_t addr,uint16_t count,int dev);
int write_sector(void *buf,uint64_t addr,uint16_t count,int dev);


#endif
