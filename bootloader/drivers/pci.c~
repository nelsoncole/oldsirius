#include <io.h>
#include <drivers/pci.h>

uint32_t read_pci_config_addr(int bus,int dev,int fun, int offset){
    outportl(PCI_PORT_ADDR,CONFIG_ADDR(bus,dev,fun, offset));
    return inportl(PCI_PORT_DATA);
}

void write_pci_config_addr(int bus,int dev,int fun, int offset,int data){
    outportl(PCI_PORT_ADDR,CONFIG_ADDR(bus,dev,fun, offset));
    outportl(PCI_PORT_DATA,data);
}


// Esta funçao deve retornar o numero de barramento, a dispositivo e a funçao
// do dispositivo conectado ao barramento PCI, de acordo a classe.
uint32_t pci_scan_bcc(int bcc){
    uint32_t data = -1;

    int bus, dev, fun;

    
    for(bus = 0;bus < MAX_BUS; bus++){
        for(dev = 0; dev < MAX_DEV; dev++){
            for(fun = 0; fun < MAX_FUN; fun++){
                outportl(PCI_PORT_ADDR,CONFIG_ADDR(bus,dev,fun, 0x8));
                data =inportl(PCI_PORT_DATA);
                if((data >> 24 &0xff) == bcc){
                    return (fun + (dev * 8) + (bus * 32));
            
                }
            }
     
        }
     
    }

    return (-1);

}
uint32_t pci_scan_bcc_scc(int bcc,int scc)
{
    uint32_t data = -1;

    int bus, dev, fun;

    
    for(bus = 0;bus < MAX_BUS; bus++){
        for(dev = 0; dev < MAX_DEV; dev++){
            for(fun = 0; fun < MAX_FUN; fun++){
                outportl(PCI_PORT_ADDR,CONFIG_ADDR(bus,dev,fun, 0x8));
                data =inportl(PCI_PORT_DATA);
                if(((data >> 24 &0xff) == bcc) && ((data >> 16 &0xff) == scc)){
                    return (fun + (dev * 8) + (bus * 32));
            
                }
            }
     
        }
     
    }

    return (-1);

}

uint32_t pci_scan_bcc_scc_prog(int bcc,int scc,int prog)
{
    uint32_t data = -1;

    int bus, dev, fun;

    
    for(bus = 0;bus < MAX_BUS; bus++){
        for(dev = 0; dev < MAX_DEV; dev++){
            for(fun = 0; fun < MAX_FUN; fun++){
                outportl(PCI_PORT_ADDR,CONFIG_ADDR(bus,dev,fun, 0x8));
                data =inportl(PCI_PORT_DATA);
                if(((data >> 24 &0xff) == bcc) && ((data >> 16 &0xff) == scc) && ((data >> 8 &0xff) == prog)){
                    return (fun + (dev * 8) + (bus * 32));
            
                }
            }
     
        }
     
    }

    return (-1);

}
uint32_t pci_scan_vendor(short vendor)
{


    return 0;
}
uint32_t pci_check_vendor(int bus,int dev, int fun, short vendor)
{



    return 0;
}
