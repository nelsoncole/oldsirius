#ifndef __mouse_h__
#define __mouse_h__
#include <typedef.h>
void mouse_write(uint8_t a_write);
uint8_t mouse_read();

void mouse_install();
void mouse_handler(void);


#endif
