#include <io.h>
#include <tss.h>
#include <string.h>


static void load_ltr(uint16_t tr){
__asm__ __volatile__ (	"movw %0,%%ax;"
			"ltrw %%ax;"
			::"a"(tr));

}

// Instaloador da TSS
// TODO: O TSS em nosso caso somente seria necessário 
// para perservar e tratar as pilhas dos diferentes níveis de privilégio
// Todo OS que usa mecanismo de proteção de níveis de privilégio 0,1,2 e 3
// mesmo quando o mesmo usa Multitarefa de software, este deve por pelomenos configurar um TSS
void tss_install(void){

	memset(&tss,0,sizeof(tss_t));

	tss->ss0 = 0x10; //RPL 0
	tss->esp0= 0;  // TODO Default 
	tss->cs=0xb; // CPL e RPL 3 user
	tss->ss=tss->ds=tss->es=tss->fs=tss->gs=0x13; // CPL e RPL 3 user

    	tss->esp = 0; // TODO

	load_ltr(0x33);   // RPL 3 user

	printf("TSS Install (Memory Base Address %X)\n",(uint32_t)tss); 

    


}

void stack0(uint32_t stack){
	tss->esp0=stack;
}

