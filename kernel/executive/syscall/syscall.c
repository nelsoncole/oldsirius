#include <io.h>
#include <system.h>
#include <executive/syscall.h>
#include <fs/fs.h>
#include <drivers/keyboard.h>

#define SYSCALL_NUM 32 + 8
static void *syscall_table[];

DEFN_SYSCALL0(cls,32);
DEFN_SYSCALL1(putchar,33, int);
DEFN_SYSCALL1(puts,34, const char*);

static void defaultsyscall(){

    puts("SYSCALL NOT INSTALED\n");

}

static void sys_wait()
{
    hlt();
}

static void sys_reboot()
{
    reboot();

}

static void *syscall_table[SYSCALL_NUM]={
    	&sys_wait,
    	&defaultsyscall,       	// eax, 1    sys_exit
    	&defaultsyscall,        // eax, 2    sys_execve
    	&defaultsyscall,       	// eax, 3    sys_fork
    	&getpid,            	// eax, 4    sys_getpid
    	&getcr3,       		// eax, 5    sys_getcr3
    	&sys_reboot,          	// eax, 6
    	&defaultsyscall,
    	&defaultsyscall,
    	&defaultsyscall,
    	&defaultsyscall,
    	&defaultsyscall,
    	&defaultsyscall,
    	&defaultsyscall,
    	&defaultsyscall,
    	&defaultsyscall,
    	&defaultsyscall,
    	&defaultsyscall,
    	&defaultsyscall,
    	&defaultsyscall,
    	&defaultsyscall,
    	&defaultsyscall,
    	&defaultsyscall,
    	&defaultsyscall,
    	&defaultsyscall,
    	&defaultsyscall,
    	&defaultsyscall,
    	&defaultsyscall,
    	&defaultsyscall,
    	&defaultsyscall,
    	&rdir,			// eax, 30
    	&cd,			// eax, 31   
	&cls,		    	// eax, 32   sys_cls
	&putchar, 		// eax, 33   sys_char
	&puts, 		    	// eax, 34   sys_puts
    	&set_color,         	// eax, 35   sys_color
    	&charset,           	// eax, 36   sys_charset
    	&set_cursor,        	// eax, 27
    	&set_cursor_x,      	// eax, 28
    	&set_cursor_y       	// eax, 29
	};


static void invalidsyscall(int num)
{

    	set_color(4);
	printf("Invalid syscall: EAX,%d INT 0x69\n",num);
	set_color(0xF);



}

int syscall_handler(uint32_t num){

    	int eax =0;

    	if(num < SYSCALL_NUM) {

		void *addr =syscall_table[num];

		__asm__ __volatile__ ("\
		pushl %%ebx;\
		pushl %%ecx;\
		pushl %%edx;\
		call *%k1;\
    		addl $12,%%esp;\
		":"=a"(eax):"r"(addr));

	}else {

    		__asm__ __volatile__ ("\
		pushl %%ebx;\
		pushl %%ecx;\
		pushl %%edx;\
		call *%k1;\
    		addl $12,%%esp;\
		":"=a"(eax):"r"(invalidsyscall),"d"(num));
    	}

    	return (eax);
}
