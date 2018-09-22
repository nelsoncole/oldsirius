#include <io.h>
#include <irqs.h>
#include <drivers/pit.h>
#define FREQ 100

static void timer_wait(uint32_t ticks);

uint32_t timer_ticks =0 ;
uint32_t eticks;

void set_frequencia(uint32_t freq){

	uint32_t divisor = 1193180/freq;


	cli();
	outportb(0x43,0x36);	/* Control WORD register
				 * bits 7-6 = 0 - Set counter 0 (counter divisor),bit 5-4=11 LSB/MSB 16bits
				 * bit 3-1=x11 Square wave generator, bit 0 =0 Binary counter 16bits
				 */
	
	outportb(0x40,(uint8_t)(divisor & 0xFF));		// 0x40 counter 0
	outportb(0x40,(uint8_t)(divisor >> 8) & 0xFF);

	irq_enable(0);
	sti();
}


void timer_install(uint8_t flag){

    	set_frequencia(FREQ);

	if(flag == 1) irq_enable(0);
	else irq_disable(0);

}


void timer_handler(){

	++timer_ticks;
	if(eticks)--eticks;

}

void delay(uint32_t millesecond){

	timer_wait(FREQ * millesecond/1000);

}



static void timer_wait(uint32_t ticks){

	timer_install(FALSE);
	eticks = ticks;
	timer_install(TRUE);

	while(eticks)
	update_cursor();	
	

	

}
