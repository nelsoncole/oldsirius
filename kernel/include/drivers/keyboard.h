#ifndef __keyboard_h__
#define __keyboard_h__
#include <typedef.h>
uint8_t keyboarde_read();
void keyboard_write(uint8_t write);
void keyboard_install();
void keyboard_handler(void);

unsigned char charset();

#endif
