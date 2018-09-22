#ifndef __rtc_h__
#define __rtc_h__
#include <typedef.h>

void disable_NMI();
void enable_NMI();
void rtc_install();
void rtc_handler(void);

uint8_t read_time(int type);



#endif
