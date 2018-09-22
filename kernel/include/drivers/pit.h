#ifndef __pit_h__
#define __pit_h__
#include <typedef.h>
#define FREQ 100

void set_frequencia(uint32_t freq);
void timer_install(uint8_t flag);
void timer_handler(void);
void delay(uint32_t milesegundo);
void sleep(uint32_t segundos);

#endif
