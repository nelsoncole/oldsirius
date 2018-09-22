#ifndef __IO_H__
#define __IO_H__
#include <typedef.h>
#define FALSE	0
#define TRUE 	1
#define NULL 	0
#define false 	0
#define true 	1
#define null 	0

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
void die(void);


// mboot
uint32_t mboot_device (); 

// ---
#include <dev.h>

// Includes memory
#include <memory/heap.h>

// Includes drivers
#include <drivers/console.h>

// Includes fs
#include <fs/fs.h>
#include <part.h>



#endif
