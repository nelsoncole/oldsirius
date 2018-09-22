#include <io.h>
#include <task.h>
#include <system.h>
#include <fs/fs.h>
#include <string.h>

extern task_t *current_task;
extern uint32_t  next_pid;

static int do_exec(char *filename, uint8_t prv);

int execve(char *filename,int argc,char *argv[])
{
    int pid = do_exec(filename,1);
    
    if(pid ==0) return (0);
    return (pid);

}


static int do_exec(char *filename,uint8_t prv)
{

	/*
	uint32_t *buffer = (uint32_t*)kalloc(2);
	open_read(buffer,filename))	*/
	
	int pid = 0;

     
    	uint8_t access = ((prv == 0)? 3 : 7);
    	char *addr =(char*)APP_MEMORY_VIRTUAL_START;

    	// alloc new_task, new_page directory
    	page_directory_t *page_directory = (page_directory_t*)kalloc(1);
    	page_table_t *page_table=(page_table_t*)kalloc(1);
	uint32_t oldcr3;
    	uint32_t pte_free;
	
	uint32_t physical_pte,physical_pde, d =0;

    	// PDEs

	memset(page_directory,0,sizeof(page_directory_t)*1024);

	memcpy(page_directory,kpage_directory,sizeof(page_directory_t)*2); // copiar as paginas identidade
	memcpy(page_directory+768,kpage_directory +768,sizeof(page_directory_t)*(1024-768)); // copiar as paginas do kernel code


	
	d = ((uint32_t)APP_MEMORY_VIRTUAL_START >> 22) &0x3ff;

    	// allocar memória fisica
    	pte_free = alloc_frame(page_table,access,1024);

	page_directory[d].p =(access)&1;
	page_directory[d].rw=(access >>1) &1;
	page_directory[d].us=(access >>2) &1;
	physical_pte = (uint32_t)page_lookups((page_table + (0*1024)));
	page_directory[d].addrpt = (physical_pte >>12) &0xFFFFF;

	physical_pde =(uint32_t)page_lookups(page_directory);

    	__asm__ __volatile__("movl %%cr3,%k0":"=a"(oldcr3));
    	load_page_diretory((page_directory_t*)physical_pde);

    	if((vfs_read(addr,filename)) != 0 ){
    		syscall_puts(filename);
    		syscall_puts(": achive, not read\n");
    		kfree_alloc(page_table);
    		kfree_alloc(page_directory);
    		free_frame(pte_free);
    		load_page_diretory((page_directory_t*)oldcr3);
    		return -1;
    	}else {


		pid = create_task((void*)(addr),(page_directory_t*)physical_pde,(0x2200000),(uint32_t)kalloc(8),prv);

	}
  
	load_page_diretory((page_directory_t*)oldcr3);
    

    	return (pid);

}



int fork()
{
    	cli();

     	uint32_t data, code;
    	//page_directory_t *page_directory = (page_directory_t*)kalloc(1); //unused

    	// clone directory

    	// ponteiro da estrutura task que nos referimos (current_task)
    	task_t *parent_task = (task_t*)current_task;

    	// criar novo processo
    	task_t *new_task = (task_t*)malloc(sizeof(task_t));
    	new_task->id = next_pid++;


    	new_task->regs.eflag =0x3202;   
    	//new_task->regs.PDBR = (_u32*)clone directory;
    	new_task->next = NULL;



    	// adiciona novo elemento no final da lista


    	// verifica parente
    	if (current_task == parent_task) {
        	// We are the parent, so set up the esp/ebp/eip for our child.
        	__asm__ __volatile__("movl %%esp, %k0" : "=r"(new_task->regs.esp));
        	__asm__ __volatile__("movl %%ebp, %k0" : "=r"(new_task->regs.ebp));
        	__asm__ __volatile__("movl %%ebp, %k0" : "=r"(data));
        	__asm__ __volatile__("movl %%ebp, %k0" : "=r"(code));
        	new_task->regs.eip = 0;
        	new_task->regs.cs = code;
        	new_task->regs.ds = data;
        	new_task->regs.es = data;
        	new_task->regs.fs = data;
        	new_task->regs.gs = data;
        	new_task->regs.ss = data;

        	sti();
        return new_task->id;
    	} else {
        	sti();
        	return 0;
    	}

}
