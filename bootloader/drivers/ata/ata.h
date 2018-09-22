/*
 * Khole Operating System
 * ata/ata.h
 * 
 * Copyright (C) 2017,2018 Nelson Cole <nelsoncole72@gmail.com>
 */
#ifndef __ATA_H__
#define __ATA_H__
#include <typedef.h>
#include <drivers/pci.h>
#include <io.h>
#include <irqs.h>
#include <dev.h>


// base address 
uint32_t ATA_BAR0;    // Primary Command Block Base Address
uint32_t ATA_BAR1;    // Primary Control Block Base Address
uint32_t ATA_BAR2;    // Secondary Command Block Base Address
uint32_t ATA_BAR3;    // Secondary Control Block Base Address
uint32_t ATA_BAR4;    // Legacy Bus Master Base Address
uint32_t ATA_BAR5;    // AHCI Base Address / SATA Index Data Pair Base Address



#define DMA_PHYS_ADDR0  (0x40000)
#define DMA_PHYS_ADDR1  (DMA_PHYS_ADDR0 + 0x10000)
#define DMA_PHYS_ADDR2  (DMA_PHYS_ADDR1 + 0x10000)
#define DMA_PHYS_ADDR3  (DMA_PHYS_ADDR2 + 0x10000)


#define PCI_CALSS_MASS_STORAGE  1

// Controladores de unidades ATA
#define ATA_IDE_CONTROLLER      0x1
#define ATA_RAID_CONTROLLER     0x4
#define ATA_AHCI_CONTROLLER     0x6


// IO Space Legacy BARs IDE 
#define ATA_IDE_BAR0 0x1F0  // Primary Command Block Base Address
#define ATA_IDE_BAR1 0x3F6  // Primary Control Block Base Address
#define ATA_IDE_BAR2 0x170  // Secondary Command Block Base Address
#define ATA_IDE_BAR3 0x376  // Secondary Control Block Base Address
#define ATA_IDE_BAR4 0      // Bus Master Base Address
#define ATA_IDE_BAR5 0      //



// ATA/ATAPI Command Set

#define ATA_CMD_CFA_ERASE_SECTORS               0xC0
#define ATA_CMD_CFA REQUEST_EXTENDED_ERROR_CODE 0x03
#define ATA_CMD_CHECK_MEDIA_CARD_TYPE           0xD1
#define ATA_CMD_CHECK_POWER_MODE                0xE5
#define ATA_CMD_DEVICE_RESET                    0x08
#define ATA_CMD_EXECUTE_DEVICE_DIAGNOSTIC       0x90
#define ATA_CMD_FLUSH_CACHE                     0xE7
#define ATA_CMD_FLUSH_CACHE_EXT                 0xEA
#define ATA_CMD_IDENTIFY_DEVICE                 0xEC
#define ATA_CMD_IDENTIFY_PACKET_DEVICE          0xA1
#define ATA_CMD_PACKET                          0xA0
#define ATA_CMD_READ_BUFFER                     0xE4
#define ATA_CMD_READ_DMA                        0xC8
#define ATA_CMD_READ_DMA_EXT                    0x25
#define ATA_CMD_READ_SECTORS                    0x20
#define ATA_CMD_READ_SECTORS_EXT                0x24
#define ATA_CMD_WRITE_BUFFER                    0xE8
#define ATA_CMD_WRITE_DMA                       0xCA
#define ATA_CMD_WRITE_DMA_EXT                   0x35
#define ATA_CMD_WRITE_SECTORS                   0x30
#define ATA_CMD_WRITE_SECTORS_EXT               0x34

// ATAPI descrito no SCSI
#define ATAPI_CMD_READ  0xA8
#define ATAPI_CMD_EJECT 0x1B

//ATA bits de status control (alternativo)
#define ATA_SC_HOB      0x80    // High Order Byte
#define ATA_SC_SRST     0x04    // Soft Reset
#define ATA_SC_nINE     0x02    // INTRQ




//ATA bits de status 
#define ATA_SR_BSY      0x80    // Busy
#define ATA_SR_DRDY     0x40    // Device Ready
#define ATA_SR_DF       0x20    // Device Fault
#define ATA_SR_DSC      0x10    // Device Seek Complete
#define ATA_SR_DRQ      0x08    // Data Request
#define ATA_SR_SRST     0x04    // 
#define ATA_SR_IDX      0x02    // Index
#define ATA_SR_ERR      0x01    // Error

//ATA bits de errro apos a leitura.
#define ATA_ER_BBK      0x80    // 
#define ATA_ER_UNC      0x40    //
#define ATA_ER_MC       0x20    //
#define ATA_ER_IDNF     0x10    //
#define ATA_ER_MCR      0x08    //
#define ATA_ER_ABRT     0x04    //
#define ATA_ER_TK0NF    0x02    //
#define ATA_ER_AMNF     0x01    //


// Registradores 
#define ATA_REG_DATA 0x00
#define ATA_REG_ERROR 0x01
#define ATA_REG_FEATURES 0x01
#define ATA_REG_SECCOUNT 0x02
#define ATA_REG_LBA0 0x03
#define ATA_REG_LBA1 0x04
#define ATA_REG_LBA2 0x05
#define ATA_REG_DEVSEL 0x06
#define ATA_REG_CMD 0x07
#define ATA_REG_STATUS 0x07


// Devices
#define ATA_MASTER_DEV  0x00
#define ATA_SLAVE_DEV   0x01

// Bus
#define ATA_PRIMARY     0x00
#define ATA_SECONDARY   0x01

// ATA type
#define ATA_DEVICE_TYPE     0x00
#define ATAPI_DEVICE_TYPE   0x01


// Modo de transferencia
#define ATA_PIO_MODO 0 
#define ATA_DMA_MODO 1
#define ATA_LBA48    48
#define ATA_LBA28    28


#define IDE_IRQ14 0
#define IDE_IRQ15 1

// Commands PCI busmastering dma 
#define IDE_DMA_BUS_ST_SP   1
#define IDE_DMA_BUS_RE_WR   8
#define IDE_DMA_BUS_READ    0
#define IDE_DMA_BUS_WRITE   1


// Status dma
#define IDE_DMA_SR_ERR     0x02




// Registros bus master base address
#define IDE_DMA_REG_CMD     0x00
#define IDE_DMA_REG_STS     0x02
#define IDE_DMA_REG_ADDR    0x04

// channel
#define IDE_DMA_PRIMARY     0x00
#define IDE_DMA_SECUNDARY   0x01


// Historia
// Programaçao do ATA a partir do ICH5/9 e suporte a IDE legado.
// ICH5 integraçao do SATA e suporte total ACPI 2.0.
// ICH6 implementaram os controladores AHCI SATA pela primeira vez.
volatile struct {

    uint8_t     adapter;

    // Suporta a IDE Controller
    uint16_t    vendor_id;
    uint16_t    device_id;
    uint16_t    command;
    uint16_t    status;
    uint8_t     prog_if;
    uint8_t     revision_id;
    uint8_t     classe;
    uint8_t     subclasse;
    uint8_t     primary_master_latency_timer;
    uint8_t     header_type;
    uint8_t     BIST;
    uint32_t    bar0;
    uint32_t    bar1; 
    uint32_t    bar2;
    uint32_t    bar3;
    uint32_t    bar4;
    uint32_t    bar5;
    uint16_t    subsystem_vendor_id;
    uint16_t    subsystem_id;  
    uint32_t    capabilities_pointer;
    uint8_t     interrupt_line;
    uint8_t     interrupt_pin;

    // AHCI

    // "Emos" de acrescer com o tempo de acordo nossas necessidades.


}ata_pci;


volatile struct ata{
    uint32_t    irq; // IRQn 14 or 15
    uint32_t    bps; // bytes per sector
    uint32_t    mts; // mode of transfere (DMA or PIO)
    uint32_t    lba; // LBA28 or LBA48
    uint32_t    dev_num;
    uint32_t    dev_type;
    uint32_t    channel; 
    uint32_t    cmd_block_base_addr;
    uint32_t    ctrl_block_base_addr;
    uint32_t    bus_master_base_addr;     


}ata[4];

/*************************** variaves *************************************/

uint16_t   *ata_identify_dev_buf;
uint8_t     ata_record_dev;
uint8_t     ata_record_channel;


// ata_main.c
int ata_pci_configuration_space(char bus,char dev,char fun);

// ata.c
void ata_wait(int val);
uint8_t ata_status_read(int p);
int ata_wait_not_busy(int p);
int ata_wait_busy(int p);
int ata_wait_no_drq(int p);
int ata_wait_drq(int p);
void ata_soft_reset(int p);
void ata_cmd_write(int p,int command);
void ata_irq_handler1(void);
void ata_irq_handler2(void);
int ata_wait_irq(int irq);


#endif
