#include <io.h>
inline uint8_t inb(uint16_t p)
{
	uint8_t ret_val; 
	__asm__ __volatile__("inb %w1,%b0":"=a"(ret_val):"dN"(p));
	return ret_val;
}
inline uint16_t inw(uint16_t p)
{
	uint16_t ret_val;
	__asm__ __volatile__("inw %w1,%w0":"=a"(ret_val):"dN"(p));
	return ret_val;
}
inline void outb(uint16_t p,uint8_t val)
{
	__asm__ __volatile__("outb %b0, %w1"::"a"(val),"dN"(p));
}

inline void outw(uint16_t p,uint16_t val)
{
	__asm__ __volatile__("outw %w0, %w1"::"a"(val),"dN"(p));
}

inline void wait_ns(int count)
{
	count /=100;	 
	while(--count)io_delay();

}

void pause() 
{
	__asm__ __volatile__("pause":::"memory");

}

void die(void)
{
	while(1)hlt();

}
