#include <io.h>
#include <gdt.h>
#include <ldt.h>
#include <tss.h>
#include <string.h>

extern void gdt_flush(uint32_t*);

typedef volatile struct gdt {

	unsigned long long limit_15_0 :16; 
	unsigned long long base_15_0 : 16;
	unsigned long long base_23_16 : 8;
	unsigned long long type : 4;
	unsigned long long s:1;
	unsigned long long dpl: 2;
	unsigned long long p:1;
	unsigned long long limit_19_16: 4;
	unsigned long long avl :1;
	unsigned long long l:1;
	unsigned long long db:1;
	unsigned long long g:1; 
	unsigned long long base_31_24 : 8;

}__attribute__((packed)) gdt_t;

typedef volatile struct gdt_ptr {
    	unsigned short limit;
	unsigned long long base;

}__attribute__((packed)) gdt_ptr_t;


gdt_t gdt[256];
gdt_ptr_t gdt_ptr[1];


static void set_gdt(int n,uint32_t limit,uint32_t base,uint8_t type,\
uint8_t s,uint8_t dpl,uint8_t p,uint8_t avl,uint8_t l,uint8_t db,uint8_t g)
{


	gdt[n].limit_15_0 = limit &0xFFFF;
	gdt[n].base_15_0 = base &0xFFFF;
	gdt[n].base_23_16 = base >> 16 &0xFF;
	gdt[n].type=type &0xF;
	gdt[n].s=s &0x1;
	gdt[n].dpl =dpl &0x3;
	gdt[n].p=p &0x1;
	gdt[n].limit_19_16= limit >>16 &0xF;
	gdt[n].avl=avl &1;
	gdt[n].l=l &1;
	gdt[n].db=db &1;
	gdt[n].g=g &1;
	gdt[n].base_31_24 = base  >> 24 &0xFF;

}



void gdt_install(void)
{

	memset(&gdt,0,sizeof(gdt_t)*256);

	//     (n,limit ,base ,type,s,dpl,p,avl,l,db,g)
	set_gdt(0,0,0,0,0,0,0,0,0,0,0);
	set_gdt(1,0xFFFFF,0x00000000,0xA,1,0,1,0,0,1,1); //dpl 0
	set_gdt(2,0xFFFFF,0x00000000,0x2,1,0,1,0,0,1,1); //dpl 0
	set_gdt(3,0xFFFFF,0x00000000,0xA,1,3,1,0,0,1,1); //dpl 3
	set_gdt(4,0xFFFFF,0x00000000,0x2,1,3,1,0,0,1,1); //dpl 3
	set_gdt(5,sizeof(ldt_t)-1,(uint32_t)&ldt,0x2,0,3,1,0,0,1,1); //ldt dpl 3
	set_gdt(6,sizeof(tss_t)-1,(uint32_t)&tss,0x9,0,3,1,0,0,1,1); //tss dpl 3



    	gdt_ptr->limit = (sizeof(gdt_t)*256)-1;
    	gdt_ptr->base = (uint32_t)gdt;
        gdt_flush((uint32_t*)gdt_ptr);

	printf("GDT Install (Memory Base Address %X)\n",(uint32_t)gdt);   
  
}
