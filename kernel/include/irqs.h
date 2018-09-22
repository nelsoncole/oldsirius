#ifndef __irqs_h__
#define __irqs_h__
#include <typedef.h>

void irq_install();
void irq_interrupter(uint32_t irq);
void irq_enable(int irq);
void irq_disable(int irq);

#endif
