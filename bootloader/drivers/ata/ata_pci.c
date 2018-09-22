/* *
 * Copyright (C) 2017-2018 (Nelson Sapalo da Silva Cole)
 * Khole OS v0.3
 *
 *
 */
#include "ata.h"

static const char *ata_sub_class_code_register_strings[] ={
    "Unknown",
    "IDE Controller",
    "Unknown",
    "Unknown",
    "RAID Controller",
    "Unknown",
    "AHCI Controller"
};

// Espaço de configuraçao PCI Mass Storage
int ata_pci_configuration_space(char bus,char dev,char fun)
{
    uint32_t data;
  
    // Indentification Device
    data  = read_pci_config_addr(bus,dev,fun,0);
    
    ata_pci.vendor_id = data &0xffff;
    ata_pci.device_id = data >> 16 &0xffff;
    
    // Classe code, programming interface, revision id
    data  = read_pci_config_addr(bus,dev,fun,8);
    
    ata_pci.classe      = data  >> 24 &0xff;
    ata_pci.subclasse   = data  >> 16 &0xff;
    ata_pci.revision_id = data &0xff;

    if(ata_pci.classe == 1 && ata_pci.subclasse == 1){
        ata_pci.adapter = ATA_IDE_CONTROLLER; 
                   
        // IDE
	
        // Compatibilidade e nativo, primary
        data  = read_pci_config_addr(bus,dev,fun,8);
        if(data &0x200) write_pci_config_addr(bus,dev,fun,8,data | 0x100);
        else write_pci_config_addr(bus,dev,fun,8,data &~ 0x100);         

        // Compatibilidade e nativo, secundary
        data  = read_pci_config_addr(bus,dev,fun,8);
        if(data &0x800) write_pci_config_addr(bus,dev,fun,8,data | 0x400);        
        else write_pci_config_addr(bus,dev,fun,8,data &~ 0x400);        



        data  = read_pci_config_addr(bus,dev,fun,8);
        ata_pci.prog_if     = data >> 8 &0xff;



        data  = read_pci_config_addr(bus,dev,fun,8);
        if(data &0x8000) {    
        // Bus Master Enable
        data  = read_pci_config_addr(bus,dev,fun,4);
        write_pci_config_addr(bus,dev,fun,4,data | 0x4);

        } 


	    // Habilitar interrupcao (INTx#)
        data  = read_pci_config_addr(bus,dev,fun,4);
        write_pci_config_addr(bus,dev,fun,4,data & ~0x400);



       	// IDE Decode Enable
       	data  = read_pci_config_addr(bus,dev,fun,0x40);
       	write_pci_config_addr(bus,dev,fun,0x40,data | 0x80008000);



        //Synchronous DMA Control Register
	    // Enable UDMA
	    data = read_pci_config_addr(bus,dev,fun,0x48);
	    write_pci_config_addr(bus,dev,fun,0x48,data | 0xf);
/*
        printf("IDE%d\n",ata_pci.revision_id);
        printf("Vendor ID: %X,Device ID: %X\n",ata_pci.vendor_id,ata_pci.device_id);
        printf("Sub Class %s Programming Interface %d Revision ID %d\n",\
        ata_sub_class_code_register_strings[ata_pci.adapter],ata_pci.prog_if,ata_pci.revision_id); */
            
    }
    else if(ata_pci.classe == 1 && ata_pci.subclasse == 4){
        ata_pci.adapter = ATA_RAID_CONTROLLER;
        //RAID
        printf("RAID%d.0\n",ata_pci.revision_id);
        printf("Vendor ID: %X,Device ID: %X\n",ata_pci.vendor_id,ata_pci.device_id);
        printf("Sub Class %s Programming Interface %d Revision ID %d\n",\
        ata_sub_class_code_register_strings[ata_pci.adapter],ata_pci.prog_if,ata_pci.revision_id);
    
        // Em avaliacao

         return PCI_MSG_AVALIABLE;
    }
    else if(ata_pci.classe == 1 && ata_pci.subclasse == 6){
        ata_pci.adapter = ATA_AHCI_CONTROLLER;
       
        //ACHI
        // Compatibilidade e nativo, primary
        data  = read_pci_config_addr(bus,dev,fun,8);
        if(data &0x200) write_pci_config_addr(bus,dev,fun,8,data | 0x100);
        else write_pci_config_addr(bus,dev,fun,8,data &~ 0x100);         

        // Compatibilidade e nativo, secundary
        data  = read_pci_config_addr(bus,dev,fun,8);
        if(data &0x800) write_pci_config_addr(bus,dev,fun,8,data | 0x400);        
        else write_pci_config_addr(bus,dev,fun,8,data &~ 0x400); 

        data  = read_pci_config_addr(bus,dev,fun,8);
        ata_pci.prog_if     = data >> 8 &0xff;



        data  = read_pci_config_addr(bus,dev,fun,8);
        if(data &0x8000) {    
        // Bus Master Enable
        data  = read_pci_config_addr(bus,dev,fun,4);
        write_pci_config_addr(bus,dev,fun,4,data | 0x4);

        } 


        // IDE Decode Enable
        data  = read_pci_config_addr(bus,dev,fun,0x40);
        write_pci_config_addr(bus,dev,fun,0x40,data | 0x80008000);



        
        // Habilitar interrupcao (INTx#)
        data  = read_pci_config_addr(bus,dev,fun,4);
        write_pci_config_addr(bus,dev,fun,4,data & ~0x400);


        // IRQ
        data  = read_pci_config_addr(bus,dev,fun,0x3C);
        //write_pci_config_addr(bus,dev,fun,0x3C,data & 0xEF);



        /* MAP—Address Map Register (SATA)
        7:6
        00b = IDE mode
        01b = AHCI mode
        10b = RAID mode
        11b = Reserved */

        // AHCI mode
        data  = read_pci_config_addr(bus,dev,fun,0x90);
        data &= ~0x80;
        data |=  0x40; 
        write_pci_config_addr(bus,dev,fun,0x90,data);








/*
        printf("AHCI%d.0\n",ata_pci.revision_id);
        printf("Vendor ID: %X,Device ID: %X\n",ata_pci.vendor_id,ata_pci.device_id);
        printf("Sub Class %s Programming Interface %d Revision ID %d\n",\
        ata_sub_class_code_register_strings[ata_pci.adapter],ata_pci.prog_if,ata_pci.revision_id);

        //SATACR0—SATA Capability Register 0
        data  = read_pci_config_addr(bus,dev,fun,0xA8);
        printf("SATA Capability Register 0: %X\n",data);

        // SATACR1—SATA Capability Register 1
        data  = read_pci_config_addr(bus,dev,fun,0xAC);
        printf("SATA Capability Register 1: %X\n",data); */
        
    }
    else {
       	//PANIC 
        printf("ATA initialize: PCI PANIC\n");
        printf("IDE Controller not found!\n");
        printf("RAID Controller not found!\n");
        printf("AHCI Controller not found!\n");
        while(TRUE)hlt();
    }



    // PCI cacheline, Latancy, Headr type, end BIST
    data  = read_pci_config_addr(bus,dev,fun,0xC);

    ata_pci.primary_master_latency_timer = data >>8 &0xff;
    ata_pci.header_type     = data >>16 &0xff;
    ata_pci.BIST            = data >>24 &0xff;
    
    ata_pci.bar0    = read_pci_config_addr(bus,dev,fun,0x10);
    ata_pci.bar1    = read_pci_config_addr(bus,dev,fun,0x14);
    ata_pci.bar2    = read_pci_config_addr(bus,dev,fun,0x18);
    ata_pci.bar3    = read_pci_config_addr(bus,dev,fun,0x1C);
    ata_pci.bar4    = read_pci_config_addr(bus,dev,fun,0x20);
    ata_pci.bar5    = read_pci_config_addr(bus,dev,fun,0x24);



//-------------------------------------------------------------------

    
    // Interrupt
    data  = read_pci_config_addr(bus,dev,fun,0x3C);
    ata_pci.interrupt_line  = data &0xff;
    ata_pci.interrupt_pin   = data >> 8 &0xff;



    // PCI command and status
    data  = read_pci_config_addr(bus,dev,fun,4);
    ata_pci.command = data &0xffff; 
    ata_pci.status  = data >>16 &0xffff;
 /*   printf("Command %x Status %X\n",ata_pci.command,ata_pci.status);

    printf("Interrupt Line %d Interrupt Pin %d\n",ata_pci.interrupt_line,ata_pci.interrupt_pin);

    data = read_pci_config_addr(bus,dev,fun,0x48);
    printf("Synchronous DMA Control Register %X\n",data); */


    return (PCI_MSG_SUCCESSFUL);

}
