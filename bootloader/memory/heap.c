#include <memory/heap.h>

uint32_t HEAP_BASE = 0x0300000; // 0x70000-0x90000 128KB (32alloc)

void *malloc(size_t size)
{	

	uint32_t *addr = (uint32_t*)HEAP_BASE;
	HEAP_BASE += 0x2000;
	return (addr);
}
void free(void *free)
{
	HEAP_BASE -= 0x2000;
}


