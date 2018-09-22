#ifndef __PIT_H__
#define __PIT_H__
#include <typedef.h>
void timer_install(uint8_t flag);
void delay(uint32_t millesecond);
void timer_handler(void);


#endif
