#ifndef __tss_h__
#define __tss_h__
#include <typedef.h>


typedef volatile struct tss {

	unsigned short priv;
	unsigned short reserved0;
	unsigned long  esp0;
	unsigned short ss0;
	unsigned short reserved1;
	unsigned long  esp1;
	unsigned short ss1;
	unsigned short reserved2;
	unsigned long  esp2;
	unsigned short ss2;
	unsigned short reserved3;
	unsigned long  cr3;
	unsigned long  eip;
	unsigned long  eflags;
	unsigned long  eax, ecx, edx, ebx, esp, ebp, esi, edi;
	unsigned short es;
	unsigned short reserved4;
	unsigned short cs;
	unsigned short reserved5;
	unsigned short ss;
	unsigned short reserved6;
	unsigned short ds;
	unsigned short reserved7;
	unsigned short fs;
	unsigned short reserved8;
	unsigned short gs;
	unsigned short reserved9;
	unsigned short ldt_sel;
	unsigned short reserved10;
	unsigned short trap;
	unsigned short io_map;

}__attribute__((packed)) tss_t;

tss_t tss[1];

void tss_install(void);
void stack0(uint32_t stack);


#endif
