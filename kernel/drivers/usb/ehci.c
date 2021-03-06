#include <io.h>
#include <drivers/pci.h>
#include <dev.h>
#include "ehci.h"

ehcipci_t *ehcipci;
EHCI_MEM_T *ehci_mem_base;
static int ehci_pci_configuration_space(char bus,char dev,char fun);

int ehci_init(void)
{

    	uint32_t data = pci_scan_bcc_scc_prog(0xC,0x3,0x20);
    	if(data  == -1)	{
		set_color(0x4);
    		puts("PCI PANIC: EHCI (USB2.0) Controller not found!\n");
		set_color(0xf);
    		return (PCI_MSG_ERROR); // Abortar
    	}

    	ehcipci =(ehcipci_t*)kalloc(1);

    	ehci_pci_configuration_space(data  >>  8 &0xff,data  >> 3  &31,data &7);

    	ehci_mem_base = (EHCI_MEM_T*)mem_map((void*)(ehcipci->mem_base),0x13,1);

    	printf("Testing EHCI (USB2.0)\n");
    	printf("caplength %X ",ehci_mem_base->caplength);
    	printf("hcivrsion %X ",ehci_mem_base->hcivrsion);
    	printf("hcsparams %d\n",ehci_mem_base->hcsparams &0xf);

    	//ehci_mem_base->port[0].preset = 1;
    	printf("ccsts %d ",ehci_mem_base->port[0].ccsts);
    	printf("cstsc %d ",ehci_mem_base->port[0].cstsc);
    	printf("ped %d ",ehci_mem_base->port[0].ped);
    	printf("pdc %d\n",ehci_mem_base->port[0].pdc);

	kfree_alloc(ehcipci);
	return 0;
}


static int ehci_pci_configuration_space(char bus,char dev,char fun)
{
    	uint32_t data;
    	// VENDOR and DEVICE ID offset 0
    	data  = read_pci_config_addr(bus,dev,fun,0);
    	ehcipci->vendor_id = data &0xffff;
    	ehcipci->device_id = data >> 16 &0xffff;

    	// CMD and STS offset 0x4    
    	data  = read_pci_config_addr(bus,dev,fun,4);
    	ehcipci->cmd = data &0xffff;
    	ehcipci->sts = data >> 16 &0xffff;

    	// BCC, SCC, ProgIF and Revision ID offset 8
    	data  = read_pci_config_addr(bus,dev,fun,8);
    	ehcipci->revision_id = data &0xff;
    	ehcipci->progif = data >> 8 &0xff;
    	ehcipci->scc = data >> 16 &0xff;
    	ehcipci->bcc = data >> 24 &0xff;

    	// Latency
    	data  = read_pci_config_addr(bus,dev,fun,0xD);
    	ehcipci->primary_master_latency_timer = data >> 8 &0xff;

    	// BAR
    	data  = read_pci_config_addr(bus,dev,fun,0x10);
    	ehcipci->mem_base = data &~0x3ff;

    	// Interrupter
    	data  = read_pci_config_addr(bus,dev,fun,0x3C);
    	ehcipci->intline = data &0xff;
    	ehcipci->intpin = data >>8 &0xff;
    

    	printf("EHCI (USB2.0)\nVendor ID %X Device ID %X\nCommand %X Status %X\n",\
    	ehcipci->vendor_id,ehcipci->device_id,ehcipci->cmd,ehcipci->sts);
    	printf("BCC %d SCC %d ProgIf %d Revision ID %d\n",\
    	ehcipci->bcc,ehcipci->scc,ehcipci->progif,ehcipci->revision_id); 
    	printf("Primary Master Latency Timer %d\nMemory Base Address %X\nInterrupt Line %d Interrupt Pin %d\n",\
    	ehcipci->primary_master_latency_timer,ehcipci->mem_base,ehcipci->intline,ehcipci->intpin);
  
    	return 0;
}
