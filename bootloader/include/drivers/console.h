#ifndef __CONSOLE_H__
#define __CONSOLE_H__
#include <typedef.h>

void update_cursor();
void set_cursor(int x, int y);
void set_cursor_x(int x);
void set_cursor_y(int y);
void set_color(const unsigned c);
void cls();
void putchar(uint8_t ch);
void puts(char* s);
void printf(const char *format,...);




#endif
