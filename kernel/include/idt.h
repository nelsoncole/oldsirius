#ifndef __idt_h__
#define __idt_h__
#include <typedef.h>

void idt_install(void);
void trap(int n,uint32_t offset,uint16_t sel,uint8_t dpl );
void interrupter(int n,uint32_t offset,uint16_t sel,uint8_t dpl);


#endif


