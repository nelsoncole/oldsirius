#include <io.h>
#include <gdt.h>
#include <ldt.h>
#include <tss.h>
#include <idt.h>
#include <task.h>
#include <drivers/pic.h>
#include <drivers/pit.h>
#include <drivers/ps2.h>
#include <drivers/keyboard.h>
#include <drivers/mouse.h>
#include <drivers/rtc.h>
#include <drivers/pci.h>
#include <dev.h>
#include <executive/part.h>
#include <fs/fs.h>
#include <fs/vfs.h>
#include <string.h>
#include <system.h>
#include <gui/gui.h>

uint32_t pmboot_info =0;
int os_ready = - 1;

char switching = 0;

char* kmonth [] = {
0,"January","February","March","April","May","June","July",0,0,0,0
}; 

static void task_mother();
static void task_main ();
static void task_test ();
void main (uint32_t arg)
{
	pmboot_info = arg;

	gdt_install();
	ldt_install();
	tss_install();
	idt_install();
	page_install(arg);
	kinit_alloc();
	init_heap();
	mem_cofg(arg);
	init_tasking();
	set_frequencia(FREQ);
    	timer_install(TRUE);
	sti();
	wait_ns(1000000);
	create_task(&task_main,(page_directory_t*)page->kpdbr,(uint32_t)kalloc(1),0,0);
	die();

}


static void task_main () 
{
	cli();
	set_color(0x2);puts("Task main initialize\n");set_color(0xf);
	sti();

	gui_init(pmboot_info);

	//ps2_install     ();
	keyboard_install();
	//mouse_install   ();
	rtc_install     ();
	ata_initialize  ();
    	ehci_init       ();
	hda_init	();
	//gpu		();
	part_initialize ();

	nic_test();
	//create_task(&task_test,(page_directory_t*)page->kpdbr,(uint32_t)kalloc(1),0,0);
	create_task(&task_mother,(page_directory_t*)page->kpdbr,(uint32_t)kalloc(8),0,0);

	
	die();
}
static void task_test () 
{
	cli();
	set_color(0x2);puts("Task test initialize\n");set_color(0xf);
	pci_info(0,2);
	sti();

	
	die();
}

char* kmsg [] = {
"reboot","shutdown","exit",0
};


volatile void *xsyscall[] = {
	0,
	&rdir,
	&cd,
};

static void task_mother() 
{
	uint8_t *sysiomem= (uint8_t *)0x400000;

	cli();
	set_color(0x2);puts("Task mother initialize\n");set_color(0xf);
	sti();
	
	page_table_t *page_table=(page_table_t*)kalloc(1);
    	uint32_t pte_free,physical_pte;
	// allocar memória fisica
    	pte_free = (uint32_t)alloc_frame(page_table,0x7,2);
	kpage_directory[1].p = 1;
	kpage_directory[1].rw= 1;
	kpage_directory[1].us= 1;
	physical_pte = (uint32_t)page_lookups((page_table + (0*1024)));
	kpage_directory[1].addrpt = (physical_pte >>12) &0xFFFFF;
	memset((char*)0x400000,0,4096*2);


	os_ready = 0;
	

	while(os_ready) nop();
	set_color(0x2);puts("OS ready for use\n");set_color(0xf);
	wait_ns(1000);

	char filename[] = "d0:/System/Bin/shell.bin";
    	int focos = execve(filename,0,0);

	if(focos) (*(uint32_t*)(sysiomem )) = focos;
	
	uint32_t oldcr3;
	uint32_t i,ret = -1;


	// big loop 
	while (true) {

		while ( !(*(uint32_t*)(sysiomem +0x04)) ) hlt();// nop();

		
		i = (*(uint32_t*)(sysiomem +0x04));
		asm volatile ("movl %%cr3,%k0":"=r"(oldcr3):);

		switching = 1;
		
		load_page_diretory((page_directory_t *)(*(uint32_t*)(sysiomem +0xC)));

		(*(uint32_t*)(sysiomem +0x08)) = 1;

		void *addr = (uint32_t*)xsyscall[i];
		char *path = (char*)(*(uint32_t*)(sysiomem +0x28));

		ret = xcall(addr,path,0,0,0);

		
		(*(uint32_t*)(sysiomem +0x24)) = ret;
		(*(uint32_t*)(sysiomem +0x08)) = 0;
		(*(uint32_t*)(sysiomem +0x04)) = 0;

		load_page_diretory((page_directory_t *)oldcr3);

		switching = 0;

		
	}


	die();
}
