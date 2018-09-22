#ifndef __STDIO_H__
#define __STDIO_H__

#include "ctype.h"




void cls();
void color(int val);
int8_t putchar(int8_t ch);
void puts(int8_t *string);
void printf(const char *format,...);
int8_t getchar();
void gets(int8_t *buffer);
void scanf(const char *format,...);










#endif
