#include <io.h>
#include <dev.h>
#include <drivers/pci.h>



static uint32_t nicbar0 = 0;
static uint32_t nicbar1 = 0;
static uint32_t nicbar2 = 0;

static int i82540_pci_configuration_space(char bus,char dev,char fun);

int nic_test(){


	char *p;

	uint32_t data = pci_scan_bcc(0x2);

	if(data == -1) {

		
		set_color(4);
		puts("PCI PANIC: Network Controller not found!\n");
		set_color(0xf);
		return (PCI_MSG_ERROR);
	}


	i82540_pci_configuration_space(data  >>  8 &0xff,data  >> 3  &31,data &7);

	p = (char*) mem_map((void*)(nicbar0),0x13,512);


	printf("REG_RAL ( 0x5400 ) %X, REG_RAH ( 0x5404 ) %X\n\n",(*(uint32_t*)(p + 0x5400)),(*(uint32_t*)(p + 0x5404)) );


	return 0;

}



static int i82540_pci_configuration_space(char bus,char dev,char fun)
{

	uint32_t data;
    	// VENDOR and DEVICE ID offset 0
    	data  = read_pci_config_addr(bus,dev,fun,0);

	printf("\n\nPCI Network Controller initialize\n");

	printf("Vendor ID %X, Device ID %X\n",data &0xffff, data >> 16 &0xffff);

	nicbar0  = read_pci_config_addr(bus,dev,fun,0x10) & 0xFFFFFFF0;
	printf("BAR0 %X\n",nicbar0);

	nicbar1  = read_pci_config_addr(bus,dev,fun,0x14);
	printf("BAR1 %X\n",nicbar1);

	nicbar2  = read_pci_config_addr(bus,dev,fun,0x18) & 0xFFFFFFF0;
	printf("BAR2 %X\n",nicbar2);






	return 0;
}
