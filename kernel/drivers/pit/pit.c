#include <io.h>
#include <irqs.h>
#include <drivers/pit.h>

uint32_t timer_ticks = 0;
uint32_t  eticks = 0;

void set_frequencia(uint32_t freq)
{

	uint32_t divisor = 1193180/freq;


	cli();

    	// Control uint16_t register
	// bits 7-6 = 0 - Set counter 0 (counter divisor),bit 5-4=11 LSB/MSB 16-bit
	// bit 3-1=x11 Square wave generator, bit 0 =0 Binary counter 16-bit
	outportb(0x43,0x36);
	outportb(0x40,(uint8_t)(divisor & 0xFF));		
	outportb(0x40,(uint8_t)(divisor >> 8) & 0xFF);

	sti();
}


static void timer_wait(uint32_t  ticks)
{


	timer_install(FALSE);
	eticks = ticks;
	timer_install(TRUE);

	while(eticks){
		update_cursor();	
	}



}


void sleep(uint32_t segundos)
{
	timer_wait(FREQ * segundos);

}

void delay(uint32_t  milesegundo)
{

	timer_wait(FREQ * milesegundo/1000);

}


void timer_handler(void)
{

	//++timer_ticks;
	if(eticks)--eticks;

}



void timer_install(uint8_t flag)
{

	if(flag == 1) irq_enable(0);
	else irq_disable(0);

}
