#ifndef __IRQS_H__
#define __IRQS_H__
#include <typedef.h>

void irq_install();
void irq_interrupter(uint32_t irq);
void irq_enable(int irq);
void irq_disable(int irq);

#endif
