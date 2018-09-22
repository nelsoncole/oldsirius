#include <io.h>
#include <string.h>
#include <ldt.h>


static void set_ldt(int n,uint16_t limit,uint32_t base,uint16_t flags){


	ldt[n].limit_15_0 = limit &0xFFFF;
	ldt[n].base_15_0 = base &0xFFFF;
	ldt[n].base_23_16 = base >> 16 &0xFF;
	ldt[n].flags = flags &0xFFFF;
	ldt[n].base_31_24 = base  >> 24 &0xFF;

}


static void load_ldt(uint16_t lldt){
__asm__ __volatile__ (	"movw %0,%%ax;"
			"lldt %%ax;"
			::"a"(lldt));

}

// Instalador do LDT
void ldt_install(void)
{

	memset(&ldt,0,sizeof(ldt_t)*256);

	set_ldt(0,0,0,0);
	set_ldt(1,0xFFFF,0,0xCFFA);	// 0x08 + 4 + dpl3  = 0xF
	set_ldt(2,0xFFFF,0,0xCFF2);	// 0x10 + 4 + dpl3  = 0x17
	
    	// LLDT
	load_ldt(0x2B);
	printf("LDT Install (Memory Base Address %X)\n",(uint32_t)ldt); 

}
