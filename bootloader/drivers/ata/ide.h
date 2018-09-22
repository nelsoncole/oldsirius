/*
 * Khole Operating System
 * ata/ide.h
 * 
 * Copyright (C) 2017,2018 Nelson Cole <nelsoncole72@gmail.com>
 */


#ifndef __IDE_H__
#define __IDE_H__

void ide_init();
int ide_identify_device(int p);
int ide_read_sector(int p,uint16_t count,uint64_t addr,void *buffer);
int ide_write_sector(int p,uint16_t count,uint64_t addr,void *buffer);
int atapi_read_sector(int p,uint16_t count,uint64_t addr,void *buffer);



#endif
