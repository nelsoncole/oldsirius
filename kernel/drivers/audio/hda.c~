#include <io.h>
#include <drivers/pci.h>
#include <dev.h>
#include "hda.h"
#include <string.h>

uint32_t HDBARL = 0;
uint32_t HDBARU = 0;
uint32_t HDA_MEM_SIZE =0;
uint32_t HDA_MEM_BASE = 0x20000;
HDA_T *HDA_MEM = 0;
stpci_t *pcihda;

hda_node_t *current_hdanode;
hda_node_t *queue_hdanode;

static int hda_pci_configuration_space(int bus,int dev,int fun);
static void hda_init_corb_rirb(HDA_T *base);
static void hda_corbrum(HDA_T *base,int flg);
static void hda_rirbrum(HDA_T *base,int flg);
static uint32_t hda_ic(uint32_t codec);
static void hda_flush(HDA_T *base);

static int hda_init_node();
int hda_init(void)
{	uint32_t data = pci_scan_bcc_scc(4,3);
	if(data == -1) {
		set_color(0x4);
		printf("PCI PANIC: Intel High Definition Audio (HDA) Controller not found!\n");set_color(0xf);
		return -1;
	}	 
	printf("PCI Intel High Definition Audio Controller initialize\n");
	pcihda = (stpci_t *)malloc(sizeof(stpci_t));

	data = hda_pci_configuration_space(data  >>  8 &0xff,data  >> 3  &31,data &7);

	if(data) {
		set_color(0x4);
		printf("PCI PANIC: Intel High Definition Audio (HDA) Controller error!\n");set_color(0xf);
		free(pcihda);
		return -1;
	}


	HDA_MEM = (HDA_T*)mem_map((void*)(HDBARL),0x13,HDA_MEM_SIZE);



	set_color(0xf);
	printf("Intel High Definition Audio Controller version %d.%d\n",HDA_MEM->vmaj,HDA_MEM->vmin);

	// Interrupt Control
	HDA_MEM->intctl |= HDA_INT_GIE | HDA_INT_CIE | HDA_INT_SIE(- 1); 
	// Controller Reset (CRST), to exit
	HDA_MEM->gctl |= HDA_GCTL_CRST;
	HDA_MEM->gctl |= 0x100;

	// codecs disponiveis
	// testes
	if(!(HDA_MEM->statests&0xf)) {

		if(!(hda_ic(__codec(0x04/*cdata*/,0xf00/*verb*/,0x00/*nid*/,0/*i*/,0x0/*cad*/)))) {
			printf("HD Audio, nenhum codec disponivel\n");
			set_color(0xf);
			free(pcihda);
			return -1;
		}
	}

	printf("HD Audio, codec(s) disponiveis %d\n",HDA_MEM->statests);

	//CORB and RIRB init
	hda_init_corb_rirb(HDA_MEM);

	// Nodeaddressing
	hda_init_node();


	set_color(0xf);
	free(pcihda);
	return 0;
}
// FIXME CORB
// CORB Base Address
static void hda_init_corb_rirb(HDA_T *base)
{

	base->corbctl &=~HDA_CORB_DMAE; 		// CORB RUM STOP
	base->corblbase = (uint32_t) HDA_MEM_BASE; 	// CORB Lower Base Address
	base->corbubase = (uint32_t) 0;			// CORB Upper Base Address
	memset((void*)(base->corblbase),0,1024);
	
	base->corbrp |= HDA_CORB_RPST;
	base->corbwp = 0;
	base->corbctl |= HDA_CORB_MEIE;

	HDA_MEM_BASE += 0x400;

	base->rirbctl &=~HDA_CORB_DMAE;
	base->rirblbase = (uint32_t) HDA_MEM_BASE; 	// RIRB Lower Base Address
	base->rirbubase = (uint32_t) 0;			// RIRB Upper Base Address
	memset((void*)(base->rirblbase),0,1024);
	base->rintctl = 0xff;
	HDA_MEM_BASE += 0x800;
	
}
// CORB Write Pointer
static uint16_t hda_corbwp(HDA_T *base,int index)
{
	return base->corbwp = (uint16_t) ((index * 4) &0xff);
}
// CORB Read Pointer
static uint16_t hda_corbrp(HDA_T *base,int index)
{
	return base->corbrp = (uint16_t) ((index * 4) &0xff);
}
// CORB Control
static void hda_corbrum(HDA_T *base,int flg)
{
	if(flg)base->corbctl |= HDA_CORB_DMAE;	
	else base->corbctl &=~HDA_CORB_DMAE; 
}
// RIRB Control
static void hda_rirbrum(HDA_T *base,int flg)
{
	if(flg)base->rirbctl |= HDA_CORB_DMAE;	
	else base->rirbctl &=~HDA_CORB_DMAE; 
}

static uint32_t hda_ic(uint32_t codec)
{
	int spin = 10000;



	HDA_MEM->icoi = codec;
	while(HDA_MEM->icis&1) {

		if(!(spin--)) {
			printf("HD Audio, IC busy\n");
			return (int) -1;
		}
	}
	spin = 10000;
	HDA_MEM->icis |= HDA_ICB |HDA_IRV; // clear ICB and new command

	while(!(HDA_MEM->icis&2)) {
	
		if(!(spin--)) {
			printf("HD Audio, IC error\n");
			return (int) -1;
		}
	}

	return (int) HDA_MEM->icii;
}

// FLUSH
static void hda_flush(HDA_T *base) 
{
	base->gsts = -1;
	base->gctl |= HDA_GCTL_FCNTRL;
	// wait flush complete
	while(!(base->gsts &HDA_GSTS_FSTS))inportb(0x80);
}

//  DMA Position Base Address
static void *hda_dma_baddr(HDA_T *base,uint32_t addr)
{

	// wait DMA engine is running
	// while(true)inportb(0x80);

	hda_flush(base);
	
	base->dplbase = (uint32_t) addr |1; 	// DMA Position Lower Base Address
	base->dpubase = (uint32_t) 0;		// DMA Position Upper Base Address
	return (uint32_t*) addr;
}


static void  hda_add_node(int nid,int startn,int numn,int type,int fun_grup_typ,int addr) 
{
	hda_node_t *new = (hda_node_t*)malloc(sizeof(hda_node_t));
	new->nid = nid;
	new->startn = startn;
	new->numn = numn;
	new->type = type;
	new->fun_grup_typ =fun_grup_typ;
	new->addr = addr;
	new->next = 0;

	// add no final da lista
	hda_node_t *p = (hda_node_t *)queue_hdanode;
	while (p->next) p = (hda_node_t*)p->next;
    	p->next = (hda_node_t*) new;

}

static int hda_init_node()
{
	current_hdanode = queue_hdanode = (hda_node_t*)malloc(sizeof(hda_node_t));

	int nid = 0;
	int startn, numn,startn1, numn1,fun_grup_typ;	
	int data_return = hda_ic( __codec(0x04,0xf00,nid,0,0x0));

	current_hdanode->nid = nid;
	current_hdanode->startn = startn = data_return &0xffff;
	current_hdanode->numn = numn = data_return >>16 &0xffff;
	current_hdanode->addr = 0;
	current_hdanode->type = 0;
	current_hdanode->fun_grup_typ =0;
	current_hdanode->next = 0;


	int i,t;
	for(i= startn;i <= numn;i++) {


		//funtion grup nodes (AFG)
		fun_grup_typ = data_return = hda_ic( __codec(0x05,0xf00,i,0,0x0));
		data_return = hda_ic( __codec(0x04,0xf00,i,0,0x0));
		hda_add_node(i,(data_return &0xffff),(data_return >>16 &0xffff),NODE_FUNTION,fun_grup_typ,0);


		startn1 = data_return &0xffff;
		numn1 = data_return >>16 &0xffff;
		// widget node
		for(t = startn1; t < startn1 + numn1;t++) {
			data_return = hda_ic( __codec(0xc,0xf00,t,0,0x0));
			hda_add_node(t,(data_return &0xffff),(data_return >>16 &0xffff),0,NODE_WIDGET,0);
			printf("%X\n",data_return);
		}

	}


	return 0;
}










// z0  |= ((data  >>  8 &0xff) << 16) | ((data >> 3 &31) << 8) | (data &7);
static int hda_pci_configuration_space(int bus,int dev,int fun)
{	uint32_t data = 0;
	uint32_t z0 = (fun &0xff) | ((dev &0xff) << 8) | ((bus &0xff) << 16);
	pci_t *pci = (pci_t*)((uint32_t)&z0);
	data = pci_read_config_dword(pci->bus,pci->dev,pci->fun,0x00);
	pcihda->vid = data &0xffff;
	pcihda->did = (data >>16) &0xffff;

	// STATUS CMD Enable Memory mapped and Bus Master
	data = pci_read_config_dword(pci->bus,pci->dev,pci->fun,0x04);
	pci_write_config_dword(pci->bus,pci->dev,pci->fun,0x04,data | 0x6);

	data = pci_read_config_dword(pci->bus,pci->dev,pci->fun,0x04);
	pcihda->cmd = data &0xffff;
	pcihda->sts = (data >>16) &0xffff;
	data = pci_read_config_dword(pci->bus,pci->dev,pci->fun,0x08);
	pcihda->rid = data &0xff;
	pcihda->pi = (data >>8) &0xff;
	pcihda->scc = (data >>16) &0xff;
	pcihda->bcc = (data >>24) &0xff;
	data = pci_read_config_dword(pci->bus,pci->dev,pci->fun,0x0c);
	pcihda->cls = data &0xff;
	pcihda->lt = (data >>8) &0xff;
	pcihda->headtyp = (data >>16) &0xff;
	pcihda->BIST = (data >>24) &0xff;

	// BAR0-1
	// Salve
	data = pci_read_config_dword(pci->bus,pci->dev,pci->fun,0x10);
	pci_write_config_dword(pci->bus,pci->dev,pci->fun,0x10,0xffffffff);
	HDA_MEM_SIZE = pci_read_config_dword(pci->bus,pci->dev,pci->fun,0x10) & 0x3fff;
 	HDA_MEM_SIZE++;
	// Restore
	pci_write_config_dword(pci->bus,pci->dev,pci->fun,0x10,data);
	// Read BAR
	data = pci_read_config_dword(pci->bus,pci->dev,pci->fun,0x10);
	HDBARL = data &~0x3fff;
	pcihda->bar0 = data;
	data = pci_read_config_dword(pci->bus,pci->dev,pci->fun,0x14);
	HDBARU = data;
	pcihda->bar1 = data;


	data = pci_read_config_dword(pci->bus,pci->dev,pci->fun,0x2c);
	pcihda->svid = data &0xffff;
	pcihda->sid = (data >>16) &0xffff;
	data = pci_read_config_dword(pci->bus,pci->dev,pci->fun,0x34);
	pcihda->capptr = data &0xff;
	data = pci_read_config_dword(pci->bus,pci->dev,pci->fun,0x3c);
	pcihda->intln = data &0xff;
	pcihda->intpn = (data >>8) &0xff;	
	printf("DID: %X VID: %X\n",pcihda->did,pcihda->vid);
	printf("STS: %X CMD: %X\n",pcihda->sts,pcihda->cmd);
	printf("BCC: %d SCC: %d PI: %d RID: %d\n",pcihda->bcc,pcihda->scc,pcihda->pi,pcihda->rid);
	printf("BIST: %X HEADTYP: %X LT: %d CLS: %d\n",pcihda->cls,pcihda->lt,pcihda->headtyp,pcihda->BIST);
	printf("HDBARU: %X HDBARL: %X HDA_MEM_SIZE %dkB\n",HDBARU,HDBARL,HDA_MEM_SIZE);
	printf("SID: %X SVID: %X\n",pcihda->sid,pcihda->svid);
	printf("INTPN: %d INTLN: %d CAPPTR: %d\n",pcihda->intpn,pcihda->intln,pcihda->capptr);
	return 0;
}





















