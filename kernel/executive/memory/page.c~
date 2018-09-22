#include <io.h>
#include <string.h>
#include <memory/page.h>

void page_install(uint32_t arg)
{	

	char *mboot = (char*)arg;

	kpage_directory = (page_directory_t *) (*(uint32_t*)(mboot + 20));
	kpage_table = (page_table_t *) (*(uint32_t*)(mboot + 24));

	page->total_directory =1;
	page->total_table = (*(uint32_t*)(mboot +4084)); 
	page->used_kdirectory = (*(uint32_t*)(mboot +4088));
	page->used_ktables = (*(uint32_t*)(mboot +4092));

	page->end_kdirectory = 768 + (*(uint32_t*)(mboot +4088)) -1;
	page->end_ktable = (*(uint32_t*)(mboot +4092));


	__asm__ __volatile__ ("movl %%cr3,%%eax":"=a"(page->kpdbr));
	


}
void *page_lookups(void *virtualaddr) 
{
	int d = (int)virtualaddr >> 22 & 0x3FF;
    	int t = (int)virtualaddr >> 12 & 0x3FF;
    	int o = (int)virtualaddr  & 0xFFF;

	page_table_t *page_table = (page_table_t *)(kpage_directory[d].addrpt <<12);

	return (uint32_t*) ((page_table[t].frames <<12) + o);

}

void load_page_diretory(page_directory_t *addr)
{
	__asm__ __volatile__("movl %k0,%%cr3"::"r"(addr));
	wait_ns(120); // Wait Translation Lookaside Buffer (TLB)
}

void page_enable()
{	__asm__ __volatile__("movl %%cr0,%%eax;"
			     "orl $0x80000000,%%eax;"
			     "movl %%eax,%%cr0"::);
}
void __native_flush_tlb_single(uint32_t addr)
{
   	__asm__ __volatile__("invlpg (%0)" ::"r" (addr) : "memory");
	wait_ns(120);
}

void flush_tlb()
{
    	uint32_t addr;
   	__asm__ __volatile__("movl %%cr3,%k0" :"=r" (addr):);
   	__asm__ __volatile__("movl %k0,%%cr3" ::"r" (addr));
	wait_ns(120);
}
