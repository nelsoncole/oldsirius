#ifndef __task_h__
#define __task_h__
#include <typedef.h>
typedef struct focus {
	uint32_t id;

}__attribute__((packed)) focus_t;

typedef struct process {
	uint32_t id;
	struct process *next;

}__attribute__((packed)) process_t;
typedef struct regs {

	uint32_t eax;
	uint32_t ebx;
	uint32_t ecx;
	uint32_t edx;
	uint32_t edi;
	uint32_t esi;
	uint32_t ebp;
	uint32_t esp;
	uint32_t eip;
	uint32_t eflag;
    	uint32_t PDBR;
    	uint32_t cs;
	uint32_t ds;
	uint32_t es;
	uint32_t fs;
	uint32_t gs;
    	uint32_t ss;

}__attribute__((packed)) regs_t;

typedef struct task {
    	// ID de tarefa  (conhecido como PID)
    	uint32_t id;
    	// Registradores
    	uint32_t esp0;
    	regs_t regs;
	// linker do próximo     
    	struct task *next;    
}__attribute__((packed)) task_t;


//lib

void init_tasking();
int getpid();
int getcr3();
int create_task(void (*main)(),page_directory_t *page_directory,uint32_t stack,uint32_t stack0,uint8_t privileg);





#endif
