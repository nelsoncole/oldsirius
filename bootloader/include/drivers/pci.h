#ifndef __PCI_H__
#define __PCI_H__
#include <typedef.h>
#define MAX_BUS 2  // 256
#define MAX_DEV 32 // 32
#define MAX_FUN 8  // 8
#define PCI_MSG_ERROR       -1
#define PCI_MSG_AVALIABLE   0x80
#define PCI_MSG_SUCCESSFUL  0
#define PCI_PORT_ADDR 0xCF8
#define PCI_PORT_DATA 0xCFC



#define CONFIG_ADDR(bus,device,fn,offset)\
                       (\
                       (((uint32_t)(bus) &0xff) << 16)|\
                       (((uint32_t)(device) &0x3f) << 11)|\
                       (((uint32_t)(fn) &0x07) << 8)|\
                       ((uint32_t)(offset) &0xfc)|0x80000000)

uint32_t read_pci_config_addr(int bus,int dev,int fun, int offset);
void write_pci_config_addr(int bus,int dev,int fun, int offset,int data);
uint32_t pci_scan_bcc(int bcc);
uint32_t pci_scan_bcc_scc(int bcc,int scc);
uint32_t pci_scan_bcc_scc_prog(int bcc,int scc,int prog);
uint32_t pci_scan_vendor(short vendor);
uint32_t pci_check_vendor(int bus,int dev, int fun, short vendor);

#endif
