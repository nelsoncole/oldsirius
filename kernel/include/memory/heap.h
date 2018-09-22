#ifndef __heap_h__
#define __heap_h__
#include <typedef.h>
void init_heap();
uint32_t malloc(size_t size);
void free(void *buffer);


#endif
