#ifndef __IDT_H__
#define __IDT_H__
#include <typedef.h>

void idt_install();
void trap(int n,uint32_t offset,uint16_t sel,uint8_t dpl );
void interrupter(int n,uint32_t offset,uint16_t sel,uint8_t dpl);


#endif


