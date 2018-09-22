/*
 * Khole Operating System
 * ata/ata.c
 * 
 * Copyright (C) 2017,2018 Nelson Cole <nelsoncole72@gmail.com>
 */

#include "ata.h"
#include "ide.h"
#include "ahci.h"


disk_t *disk,*diskq;
uint32_t ata_irq_invoked1 = 1;
uint32_t ata_irq_invoked2 = 1; 

void ata_irq_handler1(void)
{

    ata_irq_invoked1 = 1;
   
}

void ata_irq_handler2(void)
{

    ata_irq_invoked2 = 1;
    
}

void ata_wait(int val)
{ 
   val/=100;
   while(val--)io_delay();
}


int ata_wait_not_busy(int p)
{
    while(ata_status_read(p) &ATA_SR_BSY)
    if(ata_status_read(p) &ATA_SR_ERR)
    return 1;

    return 0;

}

int ata_wait_busy(int p)
{
    while(!(ata_status_read(p) &ATA_SR_BSY))
    if(ata_status_read(p) &ATA_SR_ERR)
    return 1;

    return 0;

}


int ata_wait_no_drq(int p)
{
    while(ata_status_read(p) &ATA_SR_DRQ)
    if(ata_status_read(p) &ATA_SR_ERR)
    return 1;

    return 0;
}

int ata_wait_drq(int p)
{
    
    while(!(ata_status_read(p) &ATA_SR_DRQ))
    if(ata_status_read(p) &ATA_SR_ERR)
    return 1;

    return 0;
}

void ata_soft_reset(int p)
{
    uint8_t data =  inb(ata[p].ctrl_block_base_addr);
    outb(ata[p].ctrl_block_base_addr, data | 0x4);
    outb(ata[p].ctrl_block_base_addr, data &0xfb);    
}


uint8_t ata_status_read(int p)
{

   	return inb(ata[p].cmd_block_base_addr + ATA_REG_STATUS);
    

}

void ata_cmd_write(int p,int command)
{
           
    	// no_busy      	
	ata_wait_not_busy(p);
	outb(ata[p].cmd_block_base_addr + ATA_REG_CMD,command);
	ata_wait(400);  // Esperamos 400ns

}


int ata_initialize()
{

    uint32_t data = pci_scan_bcc(PCI_CALSS_MASS_STORAGE);

    if(data  == -1) {
    	puts("ATA/ATAPI Initialize: PCI PANIC\n");
    	puts("ATA/ATAPI Controller not found!\n");
    return (PCI_MSG_ERROR); // Abortar
    }



    data = ata_pci_configuration_space(data  >>  8 &0xff,data  >> 3  &31,data &7);

    if(data == PCI_MSG_ERROR) {
        printf("Error Driver [%X]\n",data);
        return -1;    }
    else if(data == PCI_MSG_AVALIABLE) {
        
         printf("Not suport, RAID Controller\n");
         return -1;       
    }


    //disk =(disk_t*)malloc(sizeof(disk_t));


    // Initialize base address
    // AHCI/IDE Compativel com portas IO IDE legado
    ATA_BAR0 = (ata_pci.bar0 & ~7) + ATA_IDE_BAR0 * ( !ata_pci.bar0);
    ATA_BAR1 = (ata_pci.bar1 & ~3) + ATA_IDE_BAR1 * ( !ata_pci.bar1);       
    ATA_BAR2 = (ata_pci.bar2 & ~7) + ATA_IDE_BAR2 * ( !ata_pci.bar2);
    ATA_BAR3 = (ata_pci.bar3 & ~3) + ATA_IDE_BAR3 * ( !ata_pci.bar3);
    ATA_BAR4 = (ata_pci.bar4 & ~0x7) + ATA_IDE_BAR4 * ( !ata_pci.bar4);
    ATA_BAR5 = (ata_pci.bar5 & ~0xf) + ATA_IDE_BAR5 * ( !ata_pci.bar5);

    if(ata_pci.adapter == ATA_IDE_CONTROLLER){

    ide_init();


    }else if(ata_pci.adapter == ATA_AHCI_CONTROLLER){



    int abar_size = 2;

    // Aqui, vamos mapear o BAR5
    // Estou colocando na marca 28MB
    ahci_init((uint32_t)mem_map((void*)(ATA_BAR5),0x13,abar_size));
    
    

    }else {

        printf("Controller IDE or AHCI, not found\n");
        die();
    
    }
                                                                                                 
  	return 0;
}


// TODO
// Nelson aqui, devemos destinguir se a Interface é IDE ou AHCI
// Por em quanto temos suporte a IDE
int read_sector(void *buf,uint64_t addr,uint16_t count,int dev)
{


    int i;
    int data;

    switch(ata_pci.adapter)
    {
       case ATA_IDE_CONTROLLER: 
       // PATA
       // 8 Sectors em modo DMA (4KB)
       if(ata[dev].dev_type == ATA_DEVICE_TYPE){

            switch(ata[dev].mts)
            {

            case ATA_DMA_MODO:
	        for(i=0;i < count;i+=8){

                data = (count <= i )? 8: (count- i);
                    if((ide_read_sector(dev,data/*count*/,addr + i ,buf + (ata[dev].bps*i))) !=0){
                        return -1;
                    }
	            }
                break;

            case ATA_PIO_MODO:
                for(i=0;i < count;i++){
                    if((ide_read_sector(dev,1/*count*/,addr + i ,buf + (ata[dev].bps*i))) !=0){
                        return -1;
          
                    }
	            }
                break;

            }

        }else if(ata[dev].dev_type == ATAPI_DEVICE_TYPE){

            // PATAPI
            // 2 Sectors em modo DMA (4KB)
            switch(ata[dev].mts)
            {

            case ATA_DMA_MODO:
	        for(i=0;i < count;i+=2){

                data = (count <= i )? 2: (count- i);
                    if((atapi_read_sector(dev,data/*count*/,addr + i,buf+ (ata[dev].bps*i)))!=0){
                        return -1;
                    }

	            }
                break;

            case ATA_PIO_MODO:
		

                for(i=0;i < count;i++){
                    if((atapi_read_sector(dev,1/*count*/,addr + i,buf+ (ata[dev].bps*i)))!=0){
                        return -1;
                    }
	            }
                break;

            }
	    
        }
            break; //end ATA/ATAPI

        case ATA_AHCI_CONTROLLER:
        // SATA/SATAPI
        // 8 Sectors (4KB SATA or 16KB SATAPI)
          for(i=0;i < count; i+=8)
          {
                data = (count <= i )? 8: (count- i);
                if((sata_read_sector(dev,addr+i,data,buf + (sata[dev].bps*i)))!=0)
                {
                    return -1;                
                }


           }

            break; //end SATA/SATAPI
    }

    return 0;

}

int write_sector(void *buf,uint64_t addr,uint16_t count,int dev)
{
    int i;

    if(ata[dev].dev_type == ATA_DEVICE_TYPE){
    for(i=0;i < count;i++){
        if((ide_write_sector(dev,1,addr + i ,buf + (ata[dev].bps*i))) !=0)
        return -1;
    }    

    }
    else if(ata[dev].dev_type == ATAPI_DEVICE_TYPE){
        printf("Acesso negado, apenas leitura devd%\n",dev);
        return -1;
    }

    return 0;

}
