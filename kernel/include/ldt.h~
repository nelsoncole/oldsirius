#ifndef __ldt_h__
#define __ldt_h__
#include <typedef.h>
typedef volatile struct ldt {

	unsigned long long limit_15_0 :16; 
	unsigned long long base_15_0 : 16;
	unsigned long long base_23_16 : 8;
	unsigned long long flags : 16;
	unsigned long long base_31_24 : 8;

}__attribute__((packed)) ldt_t;


// Deve ser global para o GDT
ldt_t ldt[256];
void ldt_install(void);





#endif
