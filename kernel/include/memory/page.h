#ifndef __page_h__
#define __page_h__
#include <typedef.h>
typedef struct page_table
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

typedef struct page_directory
	{
	uint32_t p :1;
	uint32_t rw :1;
	uint32_t us :1;
	uint32_t pwt :1;
	uint32_t pcd :1;
	uint32_t a :1;
	uint32_t ign1 :1;
	uint32_t ps :1;
	uint32_t ign2 :4;
	uint32_t addrpt :20;

	}__attribute__((packed)) page_directory_t;

page_directory_t *kpage_directory;
page_table_t *kpage_table;


typedef struct {
	uint32_t total_directory;
	uint32_t total_table;
	uint32_t end_kdirectory;
	uint32_t end_ktable;
	uint32_t used_kdirectory;
	uint32_t used_ktables;
	uint32_t kpdbr;

}page_t;
 
page_t page[1];

void page_install(uint32_t arg);
void *page_lookups(void *virtualaddr);
void page_enable();
void load_page_diretory(page_directory_t *addr);
void __native_flush_tlb_single(uint32_t addr);
void flush_tlb();

#endif
