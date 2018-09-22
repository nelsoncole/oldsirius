#ifndef __mm_h__
#define __mm_h__
#include <typedef.h>

void mem_cofg(uint32_t arg);
void kinit_alloc ();

void *kalloc(size_t size);
void kfree_alloc(void *free);

void *mem_map(void * physaddr, uint8_t flg, size_t size);
uint32_t *alloc_frame(page_table_t *pte,uint8_t flg, size_t count);
void free_frame(void *free);

#endif
