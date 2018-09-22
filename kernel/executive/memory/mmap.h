#ifndef __mmap_h__
#define __mmap_h__

#define MMAP_RSTART  0x00000000
#define MMAP_REND    0x00400000
#define MMAP_KSTART  0xC0000000
#define MMAP_KEND    0xE0000000
#define MMAP_DSTART  0xE0000000
#define MMAP_DEVEND  0xFFFFFFFF

// Globais
unsigned long mmap_device = MMAP_DSTART; 


#endif
