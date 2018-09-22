#include <io.h>
#include <idt.h>
#include <exceptions.h>
#include <irqs.h>
#include <executive/syscall.h>
#include <string.h>
extern void idt_flush(uint32_t*);

typedef volatile struct idt 
{

    	unsigned long long offset_15_0 :16; 
	unsigned long long sel : 16;
    	unsigned long long unused :8;
	unsigned long long flags : 5;
	unsigned long long dpl : 2;
    	unsigned long long p :1;
	unsigned long long offset_31_16 : 16;


}__attribute__ ((packed)) idt_t;

typedef volatile struct idt_ptr {
    	unsigned short limit;
	unsigned long long base;

}__attribute__((packed)) idt_ptr_t;



idt_t idt[256];
idt_ptr_t idt_ptr[1];


static void set_gate_idt(int n,uint32_t offset,uint16_t sel,uint8_t  flags,uint8_t  dpl,uint8_t  p);

void idt_install(void)
{

    	memset(&idt,0,(sizeof(idt_t)*256)-1);    
    	exceptions_install();
    	irq_install();
	interrupter(0x69,(uint32_t)int69,0x8,3);

	idt_ptr->limit = (sizeof(idt_t)*256)-1;
    	idt_ptr->base = (uint32_t)idt;
    
    	idt_flush((uint32_t*)idt_ptr);
	printf("IDT Install (Memory Base Address %X)\n",(uint32_t)idt); 

}


void trap(int n,uint32_t offset,uint16_t sel,uint8_t dpl )
{
    	set_gate_idt(n,offset,sel,0x8F,dpl,1);

}

void interrupter(int n,uint32_t offset,uint16_t sel,uint8_t dpl )
{
	set_gate_idt(n,offset,sel,0x8E,dpl,1);

}



static void set_gate_idt(int n,uint32_t offset,uint16_t sel,uint8_t flags,uint8_t dpl,uint8_t p)
{

            idt[n].offset_15_0 = offset &0xFFFF;
            idt[n].sel = sel;
            idt[n].unused = 0;
            idt[n].flags = flags;
            idt[n].dpl = dpl;
            idt[n].p = p;
            idt[n].offset_31_16 = offset >> 16 &0xFFFF;



}
