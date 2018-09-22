#ifndef __io_h__
#define __io_h__
#include <typedef.h>
#define FALSE	0
#define TRUE 	1
#define NULL 	0
#define false 	0
#define true 	1
#define null 	0

#define xcall(addr,arg1,arg2,arg3,arg4)({\
uint32_t ret;\
__asm__ __volatile__("\
push %%edx;\
push %%ecx;\
push %%ebx;\
push %%eax;\
call *%k1;\
addl $0x16,%%esp":"=a"(ret):"r"(addr),"a"(arg1),"b"(arg2),"c"(arg3),"d"(arg4));ret;})

// esta macro conver seg:offset para flat
#define __ptr_cov32(p)\
( (16 * (p >>16 &0xFFFF)) + (p &0xFFFF) )

//IO Delay
#define io_delay() __asm__ __volatile__("out %%al,$0x80"::)

//IO R/W BYTE
#define inportb(p)({\
	uint8_t val;\
	__asm__ __volatile__("in %w1,%b0":"=a"(val):"d"(p));val; })

#define outportb(p,val)__asm__ __volatile__("out %b0,%w1"::"a"(val),"d"(p))

#define inpb(p)({\
	uint8_t val;\
	__asm__ __volatile__("in %w1,%b0":"=a"(val):"d"(p));val; })

#define outpb(p,val)__asm__ __volatile__("out %b0,%w1"::"a"(val),"d"(p))


//IO R/W WORD
#define inportw(p)({\
	uint32_t val;__asm__ __volatile__("inw %w1,%w0":"=a"(val):"d"(p));val; })

#define outportw(p,val) __asm__ __volatile__ ("outw %w0,%w1"::"a"(val),"d"(p))

#define inpw(p)({\
	uint32_t val;__asm__ __volatile__("inw %w1,%w0":"=a"(val):"d"(p));val; })

#define outpw(p,val) __asm__ __volatile__ ("outw %w0,%w1"::"a"(val),"d"(p))


//IO R/W DWORD
#define inportl(p)({\
	uint32_t val;\
	__asm__ __volatile__("inl %w1,%k0":"=a"(val):"d"(p));val; })

#define outportl(p,val) __asm__ __volatile__ ("outl %k0,%w1"::"a"(val),"d"(p))

#define inpl(p)({\
	uint32_t val;\
	__asm__ __volatile__("inl %w1,%k0":"=a"(val):"d"(p));val; })

#define outpl(p,val) __asm__ __volatile__ ("outl %k0,%w1"::"a"(val),"d"(p))

#define cli() __asm__ __volatile__ ("cli")
#define sti() __asm__ __volatile__ ("sti")
#define hlt() __asm__ __volatile__ ("hlt")
#define nop() __asm__ __volatile__ ("nop")

// ASSEMBLY
uint8_t inb(uint16_t p);
uint16_t inw(uint16_t p);
void outb(uint16_t p,uint8_t val);
void outw(uint16_t p,uint16_t val);

void wait_ns(int count);
void pause();
void die(void);

#include <drivers/console.h>
#include <memory/heap.h>
#include <memory/page.h>
#include <memory/mm.h>
#include <executive/syscall.h>
#include <task.h>
#include <dev.h>


#endif





