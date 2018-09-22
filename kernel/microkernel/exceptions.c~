#include <io.h>
#include <idt.h>
#include <exceptions.h>


extern void isr00();	extern void isr10();	extern void isr20();	extern void isr30();		
extern void isr01();	extern void isr11();	extern void isr21();	extern void isr31();
extern void isr02();	extern void isr12();	extern void isr22();
extern void isr03();	extern void isr13();	extern void isr23();
extern void isr04();	extern void isr14();	extern void isr24();
extern void isr05();	extern void isr15();	extern void isr25();
extern void isr06();	extern void isr16();	extern void isr26();
extern void isr07();	extern void isr17();	extern void isr27();
extern void isr08();	extern void isr18();	extern void isr28();
extern void isr09();	extern void isr19();	extern void isr29();


char *exception_mensagem[32]={
	"Divide error\n",		// 0 - fault  -- no error 
	"Debug exception\n", 	// 1 - fault/trap -- no error 
	"NMI Interrupter\n",	//  2 - interrupter -- no error
	"Breakpoint\n",		// 3 - trap -- no error
	"Overflow\n",		// 4 - trap -- no error
	"BOUND Ranger exception\n", // 5 - trap -- no error
	"Invalide opcode (Undefined opcode)\n", // 6 - fault -- no error
	"Device not avaliable (Not Math coprocessor\n)", // 7 - fault -- no error
	"Double Fault (Erro de codigo)\n",		// 8 - about -- error code
	"Coprocessor segment overrun (reservado)\n",	// 9 - fault -- no error
	"Invalide TSS (Erro de codigo)\n", 	// 10 - fault -- error code
	"Segment not present (Erro de codigo)\n",		// 11 - fault -- error code
	"Stack segment fault (Erro de codigo)\n",	// 12 - fault -- error code
	"General protetion (Erro de codigo)\n",	// 13 - fault -- error code
	"Page fault (Erro de codigo)\n",		// 14 - fault -- error code
	"Intel reserved do not use 15\n",		// 15
	"x87 FPU Floating-Point error (Math fault)\n",	// 16 - fault -- no error
	"Alignment check (Erro de codigo)\n",		// 17 - fault -- error code
	"Machine check\n",	// 18 - about -- no error 
	"SIND Floating-Point exception\n", // 19 - fault -- no error
	"Virtualization exception\n",	// 20 - fault -- no error
	"Intel reserved do not use 21\n",	// 21
	"Intel reserved do not use 22\n",	// 22
	"Intel reserved do not use 23\n",	// 23
	"Intel reserved do not use 24\n",	// 24
	"Intel reserved do not use 25\n",	// 25
	"Intel reserved do not use 26\n",	// 26
	"Intel reserved do not use 27\n",	// 27
	"Intel reserved do not use 28\n",	// 28
	"Intel reserved do not use 29\n",    // 29
	"Intel reserved do not use 30\n",	// 30
	"Intel reserved do not use 31\n",	// 31

};



void fault_exception(uint32_t  n)
{
	uint32_t  cr2;

	if(n == 8 || n == 18)set_color(0x4);
	else set_color(0xE);
	puts(exception_mensagem[n]);
	if(n == 14){

		__asm__ __volatile__("movl %%cr2,%k0":"=a"(cr2):);
		printf("\nPage Fault CR2 = %X",cr2);

		}
	for (;;);	

}



void exceptions_install()
{
    	trap(0x00,(uint32_t)isr00,0x8,0);
	trap(0x01,(uint32_t)isr01,0x8,0);
    	interrupter(0x02,(uint32_t)isr02,0x8,0);
	trap(0x03,(uint32_t)isr03,0x8,0);
	trap(0x04,(uint32_t)isr04,0x8,0);
	trap(0x05,(uint32_t)isr05,0x8,0);
	trap(0x06,(uint32_t)isr06,0x8,0);
	trap(0x07,(uint32_t)isr07,0x8,0);
	trap(0x08,(uint32_t)isr08,0x8,0);
	trap(0x09,(uint32_t)isr09,0x8,0);
	trap(0x0A,(uint32_t)isr10,0x8,0);
	trap(0x0B,(uint32_t)isr11,0x8,0);
	trap(0x0C,(uint32_t)isr12,0x8,0);
	trap(0x0D,(uint32_t)isr13,0x8,0);
	trap(0x0E,(uint32_t)isr14,0x8,0);
	trap(0x0F,(uint32_t)isr15,0x8,0);
	trap(0x10,(uint32_t)isr16,0x8,0);
	trap(0x11,(uint32_t)isr17,0x8,0);
	trap(0x12,(uint32_t)isr18,0x8,0);
	trap(0x13,(uint32_t)isr19,0x8,0);
	trap(0x24,(uint32_t)isr20,0x8,0);
	trap(0x15,(uint32_t)isr21,0x8,0);
	trap(0x16,(uint32_t)isr22,0x8,0);
	trap(0x17,(uint32_t)isr23,0x8,0);
	trap(0x18,(uint32_t)isr24,0x8,0);
	trap(0x19,(uint32_t)isr25,0x8,0);
	trap(0x1A,(uint32_t)isr26,0x8,0);
	trap(0x1B,(uint32_t)isr27,0x8,0);
	trap(0x1C,(uint32_t)isr28,0x8,0);
	trap(0x1D,(uint32_t)isr29,0x8,0);
	trap(0x1E,(uint32_t)isr30,0x8,0);
	trap(0x1F,(uint32_t)isr31,0x8,0);
}
