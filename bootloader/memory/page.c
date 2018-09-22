#include <io.h>
#include <string.h>
#include <memory.h>
#include <memory/page.h>

extern uint32_t used_page_directory;
extern uint32_t used_page_tables;
typedef volatile struct page_table
	{
	uint32_t p :1;
	uint32_t rw :1;
	uint32_t us :1;
	uint32_t pwt :1;
	uint32_t pcd :1;
	uint32_t a :1;
	uint32_t d :1;
	uint32_t pat :1;
	uint32_t g :1;
	uint32_t ign :3;
	uint32_t frames :20;

	}__attribute__((packed)) page_table_t;

typedef  volatile struct page_directory
	{
	uint32_t p :1;
	uint32_t rw :1;
	uint32_t us :1;
	uint32_t pwt :1;
	uint32_t pcd :1;
	uint32_t a :1;
	uint32_t ign 
:1;
	uint32_t ps :1;
	uint32_t ignored :4;
	uint32_t addrpt :20;

	}__attribute__((packed)) page_directory_t;

page_directory_t *page_directory= (page_directory_t*)0x00200000;
page_table_t *page_table =(page_table_t*)0x00201000; 

static void page_enable();
static void load_page_diretory(void *addr);
static void flush_tlb();
static uint32_t kernel_map(uint32_t kernel_virtual_mem,uint32_t kernel_physical_mem, uint32_t endkernel);
void page_install(uint32_t startkernel,uint32_t endkernel)
{	
	cli(); // disable interrupt
	int i;
	uint32_t addrtble;
	uint32_t physical = 0;

	memset((page_directory_t*)page_directory,0,sizeof(page_directory_t)*1024);
	memset((page_directory_t*)page_table,0,sizeof(page_table_t)*256*1024);
	
	// PTE de identidade
	for(i =0; i < 1024; i++){
		
		page_table[i].p = 1;
		page_table[i].rw= 1;
		page_table[i].pcd = 0; 
		page_table[i].g = 1;
		page_table[i].frames = ((physical+(i*0x1000)) >>12) &0xFFFFF;
	
	}

	// PDE de identidade
	page_directory[0].p = 1;
	page_directory[0].rw= 1;
	addrtble = (uint32_t)(page_table + (1024 *0));
	page_directory[0].addrpt = (addrtble >>12) &0xFFFFF;

	load_page_diretory((struct page_directory*)page_directory);
	page_enable();

	// Espera o MMU, completar a primeira tradução 
	wait_ns(400);
	

	used_page_directory = 1;
	used_page_tables = 1024;

	// Mapear okernel
	kernel_map(startkernel,KERNEL_PHYSICAL_MEM,endkernel);
	
	sti(); // enable interrupt

}

static uint32_t kernel_map(uint32_t kernel_virtual_mem,uint32_t kernel_physical_mem, uint32_t endkernel)
{
	uint32_t kernel_mem_size = ((endkernel - kernel_virtual_mem)/0x1000) + 1;

    	uint32_t pde = (uint32_t)kernel_virtual_mem >> 22 & 0x3FF;
    	uint32_t pte = (uint32_t)kernel_virtual_mem >> 12 & 0x3FF;
    	//uint32_t ofs = (uint32_t)kernel_virtual_mem  & 0xFFF;

	uint32_t physical = kernel_physical_mem;
	uint32_t addrtble;
	uint32_t pt = 1;

	int stop  = 1024;

	kernel_mem_size += pte;
	int x = kernel_mem_size/1024;
	int z = kernel_mem_size%1024;


	used_page_directory += (x) + ((z)? 1: 0);
	used_page_tables += (x*1024) + z;



	do {
		if(!(x)) stop = z;
		else stop = 1024;

		for(; pte < stop; pte++){
		
			page_table[pte+(1024 *pt)].p = 1;
			page_table[pte+(1024 *pt)].rw= 1;
			page_table[pte+(1024 *pt)].g = 1;
			page_table[pte+(1024 *pt)].frames = ((physical) >>12) &0xFFFFF;
			physical += 0x1000;
		}

		page_directory[pde].p = 1;
		page_directory[pde].rw= 1;
		addrtble = (uint32_t)(page_table + (1024 *pt));
		page_directory[pde].addrpt = (addrtble >>12) &0xFFFFF;
		pde++;
		pt++;
		pte =0;


	}while(x--);


	flush_tlb();


	return (pt -1);

}

static void load_page_diretory(void *addr)
{
	__asm__ __volatile__("movl %k0,%%cr3"::"r"(addr));
}

static void page_enable()
{	__asm__ __volatile__("movl %%cr0,%%eax;"
			     "orl $0x80000000,%%eax;"
			     "movl %%eax,%%cr0"::);
}
void __native_flush_tlb_single(uint32_t addr)
{
   	__asm__ __volatile__("invlpg (%0)" ::"r" (addr) : "memory");
}

static void flush_tlb()
{
    	uint32_t addr;
   	__asm__ __volatile__("movl %%cr3,%k0" :"=r" (addr):);
   	__asm__ __volatile__("movl %k0,%%cr3" ::"r" (addr));
}
