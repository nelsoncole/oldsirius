/*
 * Khole Operating System
 * ata/ide.c
 * 
 * Copyright (C) 2017,2018 Nelson Cole <nelsoncole72@gmail.com>
 */
#include "ata.h"
#include "ide.h"

uint8_t *dma_addr;

const char *dev_type[]={
"ATA",
"ATAPI"
};

extern uint32_t ata_irq_invoked1;
extern uint32_t ata_irq_invoked2;
static int ide_dev_init(int p);

int ata_wait_irq(int irq)
{
   uint32_t tmp = 100000; // Tempo de validação de espera de IRQ tmp *100ns
   uint8_t  data;

   if(irq == IDE_IRQ14){

   while (!ata_irq_invoked1){
    data = ata_status_read(0);
    if((data &ATA_SR_ERR)){
        ata_irq_invoked1 = 0;
        return -1;
        }
                         //ns
        if(--tmp)ata_wait(100);
        else{
        ata_irq_invoked1 = 0;
        return 0x80;
        }
    }
 
    ata_irq_invoked1 = 0;
    return 0;

   }else if(irq == IDE_IRQ15){

   while (!ata_irq_invoked2){
    data = ata_status_read(2);
    if((data &ATA_SR_ERR)){
        ata_irq_invoked2 = 0;
        return -1;
        }
                         //ns
        if(--tmp)ata_wait(100);
        else{
        ata_irq_invoked2 = 0;
        return 0x80;
        }
    }
 
    ata_irq_invoked2 = 0;
    return 0;

    }else{
    ata_irq_invoked1 = 0;
    ata_irq_invoked2 = 0;
    }

	return 0;
}


static void ide_bus_init(int p,int irq,int dev_num,int channel,\
int cmd_block_base_addr,int ctrl_block_base_addr,int bus_master_base_addr)
{

    ata[p].irq = irq;
    ata[p].dev_num = dev_num;
    ata[p].channel = channel;
    ata[p].cmd_block_base_addr = cmd_block_base_addr;
    ata[p].ctrl_block_base_addr = ctrl_block_base_addr;
    ata[p].bus_master_base_addr = bus_master_base_addr;


}

void ide_init()
{



    // Inicilizamos nossas portas IDE
    ide_bus_init(0,IDE_IRQ14,0,ATA_PRIMARY,ATA_BAR0,ATA_BAR1,ATA_BAR4 + 0);
    ide_bus_init(1,IDE_IRQ14,1,ATA_PRIMARY,ATA_BAR0,ATA_BAR1,ATA_BAR4 + 0);
    ide_bus_init(2,IDE_IRQ15,0,ATA_SECONDARY,ATA_BAR2,ATA_BAR3,ATA_BAR4 + 8);
    ide_bus_init(3,IDE_IRQ15,1,ATA_SECONDARY,ATA_BAR2,ATA_BAR3,ATA_BAR4 + 8);


    //Soft Reset, defina IRQ
    outb(ATA_BAR1,0xff);
    outb(ATA_BAR3,0xff);
    outb(ATA_BAR1,0x00);
    outb(ATA_BAR3,0x00);


    ata_record_dev = 0xff;
    ata_record_channel = 0xff;

    cli();
    irq_enable(14);
    irq_enable(15);
    sti();





    ata_identify_dev_buf =(uint16_t*)malloc(4096);

    int port;
    for(port =0;port < 4;port++)
    ide_dev_init(port);
    
 
    
}

static int ide_dev_init(int p)
{

    uint8_t data = ide_identify_device(p);
    if(data == -1)
    return 1;

    if(data == 0){

        // unidades ATA

        ata[p].dev_type   = (ata_identify_dev_buf[0]&0x8000)? 0xffff:ATA_DEVICE_TYPE;
        ata[p].lba = (ata_identify_dev_buf[83]&0x0400)? ATA_LBA48:ATA_LBA28;
        ata[p].mts = (ata_identify_dev_buf[49]&0x0100)? ATA_DMA_MODO:ATA_PIO_MODO;
       // new_dev->dev_total_num_sector  = ata_identify_dev_buf[60];
       // new_dev->dev_total_num_sector += ata_identify_dev_buf[61];
        ata[p].bps = 512; 
        //new_dev->dev_total_num_sector_lba48  = ata_identify_dev_buf[100];
        //new_dev->dev_total_num_sector_lba48 += ata_identify_dev_buf[101];
        //new_dev->dev_total_num_sector_lba48 += ata_identify_dev_buf[102];
        //new_dev->dev_total_num_sector_lba48 += ata_identify_dev_buf[103];
        
        
    }else if(data == 0x80){

        // Unidades ATAPI

        ata[p].dev_type   =(ata_identify_dev_buf[0]&0x8000)? ATAPI_DEVICE_TYPE : 0xffff;
        ata[p].lba = ATA_LBA28;
        ata[p].mts = (ata_identify_dev_buf[49]&0x0100)? ATA_DMA_MODO:ATA_PIO_MODO;
        ata[p].bps = 2048; 
    
    }else{
        printf("Port%d not found!\n",p);
        //Identificar o erro
        return (1);
    }


    printf("Port%d device type: %s\n",p,dev_type[ata[p].dev_type]);

    return 0;
}


static void ata_pio_read(int p,void *buffer,int bytes)
{
__asm__ __volatile__(\
                "cld;\
                 rep; insw"::"D"(buffer),\
                "d"(ata[p].cmd_block_base_addr + ATA_REG_DATA),\
                "c"(bytes/2));


}


static void ata_pio_write(int p,void *buffer,int bytes)
{
__asm__ __volatile__(\
                "cld;\
                 rep; outsw"::"S"(buffer),\
                "d"(ata[p].cmd_block_base_addr + ATA_REG_DATA),\
                "c"(bytes/2));
}

static void ata_set_device_and_sector(int p,uint32_t count,uint64_t addr)
{

    switch(ata[p].lba)
    {
    case 28:
        //Mode LBA28
	    outb(ata[p].cmd_block_base_addr + ATA_REG_SECCOUNT,count);	// Sector Count 7:0
	    outb(ata[p].cmd_block_base_addr + ATA_REG_LBA0,addr);		        // LBA 7-0   
	    outb(ata[p].cmd_block_base_addr + ATA_REG_LBA1,addr >> 8);          // LBA 15-8
	    outb(ata[p].cmd_block_base_addr + ATA_REG_LBA2,addr >> 16);	        // LBA 23-16
         // Modo LBA active, Select device, and LBA 27-24
        outb(ata[p].cmd_block_base_addr + ATA_REG_DEVSEL,0x40 |(ata[p].dev_num << 4) | (addr >> 24 &0x0f));
        // verifique se e a mesma unidade para nao esperar pelos 400ns
        if(ata_record_dev != ata[p].dev_num && ata_record_channel != ata[p].channel){
            ata_wait(400);
            //verifique erro
            ata_record_dev      = ata[p].dev_num;
            ata_record_channel  = ata[p].channel;}
        break;
    case 48:
        //Mode LBA48
        outb(ata[p].cmd_block_base_addr + ATA_REG_SECCOUNT,0);	            // Sector Count 15:8
	    outb(ata[p].cmd_block_base_addr + ATA_REG_LBA0,addr >> 24);	        // LBA 31-24   
	    outb(ata[p].cmd_block_base_addr + ATA_REG_LBA1,addr >> 32);	        // LBA 39-32
	    outb(ata[p].cmd_block_base_addr + ATA_REG_LBA2,addr >> 40);	        // LBA 47-40
	    outb(ata[p].cmd_block_base_addr + ATA_REG_SECCOUNT,count);	        // Sector Count 7:0
	    outb(ata[p].cmd_block_base_addr + ATA_REG_LBA0,addr);		        // LBA 7-0   
	    outb(ata[p].cmd_block_base_addr + ATA_REG_LBA1,addr >> 8);           // LBA 15-8
	    outb(ata[p].cmd_block_base_addr + ATA_REG_LBA2,addr >> 16);	        // LBA 23-16
        outb(ata[p].cmd_block_base_addr + ATA_REG_DEVSEL,0x40 | ata[p].dev_num << 4);   // Modo LBA active, Select device,        
        // verifique se e a mesma unidade para nao esperar pelos 400ns
        if(ata_record_dev != ata[p].dev_num && ata_record_channel != ata[p].channel){
            ata_wait(400);
            ata_record_dev      = ata[p].dev_num;
            ata_record_channel  = ata[p].channel;}
        break;
    case 0:
        //Modo CHS
        // not suport
        break;



       }
       

}


typedef struct ide_prdt 
{
    uint32_t    addr;
    uint32_t    len;
}ide_prdt_t;

ide_prdt_t *ide_dma_prdt = (ide_prdt_t*)0x10000;

static void ide_dma_data(int p,void *addr,uint16_t byte_count,uint8_t flg)
{
    uint8_t data;
           
    ide_dma_prdt[p].addr   = (uint32_t)addr &~1;
    ide_dma_prdt[p].len    = (byte_count &~1) |0x80000000;


    uint32_t *physical =(uint32_t*)(ide_dma_prdt + p);

    // prds physical
    outpl(ata[p].bus_master_base_addr + IDE_DMA_REG_ADDR,physical);

    //bit 3 read/write
    // attr RWC
    //      0 = Memory reads
    //      1 = Memory writes
    data = inb(ata[p].bus_master_base_addr + IDE_DMA_REG_CMD);

    switch(flg)
    {
        case IDE_DMA_BUS_READ: 
        outb(ata[p].bus_master_base_addr + IDE_DMA_REG_CMD, data | IDE_DMA_BUS_RE_WR);
    break;
        case IDE_DMA_BUS_WRITE: 
        outb(ata[p].bus_master_base_addr + IDE_DMA_REG_CMD, data &~ IDE_DMA_BUS_RE_WR);
    break;
    }

    // Clear the Interrupt bit and Error bit in the Status register
    // attrs RWC
    data = inb(ata[p].bus_master_base_addr + IDE_DMA_REG_STS);
    outb(ata[p].bus_master_base_addr + IDE_DMA_REG_STS, data | 6);

    
    
}


static void ide_dma_start(int p)
{
    // bit 0 srtat/stop
    uint8_t data = inb(ata[p].bus_master_base_addr + IDE_DMA_REG_CMD);
    outb(ata[p].bus_master_base_addr + IDE_DMA_REG_CMD, data | IDE_DMA_BUS_ST_SP);

}

static void ide_dma_stop(int p)
{
    uint8_t data = inb(ata[p].bus_master_base_addr + IDE_DMA_REG_CMD);
    outb(ata[p].bus_master_base_addr + IDE_DMA_REG_CMD, data &~ IDE_DMA_BUS_ST_SP);
    // Clear the Interrupt bit and Error bit in the Status register
    data = inb(ata[p].bus_master_base_addr + IDE_DMA_REG_STS);
    outb(ata[p].bus_master_base_addr + IDE_DMA_REG_STS, data | 6);

}

static int ide_dma_read_status(int p)
{
    return inb(ata[p].bus_master_base_addr + IDE_DMA_REG_STS);
}



int ide_identify_device(int p)
{
    uint8_t lba1,lba2;

    // Ponto flutuante
    if(ata_status_read(p) == 0xff)//Sem unidade conectada ao barramento
        return -1;


    outb(ata[p].cmd_block_base_addr + ATA_REG_SECCOUNT,0);	// Sector Count 7:0
	outb(ata[p].cmd_block_base_addr + ATA_REG_LBA0,0);        // LBA 7-0   
	outb(ata[p].cmd_block_base_addr + ATA_REG_LBA1,0);        // LBA 15-8
	outb(ata[p].cmd_block_base_addr + ATA_REG_LBA2,0);        // LBA 23-16
    
    // Select device,
    outb(ata[p].cmd_block_base_addr + ATA_REG_DEVSEL,0xE0| ata[p].dev_num << 4);
    ata_wait(400);



    // cmd
    ata_cmd_write(p,ATA_CMD_IDENTIFY_DEVICE); 
    // ata_wait_irq(p);
    // Nunca espere por um IRQ aqui
    // Devido unidades ATAPI, ao menos que pesquisamos pelo Bit ERROR
    // Melhor seria fazermos polling
     
    ata_wait(400);
    
    if(ata_status_read(p) == 0) //Sem unidade no canal 
        return -1;

   lba1 = inb(ata[p].cmd_block_base_addr + ATA_REG_LBA1);
   lba2 = inb(ata[p].cmd_block_base_addr + ATA_REG_LBA2);

   if(lba1 == 0x14 && lba2 == 0xEB){
        //kputs("Unidade PATAPI\n");   
        ata_cmd_write(p,ATA_CMD_IDENTIFY_PACKET_DEVICE);
        ata_wait(400);
        ata_wait_drq(p); 
        ata_pio_read(p,ata_identify_dev_buf,512);
        ata_wait_not_busy(p);
        ata_wait_no_drq(p);
        

        return 0x80;
   }
   else if(lba1 == 0x69  && lba2 == 0x96){

        //kputs("Unidade SATAPI\n");   
        ata_cmd_write(p,ATA_CMD_IDENTIFY_PACKET_DEVICE);
        ata_wait(400);
        ata_wait_drq(p); 
        ata_pio_read(p,ata_identify_dev_buf,512);
        ata_wait_not_busy(p);
        ata_wait_no_drq(p);
        return 0x80;

   }
   else if(lba1 == 0x3C && lba2 == 0xC3){
        //kputs("Unidade SATA\n");   
        // O dispositivo responde imediatamente um erro ao cmd Identify device
        // entao devemos esperar pelo DRQ ao invez de um BUSY
        // em seguida enviar 256 word de dados PIO.
        ata_wait_drq(p); 
        ata_pio_read(p,ata_identify_dev_buf,512);
        ata_wait_not_busy(p);
        ata_wait_no_drq(p);
        return 0;
   }


   else if(lba1 == 0 && lba2 == 0){
        // kputs("Unidade PATA\n");
        // aqui esperamos pelo DRQ
        // e eviamoos 256 word de dados PIO
        ata_wait_drq(p);
        ata_pio_read(p,ata_identify_dev_buf,512);
        ata_wait_not_busy(p);
        ata_wait_no_drq(p);

        return 0;
    }


	return 0;   

}


int ide_read_sector(int p,uint16_t count,uint64_t addr, void *buffer)
{ 
        //select device, lba, count
        ata_set_device_and_sector(p,count,addr);


        if(ata[p].mts == ATA_DMA_MODO)
        switch(p){
            case 0: dma_addr = (uint8_t*)DMA_PHYS_ADDR0;
                    break;
            case 1: dma_addr = (uint8_t*)DMA_PHYS_ADDR1;
                    break;
            case 2: dma_addr = (uint8_t*)DMA_PHYS_ADDR2;
                    break;
            case 3: dma_addr = (uint8_t*)DMA_PHYS_ADDR3;
                    break;

        }


        
        
        if(ata[p].lba == ATA_LBA28 && ata[p].mts == ATA_PIO_MODO){

            ata_cmd_write(p,ATA_CMD_READ_SECTORS);
            //ata_wait_irq(current_dev->irq); //FIXME IRQs
            ata_wait_not_busy(p);
            if(ata_wait_drq(p) != 0)return -1;
            ata_pio_read(p,buffer,ata[p].bps);
            ata_wait_not_busy(p);
            if(ata_wait_no_drq(p) != 0){

            return -1;

            }

        }

        else if(ata[p].lba == ATA_LBA48 &&ata[p].mts == ATA_PIO_MODO){

            ata_cmd_write(p,ATA_CMD_READ_SECTORS_EXT);
            //ata_wait_irq(ata[p].irq); //FIXME IRQs
            ata_wait_not_busy(p);
            if(ata_wait_drq(p) != 0)return -1;
            ata_pio_read(p,buffer,ata[p].bps);
            ata_wait_not_busy(p);
            if(ata_wait_no_drq(p) != 0){

            return -1;
            }
            
        }

        else if(ata[p].lba == ATA_LBA28 && ata[p].mts == ATA_DMA_MODO){

            
            ide_dma_data(p,dma_addr,ata[p].bps*count,IDE_DMA_BUS_READ);    
            ata_cmd_write(p,ATA_CMD_READ_DMA);
            
            ide_dma_start(p);

            ata_wait_irq(ata[p].irq);
            while(ide_dma_read_status(p) &1)
            if(ide_dma_read_status(p) &2){
            ide_dma_stop(p); 
            return -1;
            }
            ide_dma_stop(p);
            

            __asm__ __volatile__("cld; rep; movsd;"::"D"(buffer),\
            "S"(dma_addr),"c"((ata[p].bps*count)/4));
            
        }


        else if(ata[p].lba == ATA_LBA48 && ata[p].mts == ATA_DMA_MODO){
   

            ide_dma_data(p,dma_addr,ata[p].bps*count,IDE_DMA_BUS_READ);    
            ata_cmd_write(p,ATA_CMD_READ_DMA_EXT);
            
            ide_dma_start(p);

            ata_wait_irq(ata[p].irq);
            
            while(ide_dma_read_status(p) &1)
            if(ide_dma_read_status(p) &2){
            ide_dma_stop(p);

            return -1;
            }

            ide_dma_stop(p);
          
          
            __asm__ __volatile__("cld; rep; movsd;"::"D"(buffer),\
            "S"(dma_addr),"c"((ata[p].bps*count)/4));
         
        }

        return 0;

}

int ide_write_sector(int p,uint16_t count,uint64_t addr, void *buffer)
{ 
       
     
        //select device, lba, count
        ata_set_device_and_sector(p,count,addr);

        if(ata[p].mts == ATA_DMA_MODO)
        switch(p){
            case 0: dma_addr = (uint8_t*)DMA_PHYS_ADDR0;
                    break;
            case 1: dma_addr = (uint8_t*)DMA_PHYS_ADDR1;
                    break;
            case 2: dma_addr = (uint8_t*)DMA_PHYS_ADDR2;
                    break;
            case 3: dma_addr = (uint8_t*)DMA_PHYS_ADDR3;
                    break;

        }

        
        if(ata[p].lba == ATA_LBA28 && ata[p].mts == ATA_PIO_MODO){

            ata_cmd_write(p,ATA_CMD_WRITE_SECTORS);
            //ata_wait_irq(ata[p].irq); //FIXME IRQs
            ata_wait_not_busy(p);
            if(ata_wait_drq(p) != 0)return -1;
            ata_pio_write(p,buffer,ata[p].bps);

            //Flush Cache
            ata_cmd_write(p,ATA_CMD_FLUSH_CACHE);
            ata_wait_not_busy(p);
            if(ata_wait_no_drq(p) != 0){
            
            return -1;
            }
        }

        else if(ata[p].lba == ATA_LBA48 && ata[p].mts == ATA_PIO_MODO){

            ata_cmd_write(p,ATA_CMD_WRITE_SECTORS_EXT);
            //ata_wait_irq(ata[p].irq); //FIXME IRQs
            ata_wait_not_busy(p);
            if(ata_wait_drq(p) != 0)return -1;
            ata_pio_write(p,buffer,ata[p].bps);

            //Flush Cache
            ata_cmd_write(p,ATA_CMD_FLUSH_CACHE_EXT);
            ata_wait_not_busy(p);
            if(ata_wait_no_drq(p) != 0){

            return -1;
            }

        }    
    
        else if(ata[p].lba == ATA_LBA28 && ata[p].mts == ATA_DMA_MODO){
            
            __asm__ __volatile__("cld; rep; movsd;"::"S"(buffer),\
            "D"(dma_addr),"c"((ata[p].bps*count)/4));


            ide_dma_data(p,dma_addr,ata[p].bps*count,IDE_DMA_BUS_WRITE);    
            ata_cmd_write(p,ATA_CMD_WRITE_DMA);

            ide_dma_start(p);

            ata_wait_irq(ata[p].irq);
            while(ide_dma_read_status(p) &1)
            if(ide_dma_read_status(p) &2){
            ide_dma_stop(p);

            return -1;
            } 
            ide_dma_stop(p);
            //Flush Cache
            ata_cmd_write(p,ATA_CMD_FLUSH_CACHE); 

        } 


        else if(ata[p].lba == ATA_LBA48 && ata[p].mts == ATA_DMA_MODO){

            __asm__ __volatile__("cld; rep; movsd;"::"S"(buffer),\
            "D"(dma_addr),"c"((ata[p].bps*count)/4));            

            ide_dma_data(p,dma_addr,ata[p].bps*count,IDE_DMA_BUS_WRITE);    
            ata_cmd_write(p,ATA_CMD_WRITE_DMA_EXT);

            ide_dma_start(p);

            ata_wait_irq(ata[p].irq);
            while(ide_dma_read_status(p) &1)
            if(ide_dma_read_status(p) &2){
            ide_dma_stop(p);
 
            return -1;
            }
            ide_dma_stop(p);
            //Flush Cache
            ata_cmd_write(p,ATA_CMD_FLUSH_CACHE_EXT);  
        }


        return 0;

}

int atapi_read_sector(int p,uint16_t count,uint64_t addr,void *buffer)
{

    uint32_t sector_size = ata[p].bps;
    uint8_t  modo_atapi  = 0;
    char atapi_packet [] ={ATAPI_CMD_READ,0,0,0,0,0,0,0,0,0,0,0};

    
    if(ata[p].mts == ATA_DMA_MODO)
    switch(p){
        case 0: dma_addr = (uint8_t*)DMA_PHYS_ADDR0;
            break;
        case 1: dma_addr = (uint8_t*)DMA_PHYS_ADDR1;
            break;
        case 2: dma_addr = (uint8_t*)DMA_PHYS_ADDR2;
            break;
        case 3: dma_addr = (uint8_t*)DMA_PHYS_ADDR3;
            break;

    }
    
    

    // configurar o pacote SCSI, 
    // 12 bytes de tamanho
    // Use LBA28
    atapi_packet [ 0x02 ] = ( addr >> 24 ) & 0xff ;
    atapi_packet [ 0x03 ] = ( addr >> 16 ) & 0xff ;
    atapi_packet [ 0x04 ] = ( addr >> 8 ) & 0xff ;
    atapi_packet [ 0x05 ] = ( addr) & 0xff ;
    atapi_packet [ 0x09 ] = (count);
    
    // Seleccionar a unidade
    outb(ata[p].cmd_block_base_addr + ATA_REG_DEVSEL,0x40 | ata[p].dev_num << 4);
    // verifique se e a mesma unidade para nao esperar
    if(ata_record_dev != ata[p].dev_num && ata_record_channel != ata[p].channel){
            ata_wait(400); // 1000 ns
            ata_record_dev      = ata[p].dev_num;
            ata_record_channel  = ata[p].channel;
    }

    // Selecionar modo ATAPI
    // 0 = Modo PIO.
    // 1 = DMA or UDMA
    if( ata[p].mts == ATA_DMA_MODO){

        modo_atapi = 1;
        sector_size =0;
    } 
    outb(ata[p].cmd_block_base_addr + ATA_REG_FEATURES,modo_atapi); // Modo PIO or DMA

    // Defina o tamanho do buffer em LBA0 e LBA1
    // O tamanho do bytes por sector
     outb(ata[p].cmd_block_base_addr + ATA_REG_LBA0,sector_size &0xff);   // LBA 7-0   
	 outb(ata[p].cmd_block_base_addr + ATA_REG_LBA1,sector_size >> 8);   // LBA 15-8

    // Enviar comando PACKET
    ata_cmd_write(p,ATA_CMD_PACKET);
    if(modo_atapi == 0)ata_wait_irq(ata[p].irq); //FIXME IRQs
    else{ 
    if(ata_wait_not_busy(p) != 0)return -1;
    
                
    
    }

    if(ata_wait_drq(p) != 0)return -1;
    // Enviar os dados do pacote
    __asm__ __volatile__("cld; rep; outsw" ::"c" (6) , "d" (\
    (ata[p].cmd_block_base_addr +ATA_REG_DATA)),"S"(atapi_packet));
 
    if(ata_wait_drq(p) != 0)return -1; //TODO Nelson e mesmo correcto DRQ aqui?

    if(ata[p].mts == ATA_DMA_MODO){


           
            ide_dma_data(p,dma_addr,ata[p].bps*count,IDE_DMA_BUS_READ);    

            ide_dma_start(p);

            ata_wait_irq(ata[p].irq);

            while(ide_dma_read_status(p) &1)
            if(ide_dma_read_status(p) &2){
            ide_dma_stop(p); 
            return -1;
            } 

            ide_dma_stop(p);
            __asm__ __volatile__("cld; rep; movsd;"::"D"(buffer),\
            "S"(dma_addr),"c"((ata[p].bps*count)/4));
     

     return 0;


    }
    
    ata_pio_read(p,buffer,sector_size);
    //ata_wait_irq(ata[p].irq);
    ata_wait_not_busy(p);
    if(ata_wait_no_drq(p) != 0){

            return -1;
            } 
    return 0;
}
